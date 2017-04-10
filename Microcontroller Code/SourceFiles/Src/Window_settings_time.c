/* Includes */
#include "DIALOG.h"
#include "main.h"
#include "UI_resources.h"
#include "WM.h"

/* Defines */
#define ID_WINDOW_0 (GUI_ID_USER + 0x00)
#define ID_SPINBOX_HOURS (GUI_ID_USER + 0x05)
#define ID_SPINBOX_MINUTES (GUI_ID_USER + 0x06)
#define ID_IMAGE_VIEW_SAVE (GUI_ID_USER + 0x07)
#define ID_TEXT_SAVE  (GUI_ID_USER + 0x08)
#define ID_IMAGE_VIEW_RETURN  (GUI_ID_USER + 0x09)
#define ID_TEXT_RETURN  (GUI_ID_USER + 0x10)

/* Function prototypes */
static void SettingsTime_DialogCallback(WM_MESSAGE * pMsg);

/* Static data */
static WM_HWIN hThisWindow;

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
		{ WINDOW_CreateIndirect, NULL, ID_WINDOW_0, 0, 0, 480, 272, 0, 0x0, 0 },
		{ SPINBOX_CreateIndirect, NULL, ID_SPINBOX_HOURS, 80, 110, 130, 80, 0, 0x0, 0 },
		{ SPINBOX_CreateIndirect, NULL, ID_SPINBOX_MINUTES, 240, 110, 130, 80, 0, 0x0, 0 },
		{ IMAGE_CreateIndirect, NULL, ID_IMAGE_VIEW_SAVE, 10, 216, 48, 48, 0, 0, 0 },
		{ TEXT_CreateIndirect, WINDOW_TIME_SETTINGS_BUTTON_SAVE_TITLE, ID_TEXT_SAVE, 65, 230, 80, 20, 0, 0x0, 0 },
		{ IMAGE_CreateIndirect, NULL, ID_IMAGE_VIEW_RETURN, 10, 5, 48, 48, 0, 0, 0 },
		{ TEXT_CreateIndirect, WINDOW_TIME_SETTINGS_WINDOW_TITLE, ID_TEXT_RETURN, 65, 20, 80, 20, 0, 0x0, 0 }, };

/* Static code */

/**
 * @brief  Handle callback events from parent window
 * @param  pMsg: callback message structure from parent window
 * @retval None
 */
static void SettingsTime_DialogCallback(WM_MESSAGE * pMsg) {
	WM_HWIN hItem;
	int NCode;
	int Id;
	int hours;
	int minutes;
	/* Get current RTC time */
	RTC_TimeTypeDef stimestructureget = BSP_RTC_GetTime();

	switch (pMsg->MsgId) {
	case WM_INIT_DIALOG:

		hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_HOURS);
		SPINBOX_SetFont(hItem, TIME_SETTINGS_HOUR_FONT);
		SPINBOX_SetRange(hItem, 0, HOUR_RANGE_MAX);
		SPINBOX_SetValue(hItem, stimestructureget.Hours);

		hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_MINUTES);
		SPINBOX_SetFont(hItem, TIME_SETTINGS_MINUTE_FONT);
		SPINBOX_SetRange(hItem, 0, MINUTE_RANGE_MAX);
		SPINBOX_SetValue(hItem, stimestructureget.Minutes);

		/* Initialization of return icon */
		hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_VIEW_RETURN);
		IMAGE_SetBitmap(hItem, &bmBITMAP_return);

		/* Initialization of return icon title */
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_RETURN);
		TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
		TEXT_SetFont(hItem, WINDOW_TITLE_FONT);

		/* Initialization of save icon */
		hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_VIEW_SAVE);
		IMAGE_SetBitmap(hItem, &bmBITMAP_save);

		/* Initialization of save icon title */
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_SAVE);
		TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
		TEXT_SetFont(hItem, TIME_SETTINGS_SAVE_ICON_TEXT_FONT);

		break;
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch (Id) {

		case ID_IMAGE_VIEW_RETURN:
			switch (NCode) {
			case WM_NOTIFICATION_CLICKED:
				break;
			case WM_NOTIFICATION_RELEASED:
				WM_HideWindow(hThisWindow);
				break;
			}
			break;
		case ID_IMAGE_VIEW_SAVE:
			switch (NCode) {
			case WM_NOTIFICATION_RELEASED:
				hours = SPINBOX_GetValue(WM_GetDialogItem(hThisWindow, ID_SPINBOX_HOURS));
				minutes = SPINBOX_GetValue(WM_GetDialogItem(hThisWindow, ID_SPINBOX_MINUTES));
				BSP_RTC_SetTime(hours, minutes, 0);
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
  * @brief Create time settings window
  * @param None
  * @retval Handle pointing to time settings window
  */
WM_HWIN SettingsTime_CreateSettingsTimeWindow(void) {
	hThisWindow = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), SettingsTime_DialogCallback, WM_HBKWIN, 0, 0);
	return hThisWindow;
}

/* End of file */
