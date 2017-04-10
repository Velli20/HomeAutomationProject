/* Includes */
#include "DIALOG.h"
#include "main.h"
#include "UI_resources.h"

/* Defines */
#define ID_WINDOW_MENU  			(GUI_ID_USER + 0x00)
#define ID_ICONVIEW_MENU  			(GUI_ID_USER + 0x01)
#define ID_STATUS_BAR_ICON_SD_CARD  (GUI_ID_USER + 0x02)
#define ID_TEXT_TIME  				(GUI_ID_USER + 0x03)
#define ID_TEXT_DATE  				(GUI_ID_USER + 0x04)
#define ID_TEXT_TEMPERATURE  		(GUI_ID_USER + 0x05)


/* Private function prototypes */
static void Menu_SetMenuItemSelected(WM_HWIN hItem, int selected);
static void Menu_DialogCallback(WM_MESSAGE * pMsg);



/* Static data */
static const BITMAP_ITEM mBitmaps[] = {
		{ &bmBITMAP_lights, &bmBITMAP_lights_selected, WINDOW_MENU_MENU_ITEM_LIGHTS },
		{ &bmBITMAP_temperature, &bmBITMAP_temperature_selected, WINDOW_MENU_MENU_ITEM_TMEPERATURE },
		{ &bmBITMAP_console, &bmBITMAP_console_selected, WINDOW_MENU_MENU_ITEM_COMMANDS },
		{ &bmBITMAP_settings, &bmBITMAP_settings_selected, WINDOW_MENU_MENU_ITEM_SETTINGS }, };

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
		{ WINDOW_CreateIndirect, NULL, ID_WINDOW_MENU, 0, 0, 480, 272, 0, 0x0, 0 },
		{ ICONVIEW_CreateIndirect, NULL, ID_ICONVIEW_MENU, 0, 20, 480, 272, 0, 0x00600060, 0 },
		{ IMAGE_CreateIndirect, NULL, ID_STATUS_BAR_ICON_SD_CARD, 63, 0, 26, 27, 0, 0, 0 },
		{ TEXT_CreateIndirect, NULL, ID_TEXT_TIME, 400, 0, 80, 20, 0, 0x0, 0 },
		{ TEXT_CreateIndirect, NULL, ID_TEXT_DATE, 320, 0, 80, 20, 0, 0x0, 0 },
		{ TEXT_CreateIndirect, TEMPERATURE_UNIT, ID_TEXT_TEMPERATURE, 0, 0, 80, 20, 0, 0x0, 0 },};


/* Variables */
WM_HWIN hThisWindow;

/* Variable to store latest reading of the temperature sensor declared in main.c */
extern uint8_t temperatureReading;


/* Private code */

/**
 * @brief  Handle callback events from parent window
 * @param  pMsg: callback message structure from parent window
 * @retval None
 */
