/* Includes */
#include "DIALOG.h"
#include "main.h"
#include "BITMAPS.h"
#include "WM.h"
/* Defines */
#define ID_WINDOW_0 (GUI_ID_USER + 0x00)
#define ID_SPINBOX_HOURS (GUI_ID_USER + 0x05)
#define ID_SPINBOX_MINUTES (GUI_ID_USER + 0x06)
#define ID_IMAGE_VIEW_SAVE (GUI_ID_USER + 0x07)
#define ID_TEXT_SAVE  (GUI_ID_USER + 0x08)
#define ID_IMAGE_VIEW_RETURN  (GUI_ID_USER + 0x09)
#define ID_TEXT_RETURN  (GUI_ID_USER + 0x10)

#define SPINBOX_HOUR_RANGE_24H_MAX 		23
#define SPINBOX_HOUR_RANGE_MIN			-1

#define SPINBOX_MINUTE_RANGE_MAX 		60
#define SPINBOX_MINUTE_RANGE_MIN		-1

/* Funtion prototypes */WM_HWIN SettingsTime_CreateSettingsTimeWindow(void);

/* Static data */
static WM_HWIN hThisWindow;

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
		{ WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 480, 272, 0, 0x0, 0 },
		{ SPINBOX_CreateIndirect, "Spinbox", ID_SPINBOX_HOURS, 80, 110, 130, 80, 0, 0x0, 0 },
		{ SPINBOX_CreateIndirect, "Spinbox", ID_SPINBOX_MINUTES, 240, 110, 130, 80, 0, 0x0, 0 },
		{ IMAGE_CreateIndirect, "Save", ID_IMAGE_VIEW_SAVE, 10, 216, 48, 48, 0, 0, 0 },
		{ TEXT_CreateIndirect, "Tallenna", ID_TEXT_SAVE, 65, 230, 80, 20, 0, 0x0, 0 },
		{ IMAGE_CreateIndirect, "Return", ID_IMAGE_VIEW_RETURN, 10, 10, 48, 48, 0, 0, 0 },
		{ TEXT_CreateIndirect, "Aseta aika", ID_TEXT_RETURN, 65, 24, 80, 20, 0, 0x0, 0 }, };

/* Static code */

static void _cbDialog(WM_MESSAGE * pMsg) {
	const void * pData;
	WM_HWIN hItem;
	U32 FileSize;
	int NCode;
	int Id;
	int valueHours = 0;
	int valueMinutes = 0;

	switch (pMsg->MsgId) {
	case WM_INIT_DIALOG:

		hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_HOURS);
		SPINBOX_SetFont(hItem, GUI_FONT_16B_1);
		SPINBOX_SetRange(hItem, SPINBOX_HOUR_RANGE_MIN, SPINBOX_HOUR_RANGE_24H_MAX);

		hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_MINUTES);
		SPINBOX_SetFont(hItem, GUI_FONT_13B_ASCII);
		SPINBOX_SetRange(hItem, SPINBOX_MINUTE_RANGE_MIN, SPINBOX_MINUTE_RANGE_MAX);

		/* Initialization of return icon */
		hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_VIEW_RETURN);
		IMAGE_SetBitmap(hItem, &bmBITMAP_return);

		/* Initialization of return icon title */
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_RETURN);
		TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
		TEXT_SetFont(hItem, GUI_FONT_13B_1);

		/* Initialization of save icon */
		hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_VIEW_SAVE);
		IMAGE_SetBitmap(hItem, &bmBITMAP_save);

		/* Initialization of save icon title */
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_SAVE);
		TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
		TEXT_SetFont(hItem, GUI_FONT_13B_1);

		break;
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch (Id) {
		case ID_SPINBOX_HOURS:
			switch (NCode) {
			case WM_NOTIFICATION_CLICKED:
				break;
			case WM_NOTIFICATION_RELEASED:
				break;
			case WM_NOTIFICATION_MOVED_OUT:
				break;
			case WM_NOTIFICATION_VALUE_CHANGED:
				hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_HOURS);

				if (valueHours == 0 && SPINBOX_GetValue(hItem) == SPINBOX_HOUR_RANGE_MIN) {
					valueHours = SPINBOX_HOUR_RANGE_24H_MAX;
					SPINBOX_SetValue(hItem, SPINBOX_HOUR_RANGE_24H_MAX);
				} else {
					valueHours = SPINBOX_GetValue(hItem);
				}
				break;

			}
			break;
		case ID_SPINBOX_MINUTES:
			switch (NCode) {
			case WM_NOTIFICATION_CLICKED:
				break;
			case WM_NOTIFICATION_RELEASED:
				break;
			case WM_NOTIFICATION_MOVED_OUT:
				break;
			case WM_NOTIFICATION_VALUE_CHANGED:
				hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_MINUTES);

				if(valueMinutes == 0 && SPINBOX_GetValue(hItem) == SPINBOX_MINUTE_RANGE_MIN) {
					valueMinutes = (SPINBOX_MINUTE_RANGE_MAX -1);
					SPINBOX_SetValue(hItem, (SPINBOX_MINUTE_RANGE_MAX -1));

				} else if(valueMinutes == 59 && SPINBOX_GetValue(hItem) == SPINBOX_MINUTE_RANGE_MAX) {
					valueMinutes = (SPINBOX_MINUTE_RANGE_MIN +1);
					SPINBOX_SetValue(hItem, (SPINBOX_MINUTE_RANGE_MIN +1));
				} else {
					valueMinutes = SPINBOX_GetValue(hItem);
				}

				break;
			}
			break;
		case ID_IMAGE_VIEW_RETURN:
			switch (NCode) {
			case WM_NOTIFICATION_CLICKED:
				break;
			case WM_NOTIFICATION_RELEASED:
				WM_HideWindow(hThisWindow);
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

WM_HWIN SettingsTime_CreateSettingsTimeWindow(void) {
	hThisWindow = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
	return hThisWindow;
}

/* End of file */
