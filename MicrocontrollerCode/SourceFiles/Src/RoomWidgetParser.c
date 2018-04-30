// Includes

#include <stdio.h>
#include <string.h>

#include "RoomWidgetParser.h"

// widget_parser_string_to_int

static inline void widget_parser_string_to_int(const char* value, 
                                               uint32_t*   target)
{
	char* end_ptr;

	if ( !value || !*value || !target ) 
	{
		*target= 0;
		return;
	}

	// Try to parse long from string .

	end_ptr= 0;
	*target= (uint32_t)strtol(value, &end_ptr, 0);

	if ( end_ptr == value || *target == UINT32_MAX ) 
	    *target= 0;
}

// widget_parser_get_int_value

static inline void widget_parser_get_int_value(const ezxml_t node,
                                               uint32_t*     target)
{
	if ( !node || !node->txt || !*node->txt )
	{
		*target= 0;
		return;
	}
	
	widget_parser_string_to_int(node->txt, target);
}	

// widget_parser_copy_string_value

static void widget_parser_copy_string_value(const char* valueToCopy, 
                                            char**      target) 
{
	size_t length;

	if ( !valueToCopy || !*valueToCopy || !target ) 
		return;
	
	length= strlen(valueToCopy);
	if ( length == 0 ) 
		return;
	
	*target= malloc((length+1) * sizeof(char));
	if ( !*target )
	    return;
		
	strncpy(*target, valueToCopy, (length+1));
}

//widget_parser_get_string_value

static inline void widget_parser_get_string_value(const ezxml_t node, 
                                                  char**        target) 
{
	if ( !node || !node->txt || !*node->txt ) 
		return;
	
	widget_parser_copy_string_value(node->txt, target);
}

// widget_parser_get_bool_value

static inline void widget_parser_get_bool_value(const ezxml_t node,
                                                uint8_t*      target) 
{
	if ( !node || !node->txt || !*node->txt ) 
		return;
	
	*target= !strncmp(node->txt, "true", 6);
}									

// widget_parser_parse_widget

static struct RoomWidget* widget_parser_parse_widget(const ezxml_t widgetNode)
{
	struct RoomWidget* widget;

	if ( !widgetNode ) 
		return(0);
	
	widget= calloc(1, sizeof(struct RoomWidget));
	if ( !widget ) 
		return(0);

	// Try to parse <id> tags.
	
	widget_parser_get_int_value(ezxml_child(widgetNode, XML_TAG_ROOM_WIDGET_ID), &(widget->id));

	// Try to parse <type> tags.

	widget_parser_get_int_value(ezxml_child(widgetNode, XML_TAG_ROOM_WIDGET_TYPE), &(widget->type));

	// Try to parse <name> tags.

	widget_parser_get_string_value(ezxml_child(widgetNode, XML_TAG_ROOM_WIDGET_NAME), &(widget->name));

	// Try to parse <intValue> tags.

	widget_parser_get_int_value(ezxml_child(widgetNode, XML_TAG_ROOM_WIDGET_INT_VALUE), &(widget->intValue));

	// Try to parse <intTargetValue> tags.

	widget_parser_get_int_value(ezxml_child(widgetNode, XML_TAG_ROOM_WIDGET_INT_TARGET_VALUE), &(widget->intValueTarget));

	// Try to parse <boolValue> tags.

	widget_parser_get_bool_value(ezxml_child(widgetNode, XML_TAG_ROOM_WIDGET_BOOL_VALUE), &(widget->boolValue));

	// Try to parse <pin> tags.

	widget_parser_get_int_value(ezxml_child(widgetNode, XML_TAG_ROOM_WIDGET_PIN), &(widget->arduinoPin));

	// Try to parse <status> tags.

	widget_parser_get_int_value(ezxml_child(widgetNode, XML_TAG_ROOM_WIDGET_STATUS), &(widget->status));

	return(widget);
}

// widget_parser_parse_room

static struct Room* widget_parser_parse_room(const ezxml_t roomNode)
{
	struct Room*       room;
	struct RoomWidget* widget;
	ezxml_t            widgetNode;

	if ( !roomNode ) 
		return(0);
	
	// Allocate memory for the room struct and widget list.

	room= calloc(1, sizeof(struct Room));
	if ( !room ) 
		return(0);

	room->widgets= calloc(1, sizeof(struct RoomWidgetList));
	if ( !room->widgets ) 
	{
		free(room);
		return(0);
	}

	// Search for <RoomWidget> tag.

	widgetNode= ezxml_child(roomNode, XML_TAG_ROOM_WIDGET);
	while ( widgetNode ) 
	{
		// Parse room widget.

		widget= widget_parser_parse_widget(widgetNode);
		if ( widget ) 
			widget_list_add_widget_to_room(room, widget);
		
		widgetNode= widgetNode->next;
	}

	// Parse widget name.

	widget_parser_copy_string_value(ezxml_attr(roomNode, XML_ATTRIBUTE_ROOM_NAME), &(room->name));
	
	// Parse room id.

	widget_parser_string_to_int(ezxml_attr(roomNode, XML_ATTRIBUTE_ROOM_ID), &room->id);

	return(room);
}

// widget_parser_parse_xml_string

struct RoomList* widget_parser_parse_xml_string(char*  xmlString, 
                                                size_t length) 
{
	struct RoomList* list;
	ezxml_t          node;
	
	if ( !xmlString || !*xmlString || length == 0 ) 
		return(0);
	
	node= ezxml_parse_str(xmlString, length);
	if ( !node )
	    return(0);

	list= widget_parser_parse_xml_node(node);
	ezxml_free(node);
	
	return(list);
}

// widget_parser_parse_xml_node
// Searches for the widget data that is wrapped with <Room></Room> tags
// and parses it.

struct RoomList* widget_parser_parse_xml_node(const ezxml_t a_node) 
{
	struct RoomList* list;
	struct Room*     room;
	ezxml_t          roomNode;

	if ( !a_node) 
		return(0);
	
	// Search for <Room></Room> tags.

	roomNode= ezxml_child(a_node, XML_TAG_ROOM);
	if ( !roomNode ) 
	    return(0);

	list= calloc(1, sizeof(struct RoomList));
	if ( !list ) 
		return(0);

	// Iterate trough <Room></Room> tags.
	
	while ( roomNode ) 
	{
		// Parse data.

		room= widget_parser_parse_room(roomNode);
		if ( room ) 
			widget_list_add_to_room_list(list, room);

		// Move to next node.

		roomNode= roomNode->next;
	}

	return(list);
}

// widget_parser_parse_request_code

uint32_t widget_parser_parse_request_code(const ezxml_t roomConfiguration) 
{
	uint32_t value;

	if ( !roomConfiguration ) 
		return(0);
	
	widget_parser_string_to_int(ezxml_attr(roomConfiguration, XML_ATTRIBUTE_REQUEST_CODE), &value);

	return(value);
}

