#include "RoomWidgetParser.h"


/* Private function prototypes */
static void RoomWidgetParser_ParseStringValue(const ezxml_t node, char ** target);
static void RoomWidgetParser_CopyStringValue(const char * valueToCopy, char ** target);
static int RoomWidgetParser_ParseIntegerValue(const ezxml_t node, int defaultValue);
static int RoomWidgetParser_ConvertStringToIntegerValue(const char * valueToConvert, int defaultValue);
static int RoomWidgetParser_ParseBoolValue(const ezxml_t node);

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
struct RoomList* RoomWidgetParser_ParseString(char * xmlString, size_t length) {
	struct RoomList *list = NULL;
	if(!xmlString || length == 0) {
		return NULL;
	}

	ezxml_t node = ezxml_parse_str(xmlString, length);

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
	ezxml_t roomNode = ezxml_child(a_node, XML_TAG_ROOM);
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
 * @brief  Try to parse requestCode attribute from RoomConfiguration node
 * @param  roomConfiguration: Pointer to the xml node
 * @retval request code or -1 if operation was unsuccessful
 */
int RoomWidgetParser_ParseRequestCode(const ezxml_t roomConfiguration) {
	if(!roomConfiguration) {
		return -1;
	}
	return RoomWidgetParser_ConvertStringToIntegerValue(ezxml_attr(roomConfiguration, XML_ATTRIBUTE_REQUEST_CODE),-1);
}

/**
 * @brief  Search for the widget data that is wrapped with <RoomWidget></RoomWidget> tags
 * 		   and parse it.
 * @param  roomNode: Pointer to the xml node wrapped with <Room></Room> tags
 * @retval Room structure
 */
struct Room* RoomWidgetParser_ParseRoom(const ezxml_t roomNode) {
	if(!roomNode) {
		return NULL;
	}
	/* Default value for the room id if not defined in xml*/
	static int roomIdDeafult = 0;

	struct Room *room = calloc(1, sizeof(struct Room));
	if (!room) {
		/* Memory allocation failed */
		return NULL;
	}

	room->widgets = calloc(1, sizeof(struct RoomWidgetList));
	if(!(room->widgets)) {
		/* Memory allocation failed. Free allocated memory for Room structure */
		free(room);
		return NULL;
	}

	/* Search for <RoomWidget> tags */
	ezxml_t widgetNode = ezxml_child(roomNode, XML_TAG_ROOM_WIDGET);

	while (widgetNode) {
		/* try to parse RoomWidget */
		struct RoomWidget * widget = RoomWidgetParser_ParseWidget(widgetNode);

		if (widget) {
			RoomWidget_AddWidgetToRoom(room, widget);
		}
		widgetNode = widgetNode->next;
	}

	/* Try to parse room widget name. name attribute*/
	RoomWidgetParser_CopyStringValue(ezxml_attr(roomNode, XML_ATTRIBUTE_ROOM_NAME), &(room->name));
	/* Try to parse room id. id attribute */
	(room->id) = RoomWidgetParser_ConvertStringToIntegerValue(ezxml_attr(roomNode, XML_ATTRIBUTE_ROOM_ID), roomIdDeafult++);

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
	/* Default value for the room id if not defined in xml */
	static int roomDefaultId = 0;

	if(!widget) {
		/* Memory allocation failed */
		return NULL;
	}

	/* Try to parse <id> tags */
	(widget->id) = RoomWidgetParser_ParseIntegerValue(ezxml_child(widgetNode, XML_TAG_ROOM_WIDGET_ID), roomDefaultId++);

	/* Try to parse <type> tags */
	(widget->type) = RoomWidgetParser_ParseIntegerValue(ezxml_child(widgetNode, XML_TAG_ROOM_WIDGET_TYPE), -1);

	/* Try to parse <name> tags */
	RoomWidgetParser_ParseStringValue(ezxml_child(widgetNode, XML_TAG_ROOM_WIDGET_NAME), &(widget->name));

