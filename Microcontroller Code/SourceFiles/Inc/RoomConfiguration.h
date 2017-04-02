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

#include <stdio.h>

/* Name of the configuration file to scan in SD card root path */
#define CONFIGURATION_FILE_NAME "conf.XML"
/* Result codes for RoomConfigurationUpdateRoomState() function */
#define ROOM_UPDATE_RESULT_OK		1
#define ROOM_UPDATE_RESULT_ERROR	0

typedef enum {
	ROOM_CONFIGURATION_RESULT_NO_SD,
	ROOM_CONFIGURATION_RESULT_NO_FILE,
	ROOM_CONFIGURATION_RESULT_CONFIGURATION_FILE_EMPTY,
	ROOM_CONFIGURATION_RESULT_UNABLE_TO_MOUNT_SD,
	ROOM_CONFIGURATION_RESULT_UNABLE_TO_READ_FILE,
	ROOM_CONFIGURATION_RESULT_UNABLE_TO_LINK_SD_DRIVER,
	ROOM_CONFIGURATION_RESULT_PARSING_FAILED,
	ROOM_CONFIGURATION_RESULT_NO_MEMORY,
	ROOM_CONFIGURATION_RESULT_OK

} ROOM_CONFIGURATION_READ_RESULT;

/* OnRoomConfigurationChangedCallback codes */
typedef enum {
	ROOM_CONFIGURATION_UPDATED,		/* Widget state changed */
	ROOM_CONFIGURATION_CREATED,		/* RoomConfiguration created first time */
	ROOM_CONFIGURATION_DELETED,		/* RoomConfiguration deleted */
} ROOM_CONFIGURATION_EVENT;

/* Callback used to notify client if RoomConfiguration is changed */
typedef void (*OnRoomConfigurationChangedCallback)(ROOM_CONFIGURATION_EVENT event);

/* Callback used to notify client if RoomWidget state is changed */
typedef void (*OnWidgetStateChangedCallback)(struct RoomWidget * widget);


/* Public function prototypes */
ROOM_CONFIGURATION_READ_RESULT RoomConfiguration_ReadFromSdCard(void);

struct RoomWidget ** RoomConfiguration_GetPointerArrayForWidgetType(const int widgetType);

int RoomConfiguration_UpdateRoomState(const ezxml_t a_node);
int RoomConfiguration_GetWidgetCount(struct RoomList* list, const int widgetType);
int RoomConfiguration_GetThermostatWidgetCount(void);
int RoomConfiguration_GetLightWidgetCount(void);

char * RoomConfiguration_GetRoomConfiguartionInXmlFormat(void);

void RoomConfiguration_SetOnRoomConfigurationChangedCallback(OnRoomConfigurationChangedCallback c);
void RoomConfiguration_SetOnWidgetStateChangedCallback(OnWidgetStateChangedCallback c);

#endif /* __ROOMCONFIGURATION_H */



/* END OF FILE */
