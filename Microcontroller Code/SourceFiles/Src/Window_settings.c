/* Includes */
#include "DIALOG.h"
#include "main.h"
#include "BITMAPS.h"
#include "SWIPELIST.h"
#include <stdio.h>
#include "WM.h"

/* Funtion prototypes */
WM_HWIN Settings_CreateSettingsWindow(void);

/* Defines */
#define ID_WINDOW_SETTINGS_MENU  (GUI_ID_USER + 0x00)
#define ID_IMAGE_VIEW_SETTINGS_MENU_RETURN  (GUI_ID_USER + 0x01)
#define ID_TEXT_SETTINGS_MENU_RETURN  (GUI_ID_USER + 0x010)
#define ID_SETTINGS_MENU_SWIPE_LIST  (GUI_ID_USER + 0x03)

#define WINDOW_SETTINGS_WINDOW_TITLE 			"Asetukset"

/* Static data */
static WM_HWIN hThisWindow;

const char months_FIN[12][15] = {"Tammikuu", "Helmikuu", "Maaliskuu", "Huhtikuu", "Toukokuu", "Kesäkuu",
		"Heinäkuu", "Elokuu", "Syyskuu", "Lokakuu", "Marraskuu", "Joulukuu"};

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
		{ WINDOW_CreateIndirect, NULL, ID_WINDOW_SETTINGS_MENU, 1, 0, 480, 272, 0, 0x0, 0 },
		{ IMAGE_CreateIndirect, NULL, ID_IMAGE_VIEW_SETTINGS_MENU_RETURN, 10, 10, 48, 48, 0, 0, 0 },
		{ TEXT_CreateIndirect, WINDOW_SETTINGS_WINDOW_TITLE, ID_TEXT_SETTINGS_MENU_RETURN, 65, 24, 80, 20, 0, 0x0, 0 },
		{ SWIPELIST_CreateIndirect, NULL, ID_SETTINGS_MENU_SWIPE_LIST, 0, 58, 480, 214, 0, 0x0, 0 },};

/* Private code */

static void Settings_InitMenuItems(WM_HWIN hItem) {

	SWIPELIST_AddItem(hItem, "Päivämäärä", 48);
	SWIPELIST_SetBitmap(hItem, 0, SWIPELIST_BA_LEFT, &bmBITMAP_calendar);
	SWIPELIST_AddItem(hItem, "Aika", 48);
	SWIPELIST_SetBitmap(hItem, 1, SWIPELIST_BA_LEFT, &bmBITMAP_clock);
	SWIPELIST_AddItem(hItem, "WiFi", 48);
	SWIPELIST_AddItem(hItem, "Bluetooth", 48);
	SWIPELIST_AddItem(hItem, "Ethernet", 48);
	SWIPELIST_AddItem(hItem, "Huone konfiguraatio", 48);
}

static void _cbDialog(WM_MESSAGE * pMsg) {
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
		TEXT_SetFont(hItem, GUI_FONT_13B_1);

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
			case WM_NOTIFICATION_CLICKED:
				break;
			case WM_NOTIFICATION_RELEASED:
				WM_HideWindow(hThisWindow);
				break;
			}
			break;
		case ID_SETTINGS_MENU_SWIPE_LIST:
			switch (NCode) {
			case WM_NOTIFICATION_RELEASED:
				selection = SWIPELIST_GetReleasedItem(WM_GetDialogItem(pMsg->hWin, ID_SETTINGS_MENU_SWIPE_LIST));
				if(selection == 1) {
					WM_ShowWindow(SettingsTime_CreateSettingsTimeWindow());
				} else if(selection == 5) {
					WM_ShowWindow(WindowRoomConfiguration_CreateWindow());
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

WM_HWIN Settings_CreateSettingsWindow(void) {
	hThisWindow = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
	return hThisWindow;
}

/* End of file */
