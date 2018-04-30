// Includes

#include "DIALOG.h"
#include "main.h"
#include "real_time_clock.h"
#include "UI_resources.h"
#include "Log.h"

// Defines

#define ID_WINDOW_MENU  			(GUI_ID_USER + 0x00)
#define ID_ICONVIEW_MENU  			(GUI_ID_USER + 0x01)
#define ID_STATUS_BAR_ICON_SD_CARD  (GUI_ID_USER + 0x02)
#define ID_TEXT_TIME  				(GUI_ID_USER + 0x03)
#define ID_TEXT_DATE  				(GUI_ID_USER + 0x04)
#define ID_TEXT_TEMPERATURE  		(GUI_ID_USER + 0x05)

static const BITMAP_ITEM mBitmaps[]= {
		{ &bmBITMAP_lights,      &bmBITMAP_lights_selected,      WINDOW_MENU_MENU_ITEM_LIGHTS },
		{ &bmBITMAP_temperature, &bmBITMAP_temperature_selected, WINDOW_MENU_MENU_ITEM_TMEPERATURE },
		{ &bmBITMAP_console,     &bmBITMAP_console_selected,     WINDOW_MENU_MENU_ITEM_COMMANDS },
		{ &bmBITMAP_settings,    &bmBITMAP_settings_selected,    WINDOW_MENU_MENU_ITEM_SETTINGS }, };

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[]= {
		{ WINDOW_CreateIndirect,   0,                ID_WINDOW_MENU,             0,   0,  480, 272, 0, 0x0,        0 },
		{ ICONVIEW_CreateIndirect, 0,                ID_ICONVIEW_MENU,           0,   20, 480, 272, 0, 0x00600060, 0 },
		{ IMAGE_CreateIndirect,    0,                ID_STATUS_BAR_ICON_SD_CARD, 63,  0,  26,  27,  0, 0,          0 },
		{ TEXT_CreateIndirect,     0,                ID_TEXT_TIME,               400, 0,  80,  20,  0, 0x0,        0 },
		{ TEXT_CreateIndirect,     0,                ID_TEXT_DATE,               320, 0,  80,  20,  0, 0x0,        0 },
		{ TEXT_CreateIndirect,     TEMPERATURE_UNIT, ID_TEXT_TEMPERATURE,        0,   0,  80,  20,  0, 0x0,        0 },};


static WM_HWIN hThisWindow;

extern uint8_t temperatureReading;
extern APP_DATA* application_data;

// window_menu_set_item_selected

static void window_menu_set_item_selected(WM_HWIN menu_widget, 
                                          uint8_t selected) 
{
	int selection;

	if ( !menu_widget ) 
		return;
	
	selection= ICONVIEW_GetSel(menu_widget);
	if ( selection < 0 || selection > 3 ) 
		return;
	
	// Change menu item icon highlighted/unhighlighted.

	if ( selected ) 
		ICONVIEW_SetBitmapItem(menu_widget, selection, mBitmaps[selection].pBitmapSelected);
	else 
		ICONVIEW_SetBitmapItem(menu_widget, selection, mBitmaps[selection].pBitmap);
}

// window_menu_update_temperature_value

static void window_menu_update_temperature_value(void) 
{
	WM_HWIN temp_text_widget;
	char    temp_text[10];

	if ( !hThisWindow ) 
		return;
	
	// Update temperature reading on the status bar.

	memset(temp_text, 0, 10);
	snprintf(temp_text, 10, "%d %s", temperatureReading, TEMPERATURE_UNIT);

	temp_text_widget= WM_GetDialogItem(hThisWindow, ID_TEXT_TEMPERATURE);
	if ( !temp_text_widget )
	    return;

	TEXT_SetText(temp_text_widget, (const char*)temp_text);
}

// window_menu_update_date_and_time

static void window_menu_update_date_and_time(void) 
{
	RTC_TimeTypeDef hal_rtc_time_struct;
	RTC_DateTypeDef hal_rtc_date_struct;
	WM_HWIN         time_text_widget;
	WM_HWIN         date_text_widget;
	uint8_t         tick;
	char            time_text[20];

	if ( !hThisWindow ) 
		return;

	// Get the RTC time and date.

	hal_rtc_time_struct= BSP_RTC_GetTime();
	hal_rtc_date_struct= BSP_RTC_GetDate();

	time_text_widget= WM_GetDialogItem(hThisWindow, ID_TEXT_TIME);
	date_text_widget= WM_GetDialogItem(hThisWindow, ID_TEXT_DATE);

	if ( !time_text_widget || !date_text_widget )
	    return;

	// Update time and date on the status bar.

	tick= (hal_rtc_time_struct.Seconds % 2 == 0? 1 : 0);

	sprintf(time_text, (tick ? "%02d:%02d:%02d" : "%02d %02d %02d"), hal_rtc_time_struct.Hours, hal_rtc_time_struct.Minutes, hal_rtc_time_struct.Seconds);
	TEXT_SetText(time_text_widget, (const char*)time_text);

	sprintf(time_text, "%d/%d/%d", hal_rtc_date_struct.Date, hal_rtc_date_struct.Month, hal_rtc_date_struct.Year + 2000);
	TEXT_SetText(date_text_widget, (const char*)time_text);
}

