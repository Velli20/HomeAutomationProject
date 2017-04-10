#include "RoomConfiguration.h"
#include "UI_resources.h"

/* Private variables */
static FATFS SDFatFs;              /* File system object for SD card logical drive */
static FIL RoomConfigurationFile;  /* File object structure*/
static char SDPath[4];             /* SD card logical drive path */


/* ROOMLIST where room configurations is stored */
struct RoomList * roomConfiguration = NULL;
/* Callback to notify if state of some widget is changed*/
OnWidgetStateChangedCallback onWidgetStateChangedCallback;
/* Callback to notify if RoomConfiguration is changed */
OnRoomConfigurationChangedCallback onRoomConfigurationChangedCallback;


/* Public code */

/**
 * @brief  Read configuration file (conf.XML) from µSD card.
 * @param  None
 * @retval ROOM_CONFIGURATION_RESULT_OK if operation was successful
 */
ROOM_CONFIGURATION_READ_RESULT RoomConfiguration_ReadFromSdCard(void) {
	if(roomConfiguration) {
		/* Clear current RoomConfiguration instance */
		RoomWidget_FreeRoomList(roomConfiguration);
		roomConfiguration = NULL;

		if(onRoomConfigurationChangedCallback) {
			/* Notify callback that configuartion was deleted */
			onRoomConfigurationChangedCallback(ROOM_CONFIGURATION_DELETED);
		}
	}
	/* Check if uSd card is mounted */
	if (BSP_SD_IsDetected() != SD_PRESENT) {
		return ROOM_CONFIGURATION_RESULT_NO_SD;
	}

	/* Result code for this function */
	ROOM_CONFIGURATION_READ_RESULT res = ROOM_CONFIGURATION_RESULT_OK;

	/* Variable where we store how many bytes was read */
	uint32_t bytesread;

	/* Link the micro SD disk I/O driver */
	if (FATFS_LinkDriver(&SD_Driver, SDPath) != 0) {
		return ROOM_CONFIGURATION_RESULT_UNABLE_TO_LINK_SD_DRIVER;
	}

	/* Register the file system object to the FatFs module */
	if (f_mount(&SDFatFs, SDPath, 0) != FR_OK) {
		res = ROOM_CONFIGURATION_RESULT_UNABLE_TO_MOUNT_SD;
		goto end;
	}

	/* Open the configuration file object with read access */
	if (f_open(&RoomConfigurationFile, CONFIGURATION_FILE_NAME, FA_READ) != FR_OK) {
		res = ROOM_CONFIGURATION_RESULT_NO_FILE;
		goto end;
	}

	/* Get byte count of the configuration file */
	size_t configFileSize = f_size(&RoomConfigurationFile);
	if (configFileSize == 0) {
		res = ROOM_CONFIGURATION_RESULT_CONFIGURATION_FILE_EMPTY;
		goto end;
	}

	/* Allocate memory for the configuration string */
	char * configFileString = calloc(configFileSize, sizeof(char));

	if (configFileString) {
		/* If memory allocation was successful Read data from the xml file */
		FRESULT read = f_read(&RoomConfigurationFile, configFileString, configFileSize, (UINT*) &bytesread);

		if (read != FR_OK || bytesread < configFileSize) {
			/* Set error if read byte count is less than size of file we are reading */
			res = ROOM_CONFIGURATION_RESULT_UNABLE_TO_READ_FILE;
			goto end;
		}
		/* Null terminate file string */
		configFileString[configFileSize-1] = '\0';

		/* Parse the room configuration file */
		roomConfiguration = RoomWidgetParser_ParseString(configFileString, bytesread);

		if (!roomConfiguration) {
			res = ROOM_CONFIGURATION_RESULT_PARSING_FAILED;
		} else {
			if(onRoomConfigurationChangedCallback) {
				/* Notify callback that configuration was created */
				onRoomConfigurationChangedCallback(ROOM_CONFIGURATION_CREATED);
			}
		}
	} else {
		res = ROOM_CONFIGURATION_RESULT_NO_MEMORY;
	}


	end:

	/* Unlink the micro SD disk I/O driver and close file*/
	f_close(&RoomConfigurationFile);
	FATFS_UnLinkDriver(SDPath);
	if(configFileString) {
		free(configFileString);
	}

	return res;
}

