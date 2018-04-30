#ifndef __SERIAL_COMMAND_PARSER_H
#define __SERIAL_COMMAND_PARSER_H

/* Includes */
#include "ezxml.h"
#include <string.h>

/* XML tags used to represent commands */
#define TAG_MESSAGE_GET_ROOM_GONFIGURATION 		"GetRoomConfiguration"
#define TAG_MESSAGE_UPDATE_WIDGET_STATUS		"RoomConfigurationUpdate"



typedef enum {
	SERIAL_COMMAND_GET_ROOM_COFIGURATION,
	SERIAL_COMMAND_UPDATE_WIDGET_STATUS,
	SERIAL_COMMAND_UNKNOWN_COMMAND
} SERIAL_COMMAND;


/* Public function prototypes */
SERIAL_COMMAND SerialCommandParser_ParseCommand(ezxml_t node);

#endif /* __SERIAL_COMMAND_PARSER_H */

/* End of file */
