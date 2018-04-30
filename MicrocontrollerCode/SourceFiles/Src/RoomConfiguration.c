
// Includes

#include <stdio.h>
#include <string.h>

#include "RoomConfiguration.h"
#include "UI_resources.h"
#include "main.h"

#include "ff.h"
#include "ff_gen_drv.h"
#include "sd_diskio.h"

// room_configuration_read_from_sd
// Reads and parser configuration file on SD-card to
// room widget objects.

uint8_t room_configuration_read_from_sd(APP_DATA* data)
{
	FATFS    sd_fat_fs;
	FIL      room_configuration_file;
	uint32_t bytesread;
	uint8_t  result;
	size_t   file_size;
	char*    file_string;
	char     sd_path[4];
	FRESULT  read;

	// Check application data pointer.

	if ( !data )
		return(RESULT_ERROR);

	// Clear old widget data.

	if ( data->room_configuration )
	{
		widget_list_free(data->room_configuration);
		data->room_configuration= 0;

		// Notify callback that configuartion was deleted.

		if ( data->event_callback )
		    event_handler_send_event(data, 0, EVENT_ROOM_CONFIGURATION_DELETED);
	}

	// Check if uSd card is mounted.

	if ( BSP_SD_IsDetected() != SD_PRESENT )
		return(RESULT_ERROR);

	// Link the micro SD disk I/O driver.

	memset(sd_path, 0, sizeof(sd_path));
	if ( FATFS_LinkDriver(&SD_Driver, sd_path) != FR_OK )
		 return(RESULT_ERROR);

	result= RESULT_ERROR;
	file_string= 0;

	//  Register the file system object to the FatFs module.

	if ( f_mount(&sd_fat_fs, (const TCHAR*)sd_path, 0) != FR_OK )
		goto end;

	// Open the configuration file object with read access.

	if ( f_open(&room_configuration_file, CONFIGURATION_FILE_NAME, FA_READ) != FR_OK )
		goto end;
		
	// Get byte count of the configuration file.

	file_size= f_size(&room_configuration_file);
	if ( file_size == 0 )
		goto end;

	// Allocate memory for the configuration string.

	file_string= calloc(file_size, sizeof(char));
	if ( !file_string )
		goto end;

	// If memory allocation was successful Read data from the xml file.

	read= f_read(&room_configuration_file, file_string, file_size, (UINT*) &bytesread);
	if ( read != FR_OK || bytesread < file_size )
		goto end;

	// Null terminate file string.

	file_string[file_size-1] = '\0';
	
	// Parse the room configuration file.

	data->room_configuration= widget_parser_parse_xml_string(file_string, bytesread);
	if ( !data->room_configuration )
		goto end;

	// Notify callback that configuration was created.

	if ( data->event_callback )
	    event_handler_send_event(data, 0, EVENT_ROOM_CONFIGURATION_CREATED);

	result= RESULT_OK;

	end:

	// Unlink the micro SD disk I/O driver and close file.

	f_close(&room_configuration_file);
	
	FATFS_UnLinkDriver(sd_path);

	if ( file_string )
		free(file_string);

	return(result);
}

// room_configuration_update_widgets
// Parsers XML node to room widget objects and tries to search
// update excisting room widgets based on new widgets values/ids

uint8_t room_configuration_update_widgets(APP_DATA*     data,
		                                  const ezxml_t a_node)
{
	struct RoomList*   update_widgets;
	struct RoomWidget* widget_to_update;
	struct RoomWidget* widget_ptr;
	struct Room*       room_ptr;
	int                result;

	if( !a_node || !data || !data->room_configuration )
		return(RESULT_ERROR);

	// Try to parse data in XML node.

	update_widgets= widget_parser_parse_xml_node(a_node);
	if ( !update_widgets )
		return(RESULT_ERROR);

	room_ptr= update_widgets->head;
	if ( !room_ptr )
		return(RESULT_OK);

	result= ROOM_UPDATE_RESULT_ERROR;

	// Iterate trough RoomWidgets in update_widgets list.

	while ( room_ptr )
	{

		if ( !room_ptr->widgets || !room_ptr->widgets->head )
		{
			room_ptr= room_ptr->next;
			continue;
		}

		widget_ptr= room_ptr->widgets->head;
		while ( widget_ptr )
		{
			// Try to get a widget that matches id.

			widget_to_update= widget_list_find_widget_with_id(data->room_configuration, widget_ptr->id);
			if ( !widget_to_update )
			{
				widget_ptr= widget_ptr->next;
				continue;
			}

			if ( widget_to_update->type != WIDGET_TYPE_THERMOSTAT )
			{
				// Avoid overwriting read only values.

				widget_to_update->intValue= widget_ptr->intValue;
				if ( widget_ptr->intValueTarget < MAX_THERMOSTAT_TEMPERATURE && widget_ptr->intValueTarget > MIN_THERMOSTAT_TEMPERATURE )
					widget_to_update->intValueTarget= widget_ptr->intValueTarget;

				// Target value is out of range.

				else
					return(RESULT_ERROR);
			}

			// Update values in current RoomConfiguration.

			widget_to_update->boolValue= widget_ptr->boolValue;
			widget_to_update->status=    widget_ptr->status;

			result= RESULT_OK;

			// Notify callbacks.

			event_handler_send_event(data, widget_to_update, EVENT_ROOM_WIDGET_UPDATED_FROM_REMOTE);

			widget_ptr= widget_ptr->next;
		}

		room_ptr= room_ptr->next;
	}

	// Free allocated memory.

	widget_list_free(update_widgets);

	return(result);
}

