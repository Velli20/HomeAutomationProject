#include "RoomWidgetParser.h"


/* LINKED_LIST declared in main.c */
extern struct LINKED_LIST UartCommands;


/**
 * @brief  Parse room configuration xml file that contains all
 * 		   the parameters related to home automation system.
 *
 * 		   The xml file must start with <Data> tag and end with </Data>
 * 		   tag.
 * @param  xmlString: Xml file in string format to parse
 * @param  lenght: Character count of the string to be parsed
 * @retval Pointer to RoomList struct that contains
 * 		   all parsed parameters
 */
struct RoomList* RoomWidgetParser_ParseString(char * xmlString, size_t lenght) {
	struct RoomList *list = NULL;
	if(!xmlString || lenght == 0) {
		return NULL;
	}

	ezxml_t node = ezxml_parse_str(xmlString, lenght);

	if(node) {
		list = RoomWidgetParser_ParseData(node);
		ezxml_free(node);
	}

	return list;
}

/**
 * @brief  Search for the Room data that is wrapped with <Room></Room> tags
 * 		   and parse it.
 * @param  a_node: Pointer to the xml node struct to parse
 * @param  list: Pointer to the RoomList struct to link the room data
 * @retval None
 */
struct RoomList* RoomWidgetParser_ParseData(const ezxml_t a_node) {
	if(!a_node) {
		return NULL;
	}
	/* Search for <Room></Room> tags */
	ezxml_t roomNode = ezxml_child(a_node, ELEMENT_ROOM);
	if(!roomNode) {
		/* No room tags in xml file */
		return NULL;
	}

	struct RoomList *list = calloc(1, sizeof(struct RoomList));
	if(!list) {
		/* Memory allocation failed */
		return NULL;
	}
	/* Iterate trough <Room></Room> tags */
	while (roomNode) {
		/* Parse the data that is wrapped inside tags*/
		struct Room * room = RoomWidgetParser_ParseRoom(roomNode);
		if (room) {
			RoomWidget_AddToRoomList(list, room);
		}

		/* After the data is parsed move to another tags */
		roomNode = roomNode->next;
	}
	return list;
}

/**
 * @brief  Search for the widget data that is wrapped with <RoomWidget></RoomWidget> tags
 * 		   and parse it.
 * @param  roomNode: Pointer to the xml node wrapped with <Room></Room> tags
 * @retval Room struct
 */
struct Room* RoomWidgetParser_ParseRoom(const ezxml_t roomNode) {
	if(!roomNode) {
		return NULL;
	}
	struct Room *room = calloc(1, sizeof(struct Room));
	if (!room) {
		/* Memory allocation failed */
		return NULL;
	}

	room->widgets = calloc(1, sizeof(struct RoomWidgetList));
	if(!(room->widgets)) {
		return NULL;
	}

	const char *roomName = ezxml_attr(roomNode, ATTRIBUTE_NAME);
	const char *roomId = ezxml_attr(roomNode, ATTRIBUTE_ID);

	ezxml_t widgetNode = ezxml_child(roomNode, ELEMENT_WIDGET);

	while (widgetNode) {
		struct RoomWidget * widget = RoomWidgetParser_ParseWidget(widgetNode);

		if (widget && room) {
			RoomWidget_AddWidgetToRoom(room, widget);
		}
		widgetNode = widgetNode->next;
	}

	if (roomId) {
		room->id = strtol(roomId, NULL, 0);
	}

	if (roomName) {
		char * roomNameCpy = calloc(strlen(roomName), sizeof(char));
		strcpy(roomNameCpy, roomName);
		room->name = roomNameCpy;
	}


	return room;
}

/**
 * @brief  Parse widget data that is wrapped with <RoomWidget></RoomWidget> tags.
 * @param  widgetNode: Pointer to xml node wrapped with <RoomWidget></RoomWidget> tags
 * @retval RoomWidget struct
 */
struct RoomWidget* RoomWidgetParser_ParseWidget(const ezxml_t widgetNode) {
	if(!widgetNode) {
		return NULL;
	}
	struct RoomWidget *widget = calloc(1, sizeof(struct RoomWidget));

	if(!widget) {
		/* Memory allocation failed */
		return NULL;
	}

	if (ezxml_child(widgetNode, ELEMENT_ROOM_WIDGET_ID)) {
		const char * id = ezxml_child(widgetNode, ELEMENT_ROOM_WIDGET_ID)->txt;
		if (id) {
			int idCpy = strtol(id, NULL, 0);
			widget->id = idCpy;
		}
	}
	if (ezxml_child(widgetNode, ELEMENT_ROOM_WIDGET_TYPE)) {
		const char * type = ezxml_child(widgetNode, ELEMENT_ROOM_WIDGET_TYPE)->txt;
		if (type) {
			int typeCpy = strtol(type, NULL, 0);
			widget->type = typeCpy;
		}
	}
	if (ezxml_child(widgetNode, ELEMENT_ROOM_WIDGET_NAME)) {
		const char * name = ezxml_child(widgetNode, ELEMENT_ROOM_WIDGET_NAME)->txt;
		if (name) {
			size_t lenght = strlen(name);
			if (lenght > 0) {
				char * nameCpy = calloc(lenght, sizeof(char));
				strncpy(nameCpy, name, lenght);
				widget->name = nameCpy;
			}
		}
	}
	if (ezxml_child(widgetNode, ELEMENT_ROOM_WIDGET_INT_VALUE)) {
		const char * intValue = ezxml_child(widgetNode, ELEMENT_ROOM_WIDGET_INT_VALUE)->txt;
		if (intValue) {
			int intValueCpy = strtol(intValue, NULL, 0);
			widget->intValue = intValueCpy;
		}
	}
	if (ezxml_child(widgetNode, ELEMENT_ROOM_WIDGET_BOOL_VALUE)) {
		const char * boolValue = ezxml_child(widgetNode, ELEMENT_ROOM_WIDGET_BOOL_VALUE)->txt;
		if (boolValue) {
			widget->boolValue = strcmp(boolValue, "true") == 0 ? 1 : 0;
		}

	}



	return widget;
}