static void Menu_DialogCallback(WM_MESSAGE * pMsg) {
	int NCode;
	int Id;
	int selection;
	WM_HWIN hItem;

	switch (pMsg->MsgId) {

	case WM_INIT_DIALOG:

		/* Initialization of µSD card status bar icon */
		hItem = WM_GetDialogItem(pMsg->hWin, ID_STATUS_BAR_ICON_SD_CARD);

		if (BSP_SD_IsDetected() == SD_PRESENT) {
			WM_ShowWin(hItem);
		} else {
			WM_HideWin(hItem);
		}
		IMAGE_SetBitmap(hItem, &bmBITMAP_sd_card);

		/* Initialization of status bar time text */

		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_TIME);
		TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		TEXT_SetFont(hItem, MENU_WINDOW_TIME_TEXT_FONT);

		/* Initialization of status bar date text */

		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_DATE);
		TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		TEXT_SetFont(hItem, MENU_WINDOW_DATE_TEXT_FONT);

		/* Initialization of status bar temperature text */


		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_TEMPERATURE);
		TEXT_SetFont(hItem, MENU_WINDOW_TEMPERATURE_TEXT_FONT);
		TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		Menu_StatusBarUpdateTemperatureReading();


		/* Initialization of menu */

		hItem = WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_MENU);
		ICONVIEW_SetSpace(hItem, GUI_COORD_X, 24);
		ICONVIEW_SetSpace(hItem, GUI_COORD_Y, 10);
		ICONVIEW_SetFont(hItem, MENU_WINDOW_MENU_ITEM_TEXT_FONT);
		ICONVIEW_SetSel(hItem, -1);

		int i;
		for (i = 0; i < GUI_COUNTOF(mBitmaps); i++) {
			ICONVIEW_AddBitmapItem(hItem, mBitmaps[i].pBitmap, mBitmaps[i].pText);
			ICONVIEW_SetTextColor(hItem, i, 0x000000);
		}
		break;
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		hItem = WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_MENU);
		NCode = pMsg->Data.v;

		switch (Id) {
		case ID_ICONVIEW_MENU:
			switch (NCode) {
			case WM_NOTIFICATION_CLICKED:
				Menu_SetMenuItemSelected(hItem, 1);
				break;
			case WM_NOTIFICATION_RELEASED:
				Menu_SetMenuItemSelected(hItem, 0);
				selection = ICONVIEW_GetSel(hItem);

				ICONVIEW_SetSel(hItem, -1);

				switch (selection) {
				case 0:
					WM_ShowWindow(Lights_CreateLightsWindow());
					break;
				case 1:
					WM_ShowWindow(Temperature_CreateTemperatureWindow());
					break;
				case 2:
					break;
				case 3:
					WM_ShowWindow(Settings_CreateSettingsWindow());
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

/**
 * @brief  Change bitmap of the selected menu item to show selection
 * @param  hItem: Handle of ICONVIEW widget
 * @param  selected: 1 if item is clicked or 0 if item is released
 */
static void Menu_SetMenuItemSelected(WM_HWIN hItem, int selected) {
	if(!hItem) {
		return;
	}
	int selection = ICONVIEW_GetSel(hItem);

	if(selection < 0 || selection > 3) {
		return;
	}

	if(selected) {
		ICONVIEW_SetBitmapItem(hItem, selection, mBitmaps[selection].pBitmapSelected);
	} else {
		ICONVIEW_SetBitmapItem(hItem, selection, mBitmaps[selection].pBitmap);
	}
}



/* Public code */


/**
  * @brief Create menu window
  * @param None
  * @retval Handle pointing to menu window
  */
WM_HWIN Menu_CreateMenuWindow(void) {

	hThisWindow = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), Menu_DialogCallback, WM_HBKWIN, 0, 0);
	return hThisWindow;
}


/**
  * @brief Update date time text on status bar
  * @param None
  * @retval None
  */
void Menu_StatusBarUpdateDateTime() {
	if(!hThisWindow) {
		return;
	}
	int up = 0;
	uint8_t timeText[20];

	RTC_TimeTypeDef stimestructureget;
	RTC_DateTypeDef sdatestructureget;

	/* Get the RTC current Time */
	stimestructureget = BSP_RTC_GetTime();
	sdatestructureget = BSP_RTC_GetDate();

	WM_HWIN hTime = WM_GetDialogItem(hThisWindow, ID_TEXT_TIME);
	WM_HWIN hDate = WM_GetDialogItem(hThisWindow, ID_TEXT_DATE);

	Menu_StatusBarUpdateTemperatureReading();

	if (hTime && hDate && timeText) {
		up = (stimestructureget.Seconds % 2 == 0? 1 : 0);
		sprintf(timeText, (up ? "%02d:%02d:%02d" : "%02d %02d %02d"),
				stimestructureget.Hours, stimestructureget.Minutes,
				stimestructureget.Seconds);
		TEXT_SetText(hTime, timeText);

		sprintf(timeText, "%d/%d/%d", sdatestructureget.Date,
				sdatestructureget.Month, sdatestructureget.Year + 2000);
		TEXT_SetText(hDate, timeText);
	}
}

/**
  * @brief Update µSD card icon on the status bar
  * @param None
  * @retval None
  */
void Menu_StatusBarUpdateSdCardIcon() {
	if (hThisWindow) {
		WM_HWIN hItem = WM_GetDialogItem(hThisWindow, ID_STATUS_BAR_ICON_SD_CARD);
		if (!hItem) {
			return;
		}
		if (BSP_SD_IsDetected() == SD_PRESENT) {
			WM_ShowWin(hItem);
		} else {
			WM_HideWin(hItem);
		}
	}
}

/**
  * @brief Update temperature value on status bar
  * @param None
  * @retval None
  */
void Menu_StatusBarUpdateTemperatureReading() {
	if(!hThisWindow) {
		return;
	}
	char tempText[10];
	sprintf(tempText, "%d %s", temperatureReading, TEMPERATURE_UNIT);

	WM_HWIN hItem = WM_GetDialogItem(hThisWindow, ID_TEXT_TEMPERATURE);
	TEXT_SetText(hItem, tempText);
}

/* End of file */