// room_configuration_get_widget_count

uint16_t room_configuration_get_widget_count(struct RoomList* list,
		                                     const uint8_t    widgetType)
{
	struct Room*       room_ptr;
	struct RoomWidget* ptr;
	uint16_t           count;

	if ( !list || !(list->head) )
		return 0;

	room_ptr= list->head;
	count= 0;

	while ( room_ptr )
	{
		if ( !room_ptr->widgets )
		    goto next;
			
		ptr = room_ptr->widgets->head;
		while (ptr) 
		{
			if ( ptr->type == widgetType )
				count++;
		
			ptr= ptr->next;
		}
		
		next:

		room_ptr= room_ptr->next;
	}

	return(count);
}

// room_configuration_to_xml

char* room_configuration_to_xml(APP_DATA* data)
{
	char*   xml;
	char*   copy;
	ezxml_t node;

	if ( !data || !data->room_configuration )
		return(0);

	node= RoomWidgetWriter_WriteXML(data->room_configuration);
	if ( node )
	{
		xml= ezxml_toxml(node);
		if ( xml )
		{
			copy= calloc(strlen(xml) + 3, sizeof(char));
			if ( copy )
			{
				sprintf(copy, "%c%s%c", SERIAL_START_FLAG, xml, SERIAL_END_FLAG);
				ezxml_free(node);
				free(xml);
				return copy;
			}
			free(xml);
		}
		ezxml_free(node);
	}
	return(0);
}

// room_configuration_notify_widget_updated
// Sends message to connected devices that given widget is updated.

void room_configuration_notify_widget_updated(APP_DATA*          data,
		                                      struct RoomWidget* widget)
{
	ezxml_t node;
	char*   xml;
	char*   copy;

	if ( !widget || !data || !data->room_configuration )
		return;

	node= RoomWidgetWriter_GetRoomWidgetUpdateXML(data->room_configuration, widget);
	if ( node )
	{
		xml= ezxml_toxml(node);
		if ( xml )
		{
			copy= calloc(strlen(xml) + 2, sizeof(char));
			if ( copy )
			{
				sprintf(copy, "%c%s%c", SERIAL_START_FLAG, xml,SERIAL_END_FLAG );
				SerialCommandWriterReader_TxWriteMessage(copy, strlen(copy));
				free(copy);
			}
		}
		ezxml_free(node);
		free(xml);
	}
}

// room_configuration_get_widgets

struct RoomWidget** room_configuration_get_widgets(APP_DATA* data,
		                                           const int widgetType)
{
	struct RoomWidget** pp;
	struct RoomWidget*  ptr;
	struct Room*        room_ptr;
	int                 count;
	int                 position;

	if ( !data || !data->room_configuration )
		return(0);

	count= room_configuration_get_widget_count(data->room_configuration, widgetType);
	if ( count == 0 )
		return(0);

	room_ptr= data->room_configuration->head;
	if ( !room_ptr )
		return(0);

	pp= calloc(count, sizeof(struct RoomWidget*));
	if ( !pp )
		return(0);

	position= 0;
	while ( room_ptr && (position < count) )
	{
		if ( !room_ptr->widgets )
		{
			room_ptr= room_ptr->next;
			continue;
		}

		ptr= room_ptr->widgets->head;
		while (ptr)
		{
			if ( ptr->type == widgetType )
			{
				pp[position]= ptr;
				position++;
			}
			ptr= ptr->next;
		}

		room_ptr= room_ptr->next;
	}
	return pp;
}
