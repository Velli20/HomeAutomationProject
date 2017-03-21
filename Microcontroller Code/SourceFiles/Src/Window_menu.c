/* Includes */
#include "DIALOG.h"
#include "main.h"
#include "BITMAPS.h"

/* Defines */
#define ID_WINDOW_MENU  			(GUI_ID_USER + 0x00)
#define ID_ICONVIEW_MENU  			(GUI_ID_USER + 0x01)
#define ID_STATUS_BAR_ICON_SD_CARD  (GUI_ID_USER + 0x02)
#define ID_TEXT_TIME  				(GUI_ID_USER + 0x03)
#define ID_TEXT_DATE  				(GUI_ID_USER + 0x04)
#define ID_TEXT_TEMPERATURE  		(GUI_ID_USER + 0x05)

/* Private typedef */
typedef struct {
	const GUI_BITMAP * pBitmap;
	const char * pText;
} BITMAP_ITEM;

/* Private function prototypes */
static void Menu_setItemSelected(WM_HWIN hItem, int selected);
static void Menu_UpdateTemperatureReading(void);

/* Function prototypes */
WM_HWIN Menu_CreateMenuWindow(void);
void Menu_clockUpdateCallback(void);

/* Static data */
static const BITMAP_ITEM mBitmaps[] = { { &bmBITMAP_lights, "Valaistus" }, {
		&bmBITMAP_temperature, "Lämpötila" }, { &bmBITMAP_console, "Komennot" },
		{ &bmBITMAP_settings, "Asetukset" }, };

static const BITMAP_ITEM mBitmapsSelected[] = { { &bmBITMAP_lights_selected, "Valaistus" }, {
		&bmBITMAP_temperature_selected, "Lämpötila" }, { &bmBITMAP_console_selected, "Komennot" },
		{ &bmBITMAP_settings_selected, "Asetukset" }, };

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
		{ WINDOW_CreateIndirect, "Window", ID_WINDOW_MENU, 0, 0, 480, 272, 0, 0x0, 0 },
		{ ICONVIEW_CreateIndirect, "Iconview", ID_ICONVIEW_MENU, 0, 20, 480, 272, 0, 0x00600060, 0 },
		{ IMAGE_CreateIndirect, "Image", ID_STATUS_BAR_ICON_SD_CARD, 63, 0, 26, 27, 0, 0, 0 },
		{ TEXT_CreateIndirect, "", ID_TEXT_TIME, 400, 0, 80, 20, 0, 0x0, 0 },
		{ TEXT_CreateIndirect, "", ID_TEXT_DATE, 320, 0, 80, 20, 0, 0x0, 0 },
		{ TEXT_CreateIndirect, "- °C", ID_TEXT_TEMPERATURE, 0, 0, 80, 20, 0, 0x0, 0 },};


/* Variables */
WM_HWIN hThisWindow;
extern __IO ITStatus SdCardInserted;

/* Variable to store latest reading of the temperature sensor declared in main.c */
extern uint8_t mTemperature;




/* Private code */

