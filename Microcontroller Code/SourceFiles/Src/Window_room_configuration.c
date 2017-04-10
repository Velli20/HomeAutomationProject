#include "main.h"
#include "DIALOG.h"
#include "SWIPELIST.h"
#include "WM.h"
#include "UI_resources.h"

/* Defines */
#define ID_WINDOW_ROOM_CONFIGURATION 		(GUI_ID_USER + 0x22)
#define ID_ROOM_CONFIGURATION_LIST  		(GUI_ID_USER + 0x02)
#define ID_TEXT_RETURN  					(GUI_ID_USER + 0x03)
#define ID_IMAGE_VIEW_RETURN  				(GUI_ID_USER + 0x04)


#define LIST_ITEM_HEIGHT 56

/* private function prototypes */
static int WindowRoomConfiguration_MoveInRoomConfigurationList(WM_HWIN hList, WM_HWIN hTitle, int index, int back);
static void WindowRoomConfiguration_PrintRoomNames(struct RoomList * list, WM_HWIN hItem, WM_HWIN hTitle);
static void WindowRoomConfiguration_PrintWidgetNames(struct RoomList * list, int roomIndex, WM_HWIN hItem, WM_HWIN hTitle);
static void WindowRoomConfiguration_PrintWidgetValues(struct RoomList * list, WM_HWIN hItem, WM_HWIN hTitle, int widgetPos, int roomPos);

static void WindowRoomConfiguration_AppendItemText(WM_HWIN hItem, int position, const * stringF, ...);
static void WindowRoomConfiguration_AppendHeaderText(WM_HWIN hItem, const * stringF, ...);
static void WindowRoomConfiguration_SWIPELIST_Clear(WM_HWIN hItem);

static void WindowRoomConfiguration_DialogCallback(WM_MESSAGE * pMsg);

/* Private variables */
static WM_HWIN hThisWindow;



/* Declared in RoomConfiguration.c */
extern struct RoomList * roomConfiguration;

/* Static data */

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
		{ WINDOW_CreateIndirect, NULL, ID_WINDOW_ROOM_CONFIGURATION, 0, 0, 480, 272, 0, 0x0, 0 },
		{ IMAGE_CreateIndirect, NULL, ID_IMAGE_VIEW_RETURN,10, 5, 48, 48, 0, 0, 0 },
		{ TEXT_CreateIndirect, WINDOW_ROOM_CONFIGURATION_TITLE, ID_TEXT_RETURN, 65, 20, 140, 20, 0, 0x0, 0 },
		{ SWIPELIST_CreateIndirect, NULL, ID_ROOM_CONFIGURATION_LIST, 0, 59, 480, 214, 0, 0x0, 0 }, };


/* Private code */


/**
 * @brief  Handle callback events from parent window
 * @param  pMsg: callback message structure from parent window
 * @retval None
 */
static void WindowRoomConfiguration_DialogCallback(WM_MESSAGE * pMsg) {
	WM_HWIN hItem;
	WM_HWIN hList;
	WM_HWIN hTitle;
	WM_HWIN window;
	int NCode;
	int Id;
	int selectedListItem;
	int result;

	switch (pMsg->MsgId) {
	case WM_INIT_DIALOG:
		WINDOW_SetBkColor(pMsg->hWin, GUI_BLACK);
		/* Initialization of return icon */
		hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_VIEW_RETURN);
		IMAGE_SetBitmap(hItem, &bmBITMAP_return);

		/* Initialization of title */
		hTitle = WM_GetDialogItem(pMsg->hWin, ID_TEXT_RETURN);
		TEXT_SetTextAlign(hTitle, GUI_TA_LEFT | GUI_TA_VCENTER);
		TEXT_SetFont(hTitle, WINDOW_TITLE_FONT);
		TEXT_SetTextColor(hTitle, GUI_WHITE);

		/* Initialization of SWIPELIST widget */
		hList = WM_GetDialogItem(pMsg->hWin, ID_ROOM_CONFIGURATION_LIST);
		WM_MOTION_SetMoveable(hList, WM_CF_MOTION_Y, 1);
		/* Print room names */
		WindowRoomConfiguration_MoveInRoomConfigurationList(hList, hTitle, 0, 0);

		break;
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		hList = WM_GetDialogItem(pMsg->hWin, ID_ROOM_CONFIGURATION_LIST);
		hTitle = WM_GetDialogItem(pMsg->hWin, ID_TEXT_RETURN);

		switch (Id) {

		case ID_IMAGE_VIEW_RETURN:
			switch (NCode) {
			case WM_NOTIFICATION_RELEASED:
				/* Move back in RoomConfiguration list */
				result = WindowRoomConfiguration_MoveInRoomConfigurationList(hList, hTitle, -1, 1);
				if(result == 0) {
					WM_HideWindow(hThisWindow);

				}
				break;
			}
			break;
		case ID_ROOM_CONFIGURATION_LIST:
			switch (NCode) {
			case WM_NOTIFICATION_RELEASED:
				/* User has pressed list item */
				/* Get position of selected list item */
				selectedListItem = SWIPELIST_GetReleasedItem(hList);
				/* Inflate SWIPELIST widget with selected item */
				WindowRoomConfiguration_MoveInRoomConfigurationList(hList,
						hTitle, selectedListItem, 0);
				break;
			}


		break;


		}
		break;

	default:
		WM_DefaultProc(pMsg);
		break;
	}
}




