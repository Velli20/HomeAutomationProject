// Includes

#include"SerialCommandParser.h"

// SerialCommandParser_ParseCommand

SERIAL_COMMAND SerialCommandParser_ParseCommand(ezxml_t node) 
{

	SERIAL_COMMAND result;
	
	result= SERIAL_COMMAND_UNKNOWN_COMMAND;

	if ( !node || !(node->name) )
	    return(SERIAL_COMMAND_UNKNOWN_COMMAND);


	if ( !strcmp(node->name, TAG_MESSAGE_GET_ROOM_GONFIGURATION) ) 
		result = SERIAL_COMMAND_GET_ROOM_COFIGURATION;
	else if( !strcmp(node->name, TAG_MESSAGE_UPDATE_WIDGET_STATUS) ) 
		result = SERIAL_COMMAND_UPDATE_WIDGET_STATUS;
	else 
		result= SERIAL_COMMAND_UNKNOWN_COMMAND;

	return(result);
}
