// Includes.

#include "DIALOG.h"
#include "main.h"
#include "real_time_clock.h"
#include "UI_resources.h"
#include "WM.h"

// Defines.

#define ID_WINDOW_0           (GUI_ID_USER + 0x00)
#define ID_SPINBOX_HOURS      (GUI_ID_USER + 0x05)
#define ID_SPINBOX_MINUTES    (GUI_ID_USER + 0x06)
#define ID_IMAGE_VIEW_SAVE    (GUI_ID_USER + 0x07)
#define ID_TEXT_SAVE          (GUI_ID_USER + 0x08)
#define ID_IMAGE_VIEW_RETURN  (GUI_ID_USER + 0x09)
#define ID_TEXT_RETURN        (GUI_ID_USER + 0x10)

static WM_HWIN hThisWindow;

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
		{ WINDOW_CreateIndirect,  0,                                      ID_WINDOW_0,          0,   0,   480, 272, 0, 0x0, 0 },
		{ SPINBOX_CreateIndirect, 0,                                      ID_SPINBOX_HOURS,     80,  110, 130, 80,  0, 0x0, 0 },
		{ SPINBOX_CreateIndirect, 0,                                      ID_SPINBOX_MINUTES,   240, 110, 130, 80,  0, 0x0, 0 },
		{ IMAGE_CreateIndirect,   0,                                      ID_IMAGE_VIEW_SAVE,   10,  216, 48,  48,  0, 0,   0 },
		{ TEXT_CreateIndirect,    WINDOW_TIME_SETTINGS_BUTTON_SAVE_TITLE, ID_TEXT_SAVE,         65,  230, 80,  20,  0, 0x0, 0 },
		{ IMAGE_CreateIndirect,   0,                                      ID_IMAGE_VIEW_RETURN, 10,  5,   48,  48,  0, 0,   0 },
		{ TEXT_CreateIndirect,    WINDOW_TIME_SETTINGS_WINDOW_TITLE,      ID_TEXT_RETURN,       65,  20,  80,  20,  0, 0x0, 0 }, };


// SettingsTime_DialogCallback

static void SettingsTime_DialogCallback(WM_MESSAGE* pMsg) 
{
	WM_HWIN         widget;
	RTC_TimeTypeDef rtc_time;
	int             hours;
	int             minutes;

	switch ( pMsg->MsgId ) 
	{

	case WM_INIT_DIALOG:

	    // Get current RTC time.

	    rtc_time= BSP_RTC_GetTime();

		// Initialization of hour selector.

		widget= WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_HOURS);
		SPINBOX_SetFont(widget, TIME_SETTINGS_HOUR_FONT);
		SPINBOX_SetRange(widget, 0, HOUR_RANGE_MAX);
		SPINBOX_SetValue(widget, rtc_time.Hours);

		// Initialization of minute selector.

		widget= WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_MINUTES);
		SPINBOX_SetFont(widget, TIME_SETTINGS_MINUTE_FONT);
		SPINBOX_SetRange(widget, 0, MINUTE_RANGE_MAX);
		SPINBOX_SetValue(widget, rtc_time.Minutes);

		// Initialization of return icon.

		widget= WM_GetDialogItem(pMsg->hWin, ID_IMAGE_VIEW_RETURN);
		IMAGE_SetBitmap(widget, &bmBITMAP_return);

		// Initialization of return icon title.

		widget= WM_GetDialogItem(pMsg->hWin, ID_TEXT_RETURN);
		TEXT_SetTextAlign(widget, GUI_TA_LEFT | GUI_TA_VCENTER);
		TEXT_SetFont(widget, WINDOW_TITLE_FONT);

		// Initialization of save icon.

		widget= WM_GetDialogItem(pMsg->hWin, ID_IMAGE_VIEW_SAVE);
		IMAGE_SetBitmap(widget, &bmBITMAP_save);

		// Initialization of save icon title.

		widget= WM_GetDialogItem(pMsg->hWin, ID_TEXT_SAVE);
		TEXT_SetTextAlign(widget, GUI_TA_LEFT | GUI_TA_VCENTER);
		TEXT_SetFont(widget, TIME_SETTINGS_SAVE_ICON_TEXT_FONT);

		break;
	case WM_NOTIFY_PARENT:
		
		switch ( WM_GetId(pMsg->hWinSrc) ) 
		{

		case ID_IMAGE_VIEW_RETURN:

			if ( pMsg->Data.v == WM_NOTIFICATION_RELEASED ) 
				WM_HideWindow(hThisWindow);

			break;
		case ID_IMAGE_VIEW_SAVE:

			if ( pMsg->Data.v == WM_NOTIFICATION_RELEASED ) 
			{
				hours=   SPINBOX_GetValue(WM_GetDialogItem(hThisWindow, ID_SPINBOX_HOURS));
				minutes= SPINBOX_GetValue(WM_GetDialogItem(hThisWindow, ID_SPINBOX_MINUTES));
				BSP_RTC_SetTime(hours, minutes, 0);
			}
			break;
		}
		break;

	default:
		WM_DefaultProc(pMsg);
		break;
	}
}

// SettingsTime_CreateSettingsTimeWindow

WM_HWIN SettingsTime_CreateSettingsTimeWindow(void) 
{
	hThisWindow = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), SettingsTime_DialogCallback, WM_HBKWIN, 0, 0);
	return(hThisWindow);
}