/**
 * @brief  Inflates SwipeList with room names, widget names or widget values
 * @param  hList: Handle of SwipeList widget
 * @param  hTitle: Handle of title Text widget
 * @param  index: Position of selected list item index
 * @param  back: 1: Move backward in RoomConfiguration
 *         back: 0: Move forward in RoomConfiguration
 * @retval 1 if operation was successful otherwise 0
 */
static int WindowRoomConfiguration_MoveInRoomConfigurationList(WM_HWIN hList, WM_HWIN hTitle, int index, int back) {
	static int roomPos = 0;
	static int menuPositionInRoomConfiguartion = -1;

	if(back == 1 && menuPositionInRoomConfiguartion <= 0) {
		menuPositionInRoomConfiguartion = -1;
		return 0;
	} else if(back == 1 && menuPositionInRoomConfiguartion > 0) {
		menuPositionInRoomConfiguartion--;
	} else if(back == 0 && menuPositionInRoomConfiguartion < 2) {
		menuPositionInRoomConfiguartion++;
	} else {
		return 1;
	}

	/* Clear SwipeList widget */
	WindowRoomConfiguration_SWIPELIST_Clear(hList);

	switch(menuPositionInRoomConfiguartion) {
	case 0: /* Print room names */
		WindowRoomConfiguration_PrintRoomNames(roomConfiguration, hList, hTitle);
		break;
	case 1: /* Print widget names */
		if(index >= 0) {
			roomPos = index;
		}
		WindowRoomConfiguration_PrintWidgetNames(roomConfiguration, roomPos, hList, hTitle);
		break;
	case 2: /* Print widget values */
		WindowRoomConfiguration_PrintWidgetValues(roomConfiguration, hList, hTitle, index, roomPos);
		break;
	}
	return 1;
}

/**
 * @brief  Prints room configuration and returns approximate height of text in px
 * @param  hItem: Handle of text widget to print
 * @retval Approximate height of the text.
 */
static void WindowRoomConfiguration_PrintRoomNames(struct RoomList * list, WM_HWIN hItem, WM_HWIN hTitle) {
	int pos = 0;

	if(!hItem || !list || !hTitle) {
		return 0;
	}

	/* Change window title to default */
	TEXT_SetText(hTitle, WINDOW_ROOM_CONFIGURATION_TITLE);

	struct Room* roomPtr = list->head;

	while (roomPtr) {
		if(roomPtr->name) {
			WindowRoomConfiguration_AppendHeaderText(hItem, "%s", roomPtr->name);

		}

		roomPtr = roomPtr->next;

	}
}

/**
 * @brief  Prints room configuration and returns approximate height of text in px
 * @param  hItem: Handle of text widget to print
 * @retval Approximate height of the text.
 */
