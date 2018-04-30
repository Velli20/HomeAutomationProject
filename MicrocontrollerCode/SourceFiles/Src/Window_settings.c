// Includes

#include "DIALOG.h"
#include "main.h"
#include "UI_resources.h"
#include "SWIPELIST.h"
#include <stdio.h>
#include "WM.h"

// Defines

#define ID_WINDOW_SETTINGS_MENU            (GUI_ID_USER + 0x00)
#define ID_IMAGE_VIEW_SETTINGS_MENU_RETURN (GUI_ID_USER + 0x01)
#define ID_TEXT_SETTINGS_MENU_RETURN       (GUI_ID_USER + 0x010)
#define ID_SETTINGS_MENU_SWIPE_LIST        (GUI_ID_USER + 0x03)

static WM_HWIN hThisWindow;

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
		{ WINDOW_CreateIndirect, 0,                   ID_WINDOW_SETTINGS_MENU,            1,  0,  480, 272, 0, 0x0, 0 },
		{ IMAGE_CreateIndirect, 0,                    ID_IMAGE_VIEW_SETTINGS_MENU_RETURN, 10, 5,  48,  48,  0, 0,   0 },
		{ TEXT_CreateIndirect, SETTINGS_WINDOW_TITLE, ID_TEXT_SETTINGS_MENU_RETURN,       65, 20, 80,  20,  0, 0x0, 0 },
		{ SWIPELIST_CreateIndirect, 0,                ID_SETTINGS_MENU_SWIPE_LIST,        0,  58, 480, 214, 0, 0x0, 0 },};


// Settings_InitMenuItems

static inline void Settings_InitMenuItems(WM_HWIN hItem) 
{

	SWIPELIST_AddItem(hItem, SETTINGS_MENU_ITEM_DATE, SETTINGS_LIST_ITEM_HEIGHT);
	SWIPELIST_AddItem(hItem, SETTINGS_MENU_ITEM_TIME, SETTINGS_LIST_ITEM_HEIGHT);
	SWIPELIST_AddItem(hItem, SETTINGS_MENU_ITEM_WIFI, SETTINGS_LIST_ITEM_HEIGHT);
	SWIPELIST_AddItem(hItem, SETTINGS_MENU_ITEM_BLUETOOTH, SETTINGS_LIST_ITEM_HEIGHT);
	SWIPELIST_AddItem(hItem, SETTINGS_MENU_ITEM_ETHERNET, SETTINGS_LIST_ITEM_HEIGHT);
	SWIPELIST_AddItem(hItem, SETTINGS_MENU_ITEM_ROOM_CONFIGURATION, SETTINGS_LIST_ITEM_HEIGHT);
}

// Settings_DialogCallback

static void Settings_DialogCallback(WM_MESSAGE* pMsg) 
{
	WM_HWIN widget;
	int     selection;

	switch ( pMsg->MsgId ) 
	{

	case WM_INIT_DIALOG:

		// Initialization of return icon.

		widget= WM_GetDialogItem(pMsg->hWin, ID_IMAGE_VIEW_SETTINGS_MENU_RETURN);
		IMAGE_SetBitmap(widget, &bmBITMAP_return);

		// Initialization of title.

		widget= WM_GetDialogItem(pMsg->hWin, ID_TEXT_SETTINGS_MENU_RETURN);
		TEXT_SetTextAlign(widget, GUI_TA_LEFT | GUI_TA_VCENTER);
		TEXT_SetFont(widget, WINDOW_TITLE_FONT);

		// Initialization of the swipe list widget.

		widget= WM_GetDialogItem(pMsg->hWin, ID_SETTINGS_MENU_SWIPE_LIST);
		Settings_InitMenuItems(widget);

		// Change swipe list widget appearace.

		SWIPELIST_SetBkColor(widget, SWIPELIST_CI_BK_ITEM_UNSEL, GUI_WHITE);
		SWIPELIST_SetBkColor(widget, SWIPELIST_CI_BK_SEP_ITEM, GUI_WHITE);
		SWIPELIST_SetTextColor(widget, SWIPELIST_CI_ITEM_TEXT_UNSEL, GUI_BLACK);
		SWIPELIST_SetTextColor(widget, SWIPELIST_CI_ITEM_TEXT_SEL, GUI_BLACK);
		SWIPELIST_SetTextColor(widget, SWIPELIST_CI_SEP_ITEM_TEXT, GUI_BLACK);
		SWIPELIST_SetTextColor(widget, SWIPELIST_CI_ITEM_HEADER_UNSEL, GUI_BLACK);
		SWIPELIST_SetTextColor(widget, SWIPELIST_CI_ITEM_HEADER_SEL, GUI_BLACK);
		WM_MOTION_SetMoveable(widget, WM_CF_MOTION_Y, 1);

		break;
	case WM_NOTIFY_PARENT:

		switch ( WM_GetId(pMsg->hWinSrc) ) 
		{

		case ID_IMAGE_VIEW_SETTINGS_MENU_RETURN:
			
			if ( pMsg->Data.v == WM_NOTIFICATION_RELEASED ) 
				WM_HideWindow(hThisWindow);
			
			break;
		case ID_SETTINGS_MENU_SWIPE_LIST:
			
			if ( pMsg->Data.v == WM_NOTIFICATION_RELEASED ) 
			{
				selection = SWIPELIST_GetReleasedItem(WM_GetDialogItem(pMsg->hWin, ID_SETTINGS_MENU_SWIPE_LIST));
				switch ( selection )
				{
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

			}
			break;

		}
		break;
	default:
		WM_DefaultProc(pMsg);
		break;
	}
}

// Settings_CreateSettingsWindow

WM_HWIN Settings_CreateSettingsWindow(void) 
{
	hThisWindow= GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), Settings_DialogCallback, WM_HBKWIN, 0, 0);
	return(hThisWindow);
}