/**
 * @brief  Change state of widget(s)
 * @param  a_node: XML node containing parameters to update
 * @retval Returns 1 if operation was successful, otherwise 0 if an error occured
 */
int RoomConfiguration_UpdateRoomState(const ezxml_t a_node) {
	int result = ROOM_UPDATE_RESULT_ERROR;
	if(!a_node || !roomConfiguration) {
		return result;
	}


	/* Try to parse data in XML node */
	struct RoomList* roomConfigurationUpdate = RoomWidgetParser_ParseData(a_node);

	if(roomConfigurationUpdate) {
		struct Room* ptrRoom = roomConfigurationUpdate->head;
		/* Iterate trough RoomWidgets in roomConfigurationUpdate list */
		while (ptrRoom) {

			if (ptrRoom->widgets && ptrRoom->widgets->head) {
				struct RoomWidget* ptrWidget = ptrRoom->widgets->head;

				while (ptrWidget) {
					/* Try to get a widget that matches id */
					struct RoomWidget * ptrWidgetToUpdate = RoomWidget_GetRoomWidgetWithId(roomConfiguration, ptrWidget->id);

					if(ptrWidgetToUpdate) {
						if(ptrWidgetToUpdate->type != WIDGET_TYPE_THERMOSTAT) {
							/* Avoid overwriting read only values */
							ptrWidgetToUpdate->intValue = ptrWidget->intValue;

							if(ptrWidget->intValueTarget < MAX_THERMOSTAT_TEMPERATURE
									&& ptrWidget->intValueTarget > MIN_THERMOSTAT_TEMPERATURE) {
								ptrWidgetToUpdate->intValueTarget = ptrWidget->intValueTarget;
							} else {
								/* Target value is out of range */
								return ROOM_UPDATE_RESULT_ERROR;
							}
						}
						/* Update values in current RoomConfiguration */
						ptrWidgetToUpdate->boolValue = ptrWidget->boolValue;
						ptrWidgetToUpdate->status = ptrWidget->status;


						result = ROOM_UPDATE_RESULT_OK;

						/* Notify callbacks */
						RoomConfiguration_NotifyWidgetUpdated(ptrWidgetToUpdate, RC_WIDGET_UPDATED_FROM_REMOTE);
					}
					ptrWidget = ptrWidget->next;
				}
			}
			ptrRoom = ptrRoom->next;
		}
		/* Free allocated memory*/
		RoomWidget_FreeRoomList(roomConfigurationUpdate);


	}
	return result;
}

/**
 * @brief  Get array of pointers for a specific widget type.
 * 		   Note: It is up to a user to free memory allocated
 * 		   for a pointer array.
 * @param  widgetType: Type of widget
 * @retval Array of pointers or NULL if there are no widgets or if memory allocation failed
 */
struct RoomWidget ** RoomConfiguration_GetPointerArrayForWidgetType(const int widgetType) {
	if(!roomConfiguration) {
		return NULL;
	}
	int count = RoomConfiguration_GetWidgetCount(roomConfiguration, widgetType);

	if(count == 0) {
		return NULL;
	}

	struct RoomWidget **pp = calloc(count, sizeof(struct RoomWidget *));
	if(!pp) {
		/* Memory allocation failed */
		return NULL;
	}
	struct Room * headPointer = roomConfiguration->head;

	int positionInArray = 0;
	while (headPointer && (positionInArray < count)) {
		if (headPointer->widgets) {

			struct RoomWidget* ptr = headPointer->widgets->head;
			while (ptr) {
				if (ptr->type == widgetType) {
					pp[positionInArray] = ptr;
					positionInArray++;
				}
				ptr = ptr->next;
			}
		}

		headPointer = headPointer->next;
	}
	return pp;
}

/**
 * @brief  Get current instance of RoomConfiguration
 * @param  None
 * @retval Pointer to RoomList structure
 */
struct RoomList * RoomConfiguration_GetRoomConfiguration() {
	return roomConfiguration;
}

/**
 * @brief  Get count of thermostat widgets configured in configuration file
 * @param  None
 * @retval Count of widgets with a type of thermostat widget
 */
int RoomConfiguration_GetThermostatWidgetCount() {
	if(roomConfiguration) {
		return RoomConfiguration_GetWidgetCount(roomConfiguration, WIDGET_TYPE_THERMOSTAT);
	} else {
		return 0;
	}
}

/**
 * @brief  Get count of lights widgets configured in configuration file
 * @param  None
 * @retval Count of widgets with a type of light widget
 */