static void WindowRoomConfiguration_PrintWidgetNames(struct RoomList * list, int roomIndex, WM_HWIN hItem, WM_HWIN hTitle) {
	int pos = 0;
	int index = 0;

	if(!hItem || !list || !hTitle) {
		return;
	}

	struct Room* roomPtr = list->head;

	while (roomPtr) {
		if (index == roomIndex) {
			if (roomPtr->name) {
				/* Change window title to selected room name */
				TEXT_SetText(hTitle, roomPtr->name);
			} else {
				TEXT_SetText(hTitle, WINDOW_ROOM_CONFIGURATION_ROOM_NO_NAME);
			}

			if (roomPtr->widgets) {
				struct RoomWidget* widgetPtr = roomPtr->widgets->head;

				while (widgetPtr) {
					if (widgetPtr->name) {
						/* Append widget name */
						WindowRoomConfiguration_AppendHeaderText(hItem, "Widget: %s id: %d", widgetPtr->name, widgetPtr->id);
					} else {
						WindowRoomConfiguration_AppendHeaderText(hItem, "Widget: %s id: %d", WINDOW_ROOM_CONFIGURATION_WIDGET_NO_NAME, widgetPtr->id);
					}

					widgetPtr = widgetPtr->next;
				}
			}
			break;
		}

		index++;
		roomPtr = roomPtr->next;
	}
}

/**
 * @brief  Prints room configuration and returns approximate height of text in px
 * @param  hItem: Handle of text widget to print
 * @retval Approximate height of the text.
 */
static void WindowRoomConfiguration_PrintWidgetValues(struct RoomList * list, WM_HWIN hItem, WM_HWIN hTitle, int widgetPos, int roomPos) {
	int pos = 0;
	int widgetIterationPos = 0;
	int roomIterationPos = 0;

	if(!hItem || !list || !hTitle) {
		return;
	}


	struct Room* roomPtr = list->head;

	while (roomPtr) {

		/* Iterate trough linked list until we are at right position */
		if(roomIterationPos != roomPos) {
			roomIterationPos++;
			roomPtr = roomPtr->next;
			continue;
		}
		struct RoomWidget* widgetPtr = roomPtr->widgets->head;

		while (widgetPtr) {

			if(widgetIterationPos != widgetPos) {
				widgetIterationPos++;
				widgetPtr = widgetPtr->next;
				continue;
			}

			if(widgetPtr->name) {
				/* Change window title to selected RoomWidget name */
				TEXT_SetText(hTitle, widgetPtr->name);
			} else {
				/* Widget does not have name. Use default */
				TEXT_SetText(hTitle, WINDOW_ROOM_CONFIGURATION_WIDGET_NO_NAME);
			}

			/* Append widget type */
			WindowRoomConfiguration_AppendHeaderText(hItem, "Widget type: %d", widgetPtr->type);
			pos++;

			/* Append widget int value */
			WindowRoomConfiguration_AppendHeaderText(hItem, "Widget int value: %d", widgetPtr->intValue);
			pos++;

			/* Append widget int traget value */
			WindowRoomConfiguration_AppendHeaderText(hItem, "Widget int target value: %d", widgetPtr->intValueTarget);
			pos++;

			/* Append widget bool value */
			WindowRoomConfiguration_AppendHeaderText(hItem, "Widget bool value: %s", (widgetPtr->boolValue == 1 ? "true" : "false"));
			pos++;

			WindowRoomConfiguration_AppendHeaderText(hItem, "Widget status value: %d", widgetPtr->status);
			pos++;
			break;

		}
		break;

	}

}

/**
  * @brief Deletes all items on SWIPELIST widget
  * @param hItem: Handle to SWIPELIST widget
  * @retval None
  */
static void WindowRoomConfiguration_SWIPELIST_Clear(WM_HWIN hItem) {
	if(!hItem) {
		return;
	}

}

static void WindowRoomConfiguration_AppendItemText(WM_HWIN hItem, int position, const * stringF, ...) {
	char valueText[100] = {0};
	va_list ap;
	va_start(ap, stringF);

	vsprintf(valueText, stringF, ap);
	SWIPELIST_AddItemText(hItem, position, valueText);
	va_end(ap);
}

static void WindowRoomConfiguration_AppendHeaderText(WM_HWIN hItem, const * stringF, ...) {
	char valueText[100] = {0};
	va_list ap;
	va_start(ap, stringF);

	vsprintf(valueText, stringF, ap);
	SWIPELIST_AddItem(hItem, valueText, LIST_ITEM_HEIGHT);
	va_end(ap);

}

/*  Public code */

/**
  * @brief Create room configuration window
  * @param None
  * @retval Handle pointing to room configuration window
  */
WM_HWIN WindowRoomConfiguration_CreateWindow(void) {
	WM_MOTION_Enable(1);
	hThisWindow = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate),WindowRoomConfiguration_DialogCallback, WM_HBKWIN, 0, 0);
	return hThisWindow;
}

/* End of file */