static void _cbDialog(WM_MESSAGE * pMsg) {
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
		TEXT_SetFont(hItem, GUI_FONT_13B_1);

		/* Initialization of status bar date text */

		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_DATE);
		TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		TEXT_SetFont(hItem, GUI_FONT_13B_1);

		/* Initialization of status bar temperature text */


		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_TEMPERATURE);
		TEXT_SetFont(hItem, GUI_FONT_13B_1);
		TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		Menu_UpdateTemperatureReading();


		/* Initialization of menu */

		hItem = WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_MENU);
		ICONVIEW_SetSpace(hItem, GUI_COORD_X, 24);
		ICONVIEW_SetSpace(hItem, GUI_COORD_Y, 10);
		ICONVIEW_SetFont(hItem, GUI_FONT_13B_1);
		ICONVIEW_SetSel(hItem, -1);

		int i;
		for (i = 0; i < GUI_COUNTOF(mBitmaps); i++) {
			ICONVIEW_AddBitmapItem(hItem, mBitmaps[i].pBitmap,mBitmaps[i].pText);
			ICONVIEW_SetTextColor(hItem, i, 0x000000);
		}
		Main_register_clockUpdateCallback(Menu_clockUpdateCallback);
		break;
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		hItem = WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_MENU);
		NCode = pMsg->Data.v;

		switch (Id) {
		case ID_ICONVIEW_MENU:
			switch (NCode) {
			case WM_NOTIFICATION_CLICKED:
				Menu_setItemSelected(hItem, 1);
				break;
			case WM_NOTIFICATION_RELEASED:
				Menu_setItemSelected(hItem, 0);
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
					WM_ShowWindow(Commands_CreateCommandsWindow());
					break;
				case 3:
					WM_ShowWindow(Settings_CreateSettingsWindow());
					break;
				}
				break;
			case WM_NOTIFICATION_MOVED_OUT:
				break;
			case WM_NOTIFICATION_SCROLL_CHANGED:
				break;
			case WM_NOTIFICATION_SEL_CHANGED:
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
 * @brief  Change menu item
 * @param  hItem: Handle of ICONVIEW widget
 * @param  selected: 1 if item is clicked or 0 if item is released
 */
static void Menu_setItemSelected(WM_HWIN hItem, int selected) {
	int selection = ICONVIEW_GetSel(hItem);
	if(selection < 0 || selection > 3 || hItem == NULL) {
		return;
	}

	if(selected) {
		ICONVIEW_SetBitmapItem(hItem, selection, mBitmapsSelected[selection].pBitmap);
	} else {
		ICONVIEW_SetBitmapItem(hItem, selection, mBitmaps[selection].pBitmap);
	}


}

static void Menu_UpdateTemperatureReading() {
	char tempText[10];
	sprintf(tempText, "%d °C", mTemperature);

	WM_HWIN hItem = WM_GetDialogItem(hThisWindow, ID_TEXT_TEMPERATURE);
	TEXT_SetText(hItem, tempText);
}

/* Public code */

/**
 * @brief  uSD card inserted or removed callback in non blocking mode.
 *         Set sd card icon visible/invisible depending on whether
 *         a sd card is inserted or not
 * @param  htim: SD_HandleTypeDef handle
 * @retval None
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == SD_DETECT_PIN && hThisWindow != NULL) {
		SdCardInserted = SET;
		WM_HWIN hItem = WM_GetDialogItem(hThisWindow, ID_STATUS_BAR_ICON_SD_CARD);
		if(hItem == NULL) {
			return;
		}
		if (BSP_SD_IsDetected() == SD_PRESENT) {
			WM_ShowWin(hItem);
		} else {
			WM_HideWin(hItem);
		}
	}
}

WM_HWIN Menu_CreateMenuWindow(void) {

	hThisWindow = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
	return hThisWindow;
}


/**
  * @brief RTC clock update callbacks
  * @param None
  * @retval None
  */
void Menu_clockUpdateCallback(void) {
	int up = 0;
	uint8_t aShowTime[50] = { 0 };

	RTC_TimeTypeDef stimestructureget;
	RTC_DateTypeDef sdatestructureget;

	/* Get the RTC current Time */
	stimestructureget = BSP_RTC_GetTime();
	sdatestructureget = BSP_RTC_GetDate();

	WM_HWIN hTime = WM_GetDialogItem(hThisWindow, ID_TEXT_TIME);
	WM_HWIN hDate = WM_GetDialogItem(hThisWindow, ID_TEXT_DATE);

	Menu_UpdateTemperatureReading();

	if (NULL != hThisWindow) {
		up = (stimestructureget.Seconds %2 == 0? 1 : 0);
		sprintf(aShowTime, (up ? "%02d:%02d:%02d" : "%02d %02d %02d"),
				stimestructureget.Hours, stimestructureget.Minutes,
				stimestructureget.Seconds);
		TEXT_SetText(hTime, aShowTime);

		sprintf(aShowTime, "%d/%d/%d", sdatestructureget.Date,
				sdatestructureget.Month, sdatestructureget.Year + 2000);
		TEXT_SetText(hDate, aShowTime);
	}

}

/* End of file */
