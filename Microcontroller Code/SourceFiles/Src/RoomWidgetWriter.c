/* Includes */
#include "RoomWidgetWriter.h"

/* Private function prototypes */
static ezxml_t RoomWidgetWriter_WriteRoom(struct Room * room, ezxml_t root);

/**
 * @brief  Creates ezxml structure out of RoomConfiguration
 * @param  list: Pointer to the RoomConfiguration
 * @retval ezxml_t structure or NULL if operation was unsuccessful
 */
ezxml_t RoomWidgetWriter_WriteXML(struct RoomList * list) {
	if(!list || !(list->head)) {
		return NULL;
	}
	/* Create root tag <Data><Data/> */
	ezxml_t root = ezxml_new(TAG_DATA);

	struct Room* ptr = list->head;

	while (ptr) {
		RoomWidgetWriter_WriteRoom(ptr, root);
		ptr = ptr->next;

	}

	return root;
}



static ezxml_t RoomWidgetWriter_WriteRoom(struct Room * room, ezxml_t root) {
	if(!room || !(room->widgets ) || !(room->widgets->head)) {
		return NULL;
	}

	/* Create Room tag */
	/* <Room name="" id=""> */
	ezxml_t roomTAG  = ezxml_new(ELEMENT_ROOM);

	/* Pass room name and id as an attribute */
	if(room->name) {
		roomTAG = ezxml_set_attr(roomTAG, ATTRIBUTE_NAME, room->name);
	}

	char * roomId = calloc(4, sizeof(char));

	if(roomId) {
		sprintf(roomId, "%d", room->id);
		roomTAG = ezxml_set_attr(roomTAG, ATTRIBUTE_ID, roomId);
	}


	/* Insert Room tag inside Data tag */
	ezxml_insert(roomTAG, root, 0);
	struct RoomWidget* ptr = room->widgets->head;


	int widgetPosInXml = 0;
	while (ptr) {

		/* Create RoomWidget tags */
		/* <RoomWidget></RoomWidget> */
		ezxml_t widget = ezxml_new(ELEMENT_WIDGET);
		if(!widget) {
			ptr = ptr->next;
			continue;
		}

		/* Create ID tag and insert RoomWidget Id as a text  */
		/* <id type="text">value</id> */
		ezxml_t widgetId  = ezxml_new(ELEMENT_ROOM_WIDGET_ID);
		char * widgetIdtext = calloc(4, sizeof(char));
		if (widgetIdtext) {
			sprintf(widgetIdtext, "%d", ptr->id);
			widgetId = ezxml_set_txt(widgetId, widgetIdtext);
			ezxml_insert(widgetId, widget, 0);
		}
		/* Create ID tag and insert RoomWidget type as a text  */
		/* <type type="text">value</type> */
		ezxml_t widgetType  = ezxml_new(ELEMENT_ROOM_WIDGET_TYPE);
		char * widgetTypetext = calloc(4, sizeof(char));
		if (widgetTypetext) {
			sprintf(widgetTypetext, "%d", ptr->type);
			widgetType = ezxml_set_txt(widgetType, widgetTypetext);
			ezxml_insert(widgetType, widget, 1);
		}
		/* Create Name tag and insert RoomWidget name as a text  */
		/* <name type="text">value</name> */
		if (ptr->name) {
			ezxml_t widgetName = ezxml_new(ELEMENT_ROOM_WIDGET_NAME);
			widgetName = ezxml_set_txt(widgetName, ptr->name);
			ezxml_insert(widgetName, widget, 2);
		}

		/* Create IntValue tag and insert RoomWidget integer value as a text */
		/* <intValue type="text">value</intValue> */
		ezxml_t widgetIntValue  = ezxml_new(ELEMENT_ROOM_WIDGET_INT_VALUE);

		char * widgetIntValuetext = calloc(4, sizeof(char));
		if (widgetIntValuetext) {
			sprintf(widgetIntValuetext, "%d", ptr->intValue);
			widgetIntValue = ezxml_set_txt(widgetIntValue, widgetIntValuetext);
			ezxml_insert(widgetIntValue, widget, 3);
		}
		/* Create IntValue tag and insert RoomWidget integer value as a text */
		/* <boolValue type="text">value</boolValue> */
		ezxml_t widgetBoolValue  = ezxml_new(ELEMENT_ROOM_WIDGET_BOOL_VALUE);
		widgetBoolValue = ezxml_set_txt(widgetBoolValue, (ptr->boolValue == 1? "true" : "false"));
		ezxml_insert(widgetBoolValue, widget, 4);

		/* Create status tag and insert RoomWidget integer value as a text */
		/* <status type="text">value</status> */
		ezxml_t widgetStatus = ezxml_new(ELEMENT_ROOM_WIDGET_STATUS);
		char * widgetStatustext = calloc(4, sizeof(char));
		if(widgetStatustext) {
			sprintf(widgetStatustext, "%d", ptr->status);
		    widgetStatus = ezxml_set_txt(widgetStatus, widgetStatustext);
		    ezxml_insert(widgetStatus, widget, 5);
		}
		/* Insert RoomWidget inside Room tag */
		ezxml_insert(widget, roomTAG, widgetPosInXml);
		ptr = ptr->next;
		widgetPosInXml++;
	}

	return roomTAG;

}
