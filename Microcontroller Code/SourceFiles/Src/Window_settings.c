/* Includes */
#include "DIALOG.h"
#include "main.h"
#include "UI_resources.h"
#include "SWIPELIST.h"
#include <stdio.h>
#include "WM.h"


/* Defines */
#define ID_WINDOW_SETTINGS_MENU  (GUI_ID_USER + 0x00)
#define ID_IMAGE_VIEW_SETTINGS_MENU_RETURN  (GUI_ID_USER + 0x01)
#define ID_TEXT_SETTINGS_MENU_RETURN  (GUI_ID_USER + 0x010)
#define ID_SETTINGS_MENU_SWIPE_LIST  (GUI_ID_USER + 0x03)

/* Private function prototypes */
static void Settings_InitMenuItems(WM_HWIN hItem);
static void Settings_DialogCallback(WM_MESSAGE * pMsg);

/* Static data */
static WM_HWIN hThisWindow;



static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
		{ WINDOW_CreateIndirect, NULL, ID_WINDOW_SETTINGS_MENU, 1, 0, 480, 272, 0, 0x0, 0 },
		{ IMAGE_CreateIndirect, NULL, ID_IMAGE_VIEW_SETTINGS_MENU_RETURN, 10, 5, 48, 48, 0, 0, 0 },
		{ TEXT_CreateIndirect, SETTINGS_WINDOW_TITLE, ID_TEXT_SETTINGS_MENU_RETURN, 65, 20, 80, 20, 0, 0x0, 0 },
		{ SWIPELIST_CreateIndirect, NULL, ID_SETTINGS_MENU_SWIPE_LIST, 0, 58, 480, 214, 0, 0x0, 0 },};

/* Private code */

/**
 * @brief  Inflate SWIPELIST widget with menu items
 * @param  hItem: Handle of SwipeList widget
 * @retval None
 */
static void Settings_InitMenuItems(WM_HWIN hItem) {

	SWIPELIST_AddItem(hItem, SETTINGS_MENU_ITEM_DATE, SETTINGS_LIST_ITEM_HEIGHT);
	SWIPELIST_AddItem(hItem, SETTINGS_MENU_ITEM_TIME, SETTINGS_LIST_ITEM_HEIGHT);
	SWIPELIST_AddItem(hItem, SETTINGS_MENU_ITEM_WIFI, SETTINGS_LIST_ITEM_HEIGHT);
	SWIPELIST_AddItem(hItem, SETTINGS_MENU_ITEM_BLUETOOTH, SETTINGS_LIST_ITEM_HEIGHT);
	SWIPELIST_AddItem(hItem, SETTINGS_MENU_ITEM_ETHERNET, SETTINGS_LIST_ITEM_HEIGHT);
	SWIPELIST_AddItem(hItem, SETTINGS_MENU_ITEM_ROOM_CONFIGURATION, SETTINGS_LIST_ITEM_HEIGHT);
}

/**
 * @brief  Handle callback events from parent window
 * @param  pMsg: callback message structure from parent window
 * @retval None
 */
static void Settings_DialogCallback(WM_MESSAGE * pMsg) {
	WM_HWIN hItem;
	int NCode;
	int Id;
	int selection;

	switch (pMsg->MsgId) {
	case WM_INIT_DIALOG:
		/* Initialization of return icon */
		hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_VIEW_SETTINGS_MENU_RETURN);
		IMAGE_SetBitmap(hItem, &bmBITMAP_return);

		/* Initialization of title */
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_SETTINGS_MENU_RETURN);
		TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
		TEXT_SetFont(hItem, WINDOW_TITLE_FONT);

		hItem = WM_GetDialogItem(pMsg->hWin, ID_SETTINGS_MENU_SWIPE_LIST);

		/* Populate SwipeList with menu items */
		Settings_InitMenuItems(hItem);

		SWIPELIST_SetBkColor(hItem, SWIPELIST_CI_BK_ITEM_UNSEL, GUI_WHITE);
		SWIPELIST_SetBkColor(hItem, SWIPELIST_CI_BK_SEP_ITEM, GUI_WHITE);
		SWIPELIST_SetTextColor(hItem, SWIPELIST_CI_ITEM_TEXT_UNSEL, GUI_BLACK);
		SWIPELIST_SetTextColor(hItem, SWIPELIST_CI_ITEM_TEXT_SEL, GUI_BLACK);
		SWIPELIST_SetTextColor(hItem, SWIPELIST_CI_SEP_ITEM_TEXT, GUI_BLACK);
		SWIPELIST_SetTextColor(hItem, SWIPELIST_CI_ITEM_HEADER_UNSEL, GUI_BLACK);
		SWIPELIST_SetTextColor(hItem, SWIPELIST_CI_ITEM_HEADER_SEL, GUI_BLACK);
		WM_MOTION_SetMoveable(hItem, WM_CF_MOTION_Y, 1);

		break;
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch (Id) {

		case ID_IMAGE_VIEW_SETTINGS_MENU_RETURN:
			switch (NCode) {
			case WM_NOTIFICATION_RELEASED:
				WM_HideWindow(hThisWindow);
				break;
			}
			break;
		case ID_SETTINGS_MENU_SWIPE_LIST:
			switch (NCode) {
			case WM_NOTIFICATION_RELEASED:
				selection = SWIPELIST_GetReleasedItem(WM_GetDialogItem(pMsg->hWin, ID_SETTINGS_MENU_SWIPE_LIST));
				switch(selection) {
				case 0:
					WM_ShowWindow(SettingsDate_CreateSettingsDateWindow());
					break;
				case 1:
					WM_ShowWindow(SettingsTime_CreateSettingsTimeWindow());
					break;
				case 5:
					WM_ShowWindow(WindowRoomConfiguration_CreateWindow());
					break;
				}

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

/* Public code */

/**
  * @brief Create settings window
  * @param None
  * @retval Handle pointing to settings window
  */
WM_HWIN Settings_CreateSettingsWindow(void) {
	hThisWindow = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), Settings_DialogCallback, WM_HBKWIN, 0, 0);
	return hThisWindow;
}

/* End of file */
