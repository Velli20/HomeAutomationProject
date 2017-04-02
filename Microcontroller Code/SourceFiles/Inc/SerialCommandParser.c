/* Includes */
#include"SerialCommandParser.h"





/**
  * @brief  Parse incoming serial command
  * @param  node: XML node to parse
  * @retval SERIAL_COMMAND
  */
SERIAL_COMMAND SerialCommandParser_ParseCommand(ezxml_t node) {

	SERIAL_COMMAND result = SERIAL_COMMAND_UNKNOWN_COMMAND;


	if(node && (node->name)) {
		if (strcmp(node->name, TAG_MESSAGE_GET_ROOM_GONFIGURATION) == 0) {
			result = SERIAL_COMMAND_GET_ROOM_COFIGURATION;
		} else if(strcmp(node->name, TAG_MESSAGE_UPDATE_WIDGET_STATUS) == 0) {
			result = SERIAL_COMMAND_UPDATE_WIDGET_STATUS;
		}
	}


	return result;
}
