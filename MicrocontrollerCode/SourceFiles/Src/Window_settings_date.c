// Includes

#include "DIALOG.h"
#include "main.h"
#include "real_time_clock.h"
#include "UI_resources.h"
#include "WM.h"
#include "GregorianCalendar.h"

// Defines

#define ID_WINDOW_SETTINGS_DATE (GUI_ID_USER + 0x00)
#define ID_SPINBOX_DAY          (GUI_ID_USER + 0x05)
#define ID_SPINBOX_MONTH        (GUI_ID_USER + 0x06)
#define ID_SPINBOX_YEAR         (GUI_ID_USER + 0x04)
#define ID_IMAGE_VIEW_SAVE_DATE (GUI_ID_USER + 0x07)
#define ID_TEXT_SAVE            (GUI_ID_USER + 0x08)
#define ID_IMAGE_VIEW_RETURN    (GUI_ID_USER + 0x09)
#define ID_TEXT_RETURN          (GUI_ID_USER + 0x10)

static WM_HWIN hThisWindow;

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
		{ WINDOW_CreateIndirect,  0,                                      ID_WINDOW_SETTINGS_DATE, 0,   0,   480, 272, 0, 0x0, 0 },
		{ SPINBOX_CreateIndirect, 0,                                      ID_SPINBOX_DAY,          28,  110, 130, 80,  0, 0x0, 0 },
		{ SPINBOX_CreateIndirect, 0,                                      ID_SPINBOX_MONTH,        176, 110, 130, 80,  0, 0x0, 0 },
		{ SPINBOX_CreateIndirect, 0,                                      ID_SPINBOX_YEAR,         324, 110, 130, 80,  0, 0x0, 0 },
		{ IMAGE_CreateIndirect,   0,                                      ID_IMAGE_VIEW_SAVE_DATE, 10,  216, 48,  48,  0, 0,   0 },
		{ TEXT_CreateIndirect,    WINDOW_DATE_SETTINGS_BUTTON_SAVE_TITLE, ID_TEXT_SAVE,            65,  230, 80,  20,  0, 0x0, 0 },
		{ IMAGE_CreateIndirect,   0,                                      ID_IMAGE_VIEW_RETURN,    10,  5,   48,  48,  0, 0,   0 },
		{ TEXT_CreateIndirect,    WINDOW_DATE_SETTINGS_WINDOW_TITLE,      ID_TEXT_RETURN,          65,  20,  80,  20,  0, 0x0, 0 }, };

// SettingsDate_DialogCallback

static void SettingsDate_DialogCallback(WM_MESSAGE* pMsg) 
{
	WM_HWIN         widget;
	RTC_DateTypeDef rtc_date;
	int             day;
	int             month;
	int             year;

	switch (pMsg->MsgId) 
	{

	case WM_INIT_DIALOG:

	    // Get current RTC time.

	    rtc_date= BSP_RTC_GetDate();

		// Initialization of day of month selector.

		widget= WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_DAY);
		SPINBOX_SetFont(widget, DATE_SETTINGS_DAY_FONT);
		SPINBOX_SetRange(widget, 1, GregorianCalendar_GetCountOfDayInMonth(rtc_date.Year, rtc_date.Month));
		SPINBOX_SetValue(widget, rtc_date.Date);

		// Initialization of month selector.

		widget= WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_MONTH);
		SPINBOX_SetFont(widget, DATE_SETTINGS_MONTH_FONT);
		SPINBOX_SetRange(widget, 1, 12);
		SPINBOX_SetValue(widget, rtc_date.Month);

		// Initialization of year selector.

		widget= WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_YEAR);
		SPINBOX_SetFont(widget, DATE_SETTINGS_YEAR_FONT);
		SPINBOX_SetRange(widget, YEAR_RANGE_MIN, YEAR_RANGE_MAX);
		SPINBOX_SetValue(widget, rtc_date.Year + 1999);

		// Initialization of return icon.

		widget= WM_GetDialogItem(pMsg->hWin, ID_IMAGE_VIEW_RETURN);
		IMAGE_SetBitmap(widget, &bmBITMAP_return);

		// Initialization of return icon title.

		widget= WM_GetDialogItem(pMsg->hWin, ID_TEXT_RETURN);
		TEXT_SetTextAlign(widget, GUI_TA_LEFT | GUI_TA_VCENTER);
		TEXT_SetFont(widget, WINDOW_TITLE_FONT);

		// Initialization of save icon.

		widget= WM_GetDialogItem(pMsg->hWin, ID_IMAGE_VIEW_SAVE_DATE);
		IMAGE_SetBitmap(widget, &bmBITMAP_save);

		// Initialization of save icon title.

		widget= WM_GetDialogItem(pMsg->hWin, ID_TEXT_SAVE);
		TEXT_SetTextAlign(widget, GUI_TA_LEFT | GUI_TA_VCENTER);
		TEXT_SetFont(widget, DATE_SETTINGS_SAVE_ICON_TEXT_FONT);

		break;
	case WM_NOTIFY_PARENT:

		switch ( WM_GetId(pMsg->hWinSrc) ) 
		{

		case ID_IMAGE_VIEW_RETURN:
			if  ( pMsg->Data.v == WM_NOTIFICATION_RELEASED ) 
				WM_HideWindow(hThisWindow);
		
			break;

		case ID_IMAGE_VIEW_SAVE_DATE:
			if ( pMsg->Data.v == WM_NOTIFICATION_RELEASED ) 
			{
				day=   SPINBOX_GetValue(WM_GetDialogItem(hThisWindow, ID_SPINBOX_DAY));
				month= SPINBOX_GetValue(WM_GetDialogItem(hThisWindow, ID_SPINBOX_MONTH));
				year=  SPINBOX_GetValue(WM_GetDialogItem(hThisWindow, ID_SPINBOX_YEAR));
				BSP_RTC_SetDate((uint8_t)day, (uint8_t)month, (uint16_t)year);
			}
			break;
		}
		break;

	default:
		WM_DefaultProc(pMsg);
		break;
	}
}

// SettingsDate_CreateSettingsDateWindow

WM_HWIN SettingsDate_CreateSettingsDateWindow(void) 
{
	hThisWindow= GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), SettingsDate_DialogCallback, WM_HBKWIN, 0, 0);
	return(hThisWindow);
}

