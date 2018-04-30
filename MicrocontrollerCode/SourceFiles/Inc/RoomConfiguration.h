#ifndef __ROOMCONFIGURATION_H
#define __ROOMCONFIGURATION_H

/* Includes */
/* FatFs includes component */
#include "ff_gen_drv.h"
#include "sd_diskio.h"

#include "RoomWidgetConstants.h"
#include "RoomWidget.h"
#include "RoomWidgetParser.h"
#include "RoomWidgetWriter.h"
#include "SerialCommandWriterReader.h"
#include "main.h"
#include <stdio.h>

/* Name of the configuration file to scan in SD card root path */
#define CONFIGURATION_FILE_NAME "conf.XML"
/* Result codes for RoomConfigurationUpdateRoomState() function */
#define ROOM_UPDATE_RESULT_OK			1
#define ROOM_UPDATE_RESULT_ERROR		0
/* Notification codes */
#define RC_WIDGET_UPDATED_FROM_USER		0 /* User has made changes from device UI */
#define RC_WIDGET_UPDATED_FROM_REMOTE   1 /* Changes has oqqured from remote device */
#define RC_WIDGET_UPDATED_FROM_DEVICE   2 /* Changes has oqqured from device internally i.g temperature reading changed */


/* OnRoomConfigurationChangedCallback codes */
typedef enum
{
	ROOM_CONFIGURATION_UPDATED,		/* Widget state changed */
	ROOM_CONFIGURATION_CREATED,		/* RoomConfiguration created first time */
	ROOM_CONFIGURATION_DELETED,		/* RoomConfiguration deleted */
}
ROOM_CONFIGURATION_EVENT;

// room_configuration_read_from_sd

uint8_t room_configuration_read_from_sd(APP_DATA* data);

// room_configuration_get_widgets

struct RoomWidget** room_configuration_get_widgets(APP_DATA* data,
		                                           const int widgetType);

// room_configuration_update_widgets

uint8_t room_configuration_update_widgets(APP_DATA*     data,
		                                  const ezxml_t a_node);

// room_configuration_to_xml

char * room_configuration_to_xml(APP_DATA* data);

// room_configuration_notify_widget_updated

void room_configuration_notify_widget_updated(APP_DATA*          data,
		                                               struct RoomWidget* widget);

// room_configuration_get_widget_count

uint16_t room_configuration_get_widget_count(struct RoomList* list,
		                                     const uint8_t    widgetType);

#endif
