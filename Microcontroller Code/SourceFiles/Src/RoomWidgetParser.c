#include "RoomWidgetParser.h"
#include "RoomWidget.h"
#include "RoomWidgetConstants.h"


struct ROOMLIST* RoomWidgetParser_ParseString(char * xmlString, size_t lenght) {
	//printf("WidgetParser_parseDocument url: %s \n", url);
	struct ROOMLIST *list = malloc(sizeof(struct ROOMLIST));
	if(!list) {
		return NULL;
	}

	list->current = NULL;
	list->head = NULL;

	ezxml_t node = ezxml_parse_str(xmlString, lenght);

	if(node != NULL) {
		RoomWidgetParser_ParseData(node, list);
	}
	ezxml_free(node);
	return list;
}

void RoomWidgetParser_ParseData(ezxml_t a_node, struct ROOMLIST *list) {
	if(list == NULL) {
		list = malloc(sizeof(struct ROOMLIST));
	}

	ezxml_t roomNode = NULL;


    for (roomNode = ezxml_child(a_node, ELEMENT_ROOM); roomNode; roomNode = roomNode->next) {
    	struct ROOM * room = RoomWidgetParser_ParseRoom(roomNode);
    	if(room != NULL) {
    		RoomWidget_AddToRoomList(list, room);
    	}
    }


}


struct ROOM* RoomWidgetParser_ParseRoom(ezxml_t roomNode) {
	ezxml_t widgetNode = NULL;
	struct ROOM *room = malloc(sizeof(struct ROOM));


	if (roomNode) {
		const char *roomName = ezxml_attr(roomNode, ATTRIBUTE_NAME);
		int roomId = strtol((char*) ezxml_attr(roomNode, ATTRIBUTE_ID), NULL, 0);


		/*for (widgetNode = ezxml_child(roomNode, ELEMENT_WIDGET); widgetNode; widgetNode = widgetNode->next) {
			struct RoomWidget * widget = RoomWidgetParser_ParseWidget(widgetNode);

			if(widget && room) {
				RoomWidget_AddWidgetToRoom(room, widget);
			}

		}*/
		if(room) {
			room->id = roomId;

			/* roomName != null */
			if(roomName) {
				size_t lenght = strlen(roomName);
				if (lenght != 0) {
					char * roomNameCpy = malloc(sizeof(char) * lenght);
					strcpy(roomNameCpy, roomName);
					room->name = roomNameCpy;
				}
			}
		}
	}
	return room;
}


struct RoomWidget* RoomWidgetParser_ParseWidget(ezxml_t widgetNode) {
	struct RoomWidget *widget = malloc(sizeof(struct RoomWidget));

	if(!widget) {
		return NULL;
	}

	if (widgetNode != NULL) {
		if(ezxml_child(widgetNode, ELEMENT_ROOM_WIDGET_ID)) {
			char * id = ezxml_child(widgetNode, ELEMENT_ROOM_WIDGET_ID)->txt;
			/* id != null */
			if(id) {
				int idCpy = strtol(id, NULL, 0);
				widget->id = idCpy;
			}
		}
		if (ezxml_child(widgetNode, ELEMENT_ROOM_WIDGET_TYPE)) {
			char * type = ezxml_child(widgetNode, ELEMENT_ROOM_WIDGET_TYPE)->txt;
			if (type) {
				int typeCpy = strtol(type, NULL, 0);
				widget->type = typeCpy;
			}
		}
		if(ezxml_child(widgetNode, ELEMENT_ROOM_WIDGET_NAME)) {
			char * name = ezxml_child(widgetNode, ELEMENT_ROOM_WIDGET_NAME)->txt;
			if (name) {
				size_t lenght = strlen(name);
				char * nameCpy = malloc(sizeof(char) * lenght);
				strncpy(nameCpy, name, lenght);
				widget->name = nameCpy;
			}
		}
		if(ezxml_child(widgetNode, ELEMENT_ROOM_WIDGET_INT_VALUE)) {
			char * intValue = ezxml_child(widgetNode, ELEMENT_ROOM_WIDGET_INT_VALUE)->txt;
			if (intValue) {
				int intValueCpy = strtol(intValue, NULL, 0);
				widget->intValue = intValueCpy;
			}
		}
		if(ezxml_child(widgetNode, ELEMENT_ROOM_WIDGET_BOOL_VALUE)) {
			if(ezxml_child(widgetNode, ELEMENT_ROOM_WIDGET_BOOL_VALUE)->txt) {
				int boolValue = strcmp(ezxml_child(widgetNode, ELEMENT_ROOM_WIDGET_BOOL_VALUE)->txt, "true") == 0?
						1 : 0;
				widget->boolValue = boolValue;
			}

		}
		if(ezxml_child(widgetNode, ELEMENT_ROOM_WIDGET_STATUS) != NULL) {
			char * status = ezxml_child(widgetNode, ELEMENT_ROOM_WIDGET_STATUS)->txt;
			if (status != NULL) {
				int statusCpy = strtol(status, NULL, 0);
				widget->status = statusCpy;
			}
		}
	}
	return widget;
}