// window_menu_update_sd_card_icon

static void window_menu_update_sd_card_icon(void) 
{
	WM_HWIN sd_card_icon;

	if ( !hThisWindow )
	    return;

	// Show or hide sd card icon on the status bar.

	sd_card_icon= WM_GetDialogItem(hThisWindow, ID_STATUS_BAR_ICON_SD_CARD);
	if ( !sd_card_icon ) 
		return;
		
	if ( BSP_SD_IsDetected() == SD_PRESENT ) 
		WM_ShowWin(sd_card_icon);
	else 
		WM_HideWin(sd_card_icon);	
}

// window_menu_event_callback

static void window_menu_event_callback(APP_DATA*          data, 
                                       struct RoomWidget* widget, 
							           uint8_t            event)
{
	// Update status bar time.

	if ( event == EVENT_REAL_TIME_CLOCK_TICK )
	    window_menu_update_date_and_time();

	// Update status bar SD card icon.

	else if ( event == EVENT_SD_CARD_DETECTED )
	    window_menu_update_sd_card_icon();

	// Update temperature reading on the status bar.

	else if ( event == EVENT_TEMPERATURE_UPDATED )
	    window_menu_update_temperature_value();
}

// window_menu_gui_callback

static void window_menu_gui_callback(WM_MESSAGE* pMsg) 
{
	int     NCode;
	int     Id;
	int     selection;
	uint8_t i;
	WM_HWIN hItem;

	switch (pMsg->MsgId) {

	case WM_INIT_DIALOG:

		/* Initialization of SD card status bar icon */
		hItem= WM_GetDialogItem(pMsg->hWin, ID_STATUS_BAR_ICON_SD_CARD);

		if ( BSP_SD_IsDetected() == SD_PRESENT )
			WM_ShowWin(hItem);
		else 
			WM_HideWin(hItem);
		
		IMAGE_SetBitmap(hItem, &bmBITMAP_sd_card);

		/* Initialization of status bar time text */

		hItem= WM_GetDialogItem(pMsg->hWin, ID_TEXT_TIME);
		TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		TEXT_SetFont(hItem, MENU_WINDOW_TIME_TEXT_FONT);

		/* Initialization of status bar date text */

		hItem= WM_GetDialogItem(pMsg->hWin, ID_TEXT_DATE);
		TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		TEXT_SetFont(hItem, MENU_WINDOW_DATE_TEXT_FONT);

		/* Initialization of status bar temperature text */

		hItem= WM_GetDialogItem(pMsg->hWin, ID_TEXT_TEMPERATURE);
		TEXT_SetFont(hItem, MENU_WINDOW_TEMPERATURE_TEXT_FONT);
		TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		window_menu_update_temperature_value();

		/* Initialization of menu */

		hItem= WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_MENU);
		ICONVIEW_SetSpace(hItem, GUI_COORD_X, 24);
		ICONVIEW_SetSpace(hItem, GUI_COORD_Y, 10);
		ICONVIEW_SetFont(hItem, MENU_WINDOW_MENU_ITEM_TEXT_FONT);
		ICONVIEW_SetSel(hItem, -1);

		for ( i = 0; i < GUI_COUNTOF(mBitmaps); i++ ) 
		{
			ICONVIEW_AddBitmapItem(hItem, mBitmaps[i].pBitmap, mBitmaps[i].pText);
			ICONVIEW_SetTextColor(hItem, i, 0x000000);
		}

		// Register event callback.

		event_handler_register_callback(application_data, window_menu_event_callback);

		break;
	case WM_NOTIFY_PARENT:

		Id=    WM_GetId(pMsg->hWinSrc);
		hItem= WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_MENU);
		NCode= pMsg->Data.v;

		switch ( Id ) 
		{
		case ID_ICONVIEW_MENU:
			switch (NCode) {
			case WM_NOTIFICATION_CLICKED:
				window_menu_set_item_selected(hItem, 1);
				break;
			case WM_NOTIFICATION_RELEASED:
				window_menu_set_item_selected(hItem, 0);
				selection = ICONVIEW_GetSel(hItem);

				ICONVIEW_SetSel(hItem, -1);

				switch ( selection ) 
				{
				case 0:
					WM_ShowWindow(window_lights_create_window());
					break;
				case 1:
					WM_ShowWindow(Temperature_CreateTemperatureWindow());
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

// window_menu_create_window

WM_HWIN window_menu_create_window(void) 
{
	hThisWindow= GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), window_menu_gui_callback, WM_HBKWIN, 0, 0);
	return (hThisWindow);
}
