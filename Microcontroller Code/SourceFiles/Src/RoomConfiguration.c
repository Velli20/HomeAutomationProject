#include "RoomConfiguration.h"

/* Private variables */
FATFS SDFatFs;              /* File system object for SD card logical drive */
FIL RoomConfigurationFile;  /* File object */
char SDPath[4];             /* SD card logical drive path */

/* LINKED_LIST declared in main.c */
extern struct LINKED_LIST UartCommands;
extern char * roomConfigurationString;

/* ROOMLIST is where room configurations is stored */
struct ROOMLIST* list;


/**
 * @brief  Read configuration file (conf.XML) from µSD card.
 * 		   This is the file where all widgets is configured.
 * @param  None
 * @retval None
 */
FRESULT RoomConfiguration_ReadFromSdCard(void) {
	if (BSP_SD_IsDetected() != SD_PRESENT) {
		LinkedList_add("RoomConfiguration_ReadFromSdCard Error: no uSD card inserted", true, &UartCommands);
		return FR_NOT_READY;
	}

	FRESULT res = FR_NO_PATH;
	FILINFO fno;
	char * configFileString;
	uint32_t bytesread;



	/* Link the micro SD disk I/O driver */
	if (FATFS_LinkDriver(&SD_Driver, SDPath) == 0) {
		/* Register the file system object to the FatFs module */
		res = f_mount(&SDFatFs, SDPath, 0);
		if (res != FR_OK) {
			LinkedList_add("RoomConfiguration_ReadFromSdCard unable to mount the sd card", true, &UartCommands);
			goto end;
		}

		/* Open the configuration file object with read access */
		res = f_open(&RoomConfigurationFile, "conf.XML", FA_READ);
		if (res != FR_OK) {
			LinkedList_add("RoomConfiguration_ReadFromSdCard no config file!", true, &UartCommands);
			goto end;
		}

		if(roomConfigurationString) {
			free(roomConfigurationString);
		}

		/* Allocate memory for the cofig string */
		configFileString = malloc(f_size(&RoomConfigurationFile) * sizeof(char));


		/* Read data from the xml file */
		res = f_read(&RoomConfigurationFile, configFileString, f_size(&RoomConfigurationFile), (UINT*) &bytesread);
		if (res != FR_OK) {
			LinkedList_add("RoomConfiguration_ReadFromSdCard can't read config file!", true, &UartCommands);
		}

		roomConfigurationString = malloc(f_size(&RoomConfigurationFile) * sizeof(char));
		strcpy(roomConfigurationString, configFileString);

		//list = RoomWidgetParser_ParseString(configFileString, strlen(configFileString));



		f_close(&RoomConfigurationFile);
	}

	end:

	FATFS_UnLinkDriver(SDPath);
	if(configFileString != NULL) {
		free(configFileString);
	}
	return res;
}
