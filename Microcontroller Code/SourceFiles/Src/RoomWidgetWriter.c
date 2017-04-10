/* Includes */
#include "RoomWidgetWriter.h"

/* Private function prototypes */
static void RoomWidgetWriter_WriteRoom(struct Room * room, ezxml_t root);
static void RoomWidgetWriter_WriteWidget(struct RoomWidget* ptr, ezxml_t widget);
static ezxml_t RoomWidgetWriter_AppendIntTag(int intToAppend, char * tagName);

/**
 * @brief  Creates ezxml structure out of RoomConfiguration
 * @param  list: Pointer to the RoomConfiguration
 * @retval ezxml_t structure or NULL if operation was unsuccessful
 */
ezxml_t RoomWidgetWriter_WriteXML(struct RoomList * list) {
	if(!list || !(list->head)) {
		return NULL;
	}
	/* Create root tag <RoomConfiguration><RoomConfiguration/> */
	ezxml_t root = ezxml_new(XML_TAG_ROOM_CONFIGURATION);

	struct Room* ptr = list->head;

	while (ptr) {
		RoomWidgetWriter_WriteRoom(ptr, root);
		ptr = ptr->next;

	}

	return root;
}

/**
 * @brief  Creates ezxml structure out of RoomConfiguration including
 *         only data that is related to this one RoomWidget structure
 * @param  list: Pointer to the RoomConfiguration
 * @param  widget: RoomWidget structure
 * @retval ezxml_t structure or NULL if operation was unsuccessful
 */
ezxml_t RoomWidgetWriter_GetRoomWidgetUpdateXML(struct RoomList * list, struct RoomWidget * widget) {
	if(!list || !(list->head)) {
		return NULL;
	}

	/* First find Room structure where this one widget belongs */
	struct Room* roomPtr = RoomWidget_GetRoomForRoomWidget(list, widget);
	if(!roomPtr) {
		return NULL;
	}
	/* Create root tag <RoomConfigurationUpdate><RoomConfigurationUpdate/> */
	ezxml_t root = ezxml_new(XML_TAG_ROOM_CONFIGURATION_UPDATE);
	/* Create Room tag */
    /* <Room name="" id=""> */
	ezxml_t roomTAG  = ezxml_new(XML_TAG_ROOM);
	if(!root || !roomTAG) {
		return;
	}

	if(roomPtr->name) {
		/* Create roomName attribute */
		roomTAG = ezxml_set_attr(roomTAG, XML_ATTRIBUTE_ROOM_NAME, roomPtr->name);
	}

	char * roomId = calloc(4, sizeof(char));
	if(roomId) {
		sprintf(roomId, "%d", roomPtr->id);
		/* Create roomId attribute */
	    roomTAG = ezxml_set_attr(roomTAG, XML_ATTRIBUTE_ROOM_ID, roomId);
	}

	/* Insert Room tag inside Data tag */
	ezxml_insert(roomTAG, root, 0);
	/* Create RoomWidget tags */
	/* <RoomWidget></RoomWidget> */
	ezxml_t widgetTAG = ezxml_new(XML_TAG_ROOM_WIDGET);
	/* Write widget parameters inside tags */
	RoomWidgetWriter_WriteWidget(widget, widgetTAG);
	/* Insert RoomWidget tag inside Room tag */
	ezxml_insert(widgetTAG, roomTAG, 0);

	return root;
}



static void RoomWidgetWriter_WriteRoom(struct Room * room, ezxml_t root) {
	if(!room || !(room->widgets ) || !(room->widgets->head)) {
		return;
	}

	/* Create Room tag */
	/* <Room name="" id=""> */
	ezxml_t roomTAG  = ezxml_new(XML_TAG_ROOM);

	if(room->name) {
		/* Add roomName attribute */
		roomTAG = ezxml_set_attr(roomTAG, XML_ATTRIBUTE_ROOM_NAME, room->name);
	}

	char * roomId = calloc(3, sizeof(char));

	if(roomId) {
		/* Add roomId attribute */
		sprintf(roomId, "%d", room->id);
		roomTAG = ezxml_set_attr(roomTAG, XML_ATTRIBUTE_ROOM_ID, roomId);
	}


	/* Insert Room tag inside Data tag */
	ezxml_insert(roomTAG, root, 0);
	struct RoomWidget* ptr = room->widgets->head;


	int widgetPosInXml = 0;
	while (ptr) {

		/* Create RoomWidget tags */
		/* <RoomWidget></RoomWidget> */
		ezxml_t widget = ezxml_new(XML_TAG_ROOM_WIDGET);
		if(!widget) {
			ptr = ptr->next;
			continue;
		}

		RoomWidgetWriter_WriteWidget(ptr, widget);
		/* Insert RoomWidget inside Room tag */
		ezxml_insert(widget, roomTAG, widgetPosInXml);

		ptr = ptr->next;
		widgetPosInXml++;
	}

	return;
}

