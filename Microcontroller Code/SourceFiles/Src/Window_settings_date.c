/* Includes */
#include "DIALOG.h"
#include "main.h"
#include "UI_resources.h"
#include "WM.h"
#include "GregorianCalendar.h"

/* Defines */
#define ID_WINDOW_SETTINGS_DATE (GUI_ID_USER + 0x00)
#define ID_SPINBOX_DAY (GUI_ID_USER + 0x05)
#define ID_SPINBOX_MONTH (GUI_ID_USER + 0x06)
#define ID_SPINBOX_YEAR (GUI_ID_USER + 0x04)
#define ID_IMAGE_VIEW_SAVE_DATE (GUI_ID_USER + 0x07)
#define ID_TEXT_SAVE  (GUI_ID_USER + 0x08)
#define ID_IMAGE_VIEW_RETURN  (GUI_ID_USER + 0x09)
#define ID_TEXT_RETURN  (GUI_ID_USER + 0x10)

/* Function prototypes */
static void SettingsDate_DialogCallback(WM_MESSAGE * pMsg);

/* Static data */
static WM_HWIN hThisWindow;

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
		{ WINDOW_CreateIndirect, NULL, ID_WINDOW_SETTINGS_DATE, 0, 0, 480, 272, 0, 0x0, 0 },
		{ SPINBOX_CreateIndirect, NULL, ID_SPINBOX_DAY, 28, 110, 130, 80, 0, 0x0, 0 },
		{ SPINBOX_CreateIndirect, NULL, ID_SPINBOX_MONTH, 176, 110, 130, 80, 0, 0x0, 0 },
		{ SPINBOX_CreateIndirect, NULL, ID_SPINBOX_YEAR, 324, 110, 130, 80, 0, 0x0, 0 },
		{ IMAGE_CreateIndirect, NULL, ID_IMAGE_VIEW_SAVE_DATE, 10, 216, 48, 48, 0, 0, 0 },
		{ TEXT_CreateIndirect, WINDOW_DATE_SETTINGS_BUTTON_SAVE_TITLE, ID_TEXT_SAVE, 65, 230, 80, 20, 0, 0x0, 0 },
		{ IMAGE_CreateIndirect, NULL, ID_IMAGE_VIEW_RETURN, 10, 5, 48, 48, 0, 0, 0 },
		{ TEXT_CreateIndirect, WINDOW_DATE_SETTINGS_WINDOW_TITLE, ID_TEXT_RETURN, 65, 20, 80, 20, 0, 0x0, 0 }, };

/* Static code */

/**
 * @brief  Handle callback events from parent window
 * @param  pMsg: callback message structure from parent window
 * @retval None
 */
static void SettingsDate_DialogCallback(WM_MESSAGE * pMsg) {
	WM_HWIN hItem;
	int NCode;
	int Id;

	int day;
	int month;
	int year;

	/* Get current RTC time */
	RTC_DateTypeDef sdatetructureget = BSP_RTC_GetDate();

	switch (pMsg->MsgId) {
	case WM_INIT_DIALOG:
		/* Initialization of day SPINBOX */
		hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_DAY);
		SPINBOX_SetFont(hItem, DATE_SETTINGS_DAY_FONT);
		SPINBOX_SetRange(hItem, 1, GregorianCalendar_GetCountOfDayInMonth(sdatetructureget.Year, sdatetructureget.Month));
		SPINBOX_SetValue(hItem, sdatetructureget.Date);

		/* Initialization of month SPINBOX */
		hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_MONTH);
		SPINBOX_SetFont(hItem, DATE_SETTINGS_MONTH_FONT);
		SPINBOX_SetRange(hItem, 1, 12);
		SPINBOX_SetValue(hItem, sdatetructureget.Month);

		/* Initialization of year SPINBOX */
		hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_YEAR);
		SPINBOX_SetFont(hItem, DATE_SETTINGS_YEAR_FONT);
		SPINBOX_SetRange(hItem, YEAR_RANGE_MIN, YEAR_RANGE_MAX);
		SPINBOX_SetValue(hItem, sdatetructureget.Year + 2000);

		/* Initialization of return icon */
		hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_VIEW_RETURN);
		IMAGE_SetBitmap(hItem, &bmBITMAP_return);

		/* Initialization of return icon title */
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_RETURN);
		TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
		TEXT_SetFont(hItem, WINDOW_TITLE_FONT);

		/* Initialization of save icon */
		hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_VIEW_SAVE_DATE);
		IMAGE_SetBitmap(hItem, &bmBITMAP_save);

		/* Initialization of save icon title */
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_SAVE);
		TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
		TEXT_SetFont(hItem, DATE_SETTINGS_SAVE_ICON_TEXT_FONT);

		break;
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch (Id) {

		case ID_IMAGE_VIEW_RETURN:
			switch (NCode) {
			case WM_NOTIFICATION_RELEASED:
				WM_HideWindow(hThisWindow);
				break;
			}
			break;
		case ID_IMAGE_VIEW_SAVE_DATE:
			switch (NCode) {
			case WM_NOTIFICATION_RELEASED:
				day = SPINBOX_GetValue(WM_GetDialogItem(hThisWindow, ID_SPINBOX_DAY));
				month = SPINBOX_GetValue(WM_GetDialogItem(hThisWindow, ID_SPINBOX_MONTH));
				year = SPINBOX_GetValue(WM_GetDialogItem(hThisWindow, ID_SPINBOX_YEAR));
				BSP_RTC_SetDate(day, month, year-2000);
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
  * @brief Create DATE settings window
  * @param None
  * @retval Handle pointing to time settings window
  */
WM_HWIN SettingsDate_CreateSettingsDateWindow(void) {
	hThisWindow = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), SettingsDate_DialogCallback, WM_HBKWIN, 0, 0);
	return hThisWindow;
}

/* End of file */