	/* Try to parse <intValue> tags */
	(widget->intValue) = RoomWidgetParser_ParseIntegerValue(ezxml_child(widgetNode, XML_TAG_ROOM_WIDGET_INT_VALUE), 0);

	/* Try to parse <intTargetValue> tags */
	(widget->intValueTarget) = RoomWidgetParser_ParseIntegerValue(ezxml_child(widgetNode, XML_TAG_ROOM_WIDGET_INT_TARGET_VALUE), 0);

	/* Try to parse <boolValue> tags */
	(widget->boolValue) = RoomWidgetParser_ParseBoolValue(ezxml_child(widgetNode, XML_TAG_ROOM_WIDGET_BOOL_VALUE));

	/* Try to parse <pin> tags */
	(widget->arduinoPin) = RoomWidgetParser_ParseIntegerValue(ezxml_child(widgetNode, XML_TAG_ROOM_WIDGET_PIN), -1);

	/* Try to parse <status> tags */
	(widget->status) = RoomWidgetParser_ParseIntegerValue(ezxml_child(widgetNode, XML_TAG_ROOM_WIDGET_STATUS), 0);

	return widget;
}

/**
 * @brief  Parse string value from ezxml_t structure
 * @param  node: ezxml_t structure to parse
 * @param  target: Pointer to pointer to the char array where to store array
 */
static void RoomWidgetParser_ParseStringValue(const ezxml_t node, char ** target) {
	if(!node || !(node->txt)) {
		return;
	}
	RoomWidgetParser_CopyStringValue(node->txt, target);
}

/**
 * @brief  Copies char array pointed by valueToCopy to target
 * @param  valueToCopy: Pointer to the source char array
 * @param  target: Pointer to pointer to the destination char array
 */
static void RoomWidgetParser_CopyStringValue(const char * valueToCopy, char ** target) {
	if(!valueToCopy) {
		return;
	}
	size_t stringValueLength = strlen(valueToCopy);

	if(stringValueLength == 0) {
		/* Beware of zero length allocations */
		return;
	}

	/* Allocate memory for copy */
	char * stringValueCpy = calloc(stringValueLength+1, sizeof(char));

	if(stringValueCpy) {

		strcpy(stringValueCpy, valueToCopy);
		/* Force null termination */
		stringValueCpy[stringValueLength] = '\0';
		*target = stringValueCpy;
	}
}

/**
 * @brief  Parse integer value from ezxml_t structure
 * @param  node: ezxml_t structure to parse
 * @retval parsed int value or deafult value if operation was unsuccessful
 */
static int RoomWidgetParser_ParseIntegerValue(const ezxml_t node, int defaultValue) {
	if(!node || !(node->txt)) {
		return defaultValue;
	}
	return RoomWidgetParser_ConvertStringToIntegerValue((node->txt), defaultValue);
}

/**
 * @brief  Convert char array to integer
 * @param  valueToConvert: Pointer to the char array to convert
 * @param  defaultValue: Default value to return if char
 *         array dosen't contain digits
 * @retval parsed int value or default value if operation
 *         was unsuccessful
 */
static int RoomWidgetParser_ConvertStringToIntegerValue(const char * valueToConvert, int defaultValue) {
	if(!valueToConvert) {
		return defaultValue;
	}
	char * integerTextEndPtr;

	/* Try to parse long from string */
	long result = strtol(valueToConvert, integerTextEndPtr, 0);

	if (integerTextEndPtr == (valueToConvert)) {
	    /* No value parsed */
		return defaultValue;
	}
	if ((result == LONG_MAX || result == LONG_MIN)) {
	    /* Out of range */
		return defaultValue;
	}
	return ((int) result);
}

/**
 * @brief  Parse bool value from ezxml_t structure
 * @param  node: ezxml_t structure to parse
 */
static int RoomWidgetParser_ParseBoolValue(const ezxml_t node) {
	if(!node || !(node->txt)) {
		return 0;
	}
	const char * boolValue = node->txt;

	if (strlen(boolValue) > 0) {
		return (strcmp(boolValue, "true") == 0 ? 1 : 0);
	}
	return 0;
}