static void RoomWidgetWriter_WriteWidget(struct RoomWidget* ptr, ezxml_t widget) {
	/* current row in xml. Increased when new tag is added */
	int widgetPositionInXml = 0;

	/* Create ID tag and insert RoomWidget Id as a text  */
	/* <id type="text">value</id> */
	ezxml_t widgetId = RoomWidgetWriter_AppendIntTag(ptr->id, XML_TAG_ROOM_WIDGET_ID);
	if (widgetId) {
		ezxml_insert(widgetId, widget, widgetPositionInXml);
		widgetPositionInXml++;

	}

	/* Create type tag and insert RoomWidget type as a text  */
	/* <type type="text">value</type> */
	ezxml_t widgetType = RoomWidgetWriter_AppendIntTag(ptr->type, XML_TAG_ROOM_WIDGET_TYPE);
	if (widgetType) {
		ezxml_insert(widgetType, widget, widgetPositionInXml);
		widgetPositionInXml++;
	}

	/* Create Name tag and insert RoomWidget name as a text  */
	/* <name type="text">value</name> */
	if (ptr->name) {
		ezxml_t widgetName = ezxml_new(XML_TAG_ROOM_WIDGET_NAME);
		widgetName = ezxml_set_txt(widgetName, ptr->name);
		ezxml_insert(widgetName, widget, widgetPositionInXml);
		widgetPositionInXml++;
	}

	/* Create IntValue tag and insert RoomWidget integer value as a text */
	/* <intValue type="text">value</intValue> */
	ezxml_t widgetIntValue = RoomWidgetWriter_AppendIntTag(ptr->intValue, XML_TAG_ROOM_WIDGET_INT_VALUE);
	if (widgetIntValue) {
		ezxml_insert(widgetIntValue, widget, widgetPositionInXml);
		widgetPositionInXml++;
	}

	/* Create IntTargetValue tag and insert RoomWidget integer value as a text */
	/* <IntTargetValue type="text">value</IntTargetValue> */
	ezxml_t widgetIntValueTarget = RoomWidgetWriter_AppendIntTag(ptr->intValueTarget, XML_TAG_ROOM_WIDGET_INT_TARGET_VALUE);
	if (widgetIntValueTarget) {
		ezxml_insert(widgetIntValueTarget, widget, widgetPositionInXml);
		widgetPositionInXml++;
	}

	/* Create IntValue tag and insert RoomWidget integer value as a text */
	/* <boolValue type="text">value</boolValue> */
	if (ptr->boolValue < 2) {
		ezxml_t widgetBoolValue = ezxml_new(XML_TAG_ROOM_WIDGET_BOOL_VALUE);
		if (widgetBoolValue) {
			widgetBoolValue = ezxml_set_txt(widgetBoolValue, (ptr->boolValue == 1 ? "true" : "false"));
			ezxml_insert(widgetBoolValue, widget, widgetPositionInXml);
			widgetPositionInXml++;
		}
	}

	/* Create status tag and insert RoomWidget integer value as a text */
	/* <status type="text">value</status> */
	ezxml_t widgetStatus = RoomWidgetWriter_AppendIntTag(ptr->status, XML_TAG_ROOM_WIDGET_STATUS);
	if (widgetStatus) {
		ezxml_insert(widgetStatus, widget, widgetPositionInXml);
		widgetPositionInXml++;

	}
}


static ezxml_t RoomWidgetWriter_AppendIntTag(int intToAppend, char * tagName) {
	if(!tagName) {
		return NULL;
	}
	ezxml_t newTag = ezxml_new(tagName);
	char * intToAppendText = calloc(3, sizeof(char));

	if(newTag && intToAppendText) {
		sprintf(intToAppendText, "%d", intToAppend);
		newTag = ezxml_set_txt(newTag, intToAppendText);
	}
	return newTag;
}