int RoomConfiguration_GetLightWidgetCount() {
	if(roomConfiguration) {
		return RoomConfiguration_GetWidgetCount(roomConfiguration, WIDGET_TYPE_LIGHT);
	} else {
		return 0;
	}
}

/**
 * @brief  Get count of specific widget type
 * @param  list: Pointer to RoomList where search widget type
 * @param  widgetType: Type of widget to get a count of
 * @retval Count of widgets
 */
int RoomConfiguration_GetWidgetCount(struct RoomList* list, const int widgetType) {
	if (!list || !(list->head)) {
		return 0;
	}

	struct Room * headPointer = list->head;
	int count = 0;

	while (headPointer) {
		if (headPointer->widgets) {

			struct RoomWidget* ptr = headPointer->widgets->head;
			while (ptr) {
				if(ptr->type == widgetType) {
					count++;
				}
				ptr = ptr->next;
			}
		}

		headPointer = headPointer->next;
	}
	return count;
}

/**
 * @brief  Get current RoomConfiguration in xml format
 * @param  None
 * @retval Pointer to char array or NULL
 */
char * RoomConfiguration_GetRoomConfiguartionInXmlFormat(void) {
	if(!roomConfiguration) {
		return NULL;
	}

	ezxml_t node = RoomWidgetWriter_WriteXML(roomConfiguration);
	if(node) {
		char * xml = ezxml_toxml(node);

		if(xml) {
			char * copy = calloc(strlen(xml) + 3, sizeof(char));
			if(copy) {
				sprintf(copy, "%c%s%c", SERIAL_START_FLAG, xml, SERIAL_END_FLAG);
				ezxml_free(node);
				free(xml);
				return copy;
			}
			free(xml);
		}
		ezxml_free(node);
	}
	return NULL;
}

/**
 * @brief  Set callback routine to be executed when the RoomConfiguartion is changed.
 * @param  c: Pointer to a function to be called when changes on RoomConfiguration
 * 		   is occured.
 * @retval None
 */
void RoomConfiguration_SetOnRoomConfigurationChangedCallback(OnRoomConfigurationChangedCallback c) {
	onRoomConfigurationChangedCallback = c;
}

/**
 * @brief  Set callback routine to be executed when the specified RoomWidget state is changed.
 * @param  c: Pointer to a function to be called when changes on RoomWidget state is occured
 * @retval None
 */
void RoomConfiguration_SetOnWidgetStateChangedCallback(OnWidgetStateChangedCallback c) {
	onWidgetStateChangedCallback = c;
}

/**
 * @brief  Notify UI and remote devices that one of the RoomWidgets has been updated
 * @param  widget: Pointer to the widget that has been updated
 * @retval None
 */
void RoomConfiguration_NotifyWidgetUpdated(struct RoomWidget * widget, int notificationCode) {
	/* Notify callbacks that RoomConfiguration was changed */
	if (onWidgetStateChangedCallback && widget) {
		onWidgetStateChangedCallback(widget, notificationCode);

		if(notificationCode != RC_WIDGET_UPDATED_FROM_REMOTE) {
			/* Avoid loops by sending incoming data back to remote device */
			RoomConfiguration_NotifyRemoteDeviceWidgetUpdated(widget);
		}

	}
	if (onRoomConfigurationChangedCallback) {
		onRoomConfigurationChangedCallback(ROOM_CONFIGURATION_UPDATED);
	}
}

/**
 * @brief  Notify remote devices that one of the RoomWidgets has been updated
 * @param  widget: Pointer to the widget that has been updated
 * @retval None
 */
void RoomConfiguration_NotifyRemoteDeviceWidgetUpdated(struct RoomWidget * widget) {
	if (!widget || !roomConfiguration) {
		return;
	}

	ezxml_t node = RoomWidgetWriter_GetRoomWidgetUpdateXML(roomConfiguration, widget);

	if (node) {
		char * xml = ezxml_toxml(node);
		if (xml) {
			char * copy = calloc(strlen(xml) + 2, sizeof(char));
			if (copy) {
				sprintf(copy, "%c%s%c", SERIAL_START_FLAG, xml,SERIAL_END_FLAG );
				SerialCommandWriterReader_TxWriteMessage(copy, strlen(copy));
				free(copy);
			}


		}
		ezxml_free(node);
		free(xml);
	}
}
