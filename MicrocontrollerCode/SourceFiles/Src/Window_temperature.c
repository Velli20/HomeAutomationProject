// Includes

#include "DIALOG.h"
#include "SWIPELIST.h"
#include "main.h"
#include "UI_resources.h"
#include "RoomConfiguration.h"

// Defines

#define ID_WINDOW_TEMPERATURE  						(GUI_ID_USER + 0x00)
#define ID_IMAGE_VIEW_TEMPERATURE_RETURN 			(GUI_ID_USER + 0x01)
#define ID_TEXT_VIEW_TEMPERATURE_RETURN				(GUI_ID_USER + 0x02)
#define ID_SWIPE_LIST_TEMPERATURE  					(GUI_ID_USER + 0x03)
#define ID_SLIDER_TEMPERATURE  						(GUI_ID_USER + 0x04)
#define ID_TEXT_TEMPERATURE_TARGET  				(GUI_ID_USER + 0x05)
#define ID_TEXT_TEMPERATURE_CURRENT  				(GUI_ID_USER + 0x06)


static WM_HWIN hThisWindow;
static int selected_temperature_widget;
static int selected_temperature_widget_id;

extern uint8_t temperatureReading;
extern APP_DATA* application_data;

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
		{ WINDOW_CreateIndirect,    0,                       ID_WINDOW_TEMPERATURE,            0,   0,   480, 272, 0, 0x0, 0 },
		{ IMAGE_CreateIndirect,     0,                       ID_IMAGE_VIEW_TEMPERATURE_RETURN, 10,  5,   48,  48,  0, 0,   0 },
		{ TEXT_CreateIndirect,      THERMOSTAT_WINDOW_TITLE, ID_TEXT_VIEW_TEMPERATURE_RETURN,  65,  19,  80,  20,  0, 0x0, 0 },
		{ SWIPELIST_CreateIndirect, 0,                       ID_SWIPE_LIST_TEMPERATURE,        0,   59,  240, 214, 0, 0x0, 0 },
		{ SLIDER_CreateIndirect,    0,                       ID_SLIDER_TEMPERATURE,            255, 200, 215, 40,  0, 0x0, 0 },
        { TEXT_CreateIndirect,      0,                       ID_TEXT_TEMPERATURE_TARGET,       280, 160, 160, 40,  0, 0x0, 0 },
        { TEXT_CreateIndirect,      0,                       ID_TEXT_TEMPERATURE_CURRENT,      320, 100, 80,  80,  0, 0x0, 0 },};

// Temperature_UpdateRoomList

static void Temperature_UpdateRoomList(void) 
{
	struct RoomWidget** thermostat_widgets;
	WM_HWIN             swipe_list_widget;
	char                temperature_text[20];
	uint32_t            count;
	uint32_t            i;

	if ( !hThisWindow ) 
		return;

	// Get swipe list widget.

	swipe_list_widget= WM_GetDialogItem(hThisWindow, ID_SWIPE_LIST_TEMPERATURE);
	if ( !swipe_list_widget )
	    return;

	// Get count of defined thermostat widgets.

	count= room_configuration_get_widget_count(application_data->room_configuration, WIDGET_TYPE_THERMOSTAT);
	if ( count == 0 )
	    return;

	// Get thermostat widgets.

	thermostat_widgets= room_configuration_get_widgets(application_data, WIDGET_TYPE_THERMOSTAT);
	if ( !thermostat_widgets ) 
		return;

	for ( i = 0; i < count; i++ ) 
	{
		if ( !thermostat_widgets[i] ) 
		    continue;

		sprintf(temperature_text, "%s %d %s", THERMOSTAT_MEASURED_TEMPERATURE, (int)thermostat_widgets[i]->intValue, TEMPERATURE_UNIT);

		// Update item text based on temperature reading.

		SWIPELIST_SetText(swipe_list_widget, i, 1, temperature_text);
	}

	free(thermostat_widgets);
}

// Temperature_InitRoomList

static void Temperature_InitRoomList(const WM_HWIN swipe_list_widget) 
{
	struct RoomWidget** thermostat_widgets;
	struct RoomWidget*  thermostat_widget;
	char                temperature_text[20];
	uint32_t            count;
	uint32_t            i;
	uint32_t            y_size;

	if ( !swipe_list_widget ) 
		return;
	
	// Get count of available thermostat widgets.

	count= room_configuration_get_widget_count(application_data->room_configuration, WIDGET_TYPE_THERMOSTAT);
	if ( count == 0 )
	    return;

	// Get thermostat widgets.

	thermostat_widgets= room_configuration_get_widgets(application_data, WIDGET_TYPE_THERMOSTAT);
	if ( !thermostat_widgets )
	    return;

	for ( i = 0; i < count; i++ ) 
	{
		thermostat_widget= thermostat_widgets[i];
		if ( !thermostat_widget )
			continue;

		sprintf(temperature_text, "%s %d %s", THERMOSTAT_MEASURED_TEMPERATURE, temperatureReading, TEMPERATURE_UNIT);

		// Add list item with a name of the thermostat widget.

		if ( (thermostat_widget->name) && strlen((thermostat_widget->name)) > 0 )
			SWIPELIST_AddItem(swipe_list_widget, (thermostat_widget->name), ROOMS_LIST_ITEM_HEIGHT);

		// If name is NULL or empty then use default name.

		else 
			SWIPELIST_AddItem(swipe_list_widget, THERMOSTAT_NO_NAME, ROOMS_LIST_ITEM_HEIGHT);

		// Add item text to describe the temperature reading of the thermostat.

		SWIPELIST_AddItemText(swipe_list_widget, i, temperature_text);

	}
	
	// CHange size of the swipe list widget.

	y_size= (count * ROOMS_LIST_ITEM_HEIGHT) +count;
	if ( y_size < _aDialogCreate[3].ySize ) 
		WM_SetSize(swipe_list_widget, _aDialogCreate[3].xSize, y_size);

	free(thermostat_widgets);
}

// Temperature_SetFocusedTemperatureWidget

static void Temperature_SetFocusedTemperatureWidget(int position) 
{
	struct RoomWidget** widgets;
	struct RoomWidget*  thermostat_widget;
	WM_HWIN             target_temp_text_widget;
	WM_HWIN             current_temp_text_widget;
	WM_HWIN             slider_widget;
	char                temperature_text[15];
	uint32_t            count;

	if ( !hThisWindow ) 
		return;
	
	// Find control widgets.

	target_temp_text_widget=  WM_GetDialogItem(hThisWindow, ID_TEXT_TEMPERATURE_TARGET);
	current_temp_text_widget= WM_GetDialogItem(hThisWindow, ID_TEXT_TEMPERATURE_CURRENT);
	slider_widget=            WM_GetDialogItem(hThisWindow, ID_SLIDER_TEMPERATURE);
	if ( !target_temp_text_widget || !current_temp_text_widget || !slider_widget )
	    return;

	// Get count of thermostat widgets.

	count= room_configuration_get_widget_count(application_data->room_configuration, WIDGET_TYPE_THERMOSTAT);
	if ( count == 0 ||  position > count )
	    return;
	
	// Get pointer array of RoomWidgets with a type of WIDGET_TYPE_LIGHT.

	widgets= room_configuration_get_widgets(application_data, WIDGET_TYPE_THERMOSTAT);
	if ( !widgets )
		return;
	
	// Get pointer to the thermostat widget based on list position.

	thermostat_widget= widgets[position];
	if ( !thermostat_widget )
	{
		free(widgets);
		return;
	}

	// Store id of the selected widget so we can access it later easily.

    selected_temperature_widget_id= thermostat_widget->id;

	// Set the slider value.

	SLIDER_SetValue(slider_widget, thermostat_widget->intValueTarget);
	
	// Change target temperature text.

	sprintf(temperature_text, "%s %d %s", THERMOSTAT_TARGET_TEMPERATURE, (int)thermostat_widget->intValueTarget, TEMPERATURE_UNIT);
	TEXT_SetText(target_temp_text_widget, temperature_text);
	
	// Change current temperature text.

	sprintf(temperature_text, "%d %s", (int)temperatureReading, TEMPERATURE_UNIT);
	TEXT_SetText(current_temp_text_widget, temperature_text);
	
	free(widgets);
}

// Temperature_UpdateTargetTemperature

static void Temperature_UpdateTargetTemperature(uint8_t touching_slider) 
{
	WM_HWIN            temperature_text_widget;
	WM_HWIN            slider_widget;
	struct RoomWidget* room_widget;
	int                slider_value; 
	char               temperature_text[15];

	if ( !hThisWindow ) 
		return;
	
	temperature_text_widget= WM_GetDialogItem(hThisWindow, ID_TEXT_TEMPERATURE_TARGET);
	slider_widget=           WM_GetDialogItem(hThisWindow, ID_SLIDER_TEMPERATURE);
	room_widget=             widget_list_find_widget_with_id(application_data->room_configuration, selected_temperature_widget_id);

	if ( !temperature_text_widget || !slider_widget || !room_widget ) 
		return;
	
	// Get the slider value.

	slider_value= SLIDER_GetValue(slider_widget);

	// Set target temperature value.

	room_widget->intValueTarget= slider_value;

	// Update target temperature text value.

	memset(temperature_text, 0, 15);
	sprintf(temperature_text, "%s %u %s", THERMOSTAT_TARGET_TEMPERATURE, slider_value, TEMPERATURE_UNIT);
	TEXT_SetText(temperature_text_widget, temperature_text);
	
	// Notify rest of the UI that value has been updated.

	if ( !touching_slider ) 
	    event_handler_send_event(application_data, room_widget, EVENT_ROOM_WIDGET_UPDATED_LOCALLY);
}

// Temperature_OnWidgetStateChangedCallback

static void Temperature_OnWidgetStateChangedCallback(APP_DATA*          data,
		                                             struct RoomWidget* widget,
		                                             uint8_t            event)
{
	switch ( event )
	{
	
	case EVENT_ROOM_WIDGET_UPDATED_FROM_REMOTE:
		Temperature_SetFocusedTemperatureWidget(selected_temperature_widget);
		break;

	case EVENT_ROOM_WIDGET_UPDATED_LOCALLY:
		Temperature_UpdateRoomList();
		break;
	}
}

// Temperature_DialogCallback

static void Temperature_DialogCallback(WM_MESSAGE* pMsg)
{
	WM_HWIN widget;

	switch ( pMsg->MsgId ) 
	{
	case WM_PAINT:
		// Draw black line to separate title and window content.

		GUI_SetColor(GUI_BLACK);
		GUI_SetPenSize(1);
		GUI_DrawLine(0, 58, 480, 58);
		GUI_DrawLine(240, 58, 240, 272);
		WM_DefaultProc(pMsg);
		break;

	case WM_INIT_DIALOG:

		// Initialization of return icon.

		widget= WM_GetDialogItem(pMsg->hWin, ID_IMAGE_VIEW_TEMPERATURE_RETURN);
		IMAGE_SetBitmap(widget, &bmBITMAP_return);

		// Initialization of title.

		widget= WM_GetDialogItem(pMsg->hWin, ID_TEXT_VIEW_TEMPERATURE_RETURN);
		TEXT_SetTextAlign(widget, GUI_TA_LEFT | GUI_TA_VCENTER);
		TEXT_SetFont(widget, WINDOW_TITLE_FONT);

		// Initialization of slider.

		widget= WM_GetDialogItem(pMsg->hWin, ID_SLIDER_TEMPERATURE);
		SLIDER_SetFocusColor(widget, GUI_WHITE);
		SLIDER_SetRange(widget, MIN_THERMOSTAT_TEMPERATURE, MAX_THERMOSTAT_TEMPERATURE);
		SLIDER_SetWidth(widget, TEMPERATURE_SLIDER_HANDLE_WIDTH);

		// Initialization of current temperature reading.

		widget= WM_GetDialogItem(pMsg->hWin, ID_TEXT_TEMPERATURE_CURRENT);
		TEXT_SetTextAlign(widget, TEXT_CF_HCENTER);
		TEXT_SetFont(widget, THERMOSTAT_WINDOW_CURRENT_TEMPERATURE_FONT);
		TEXT_SetTextColor(widget, LEMON);

		// Initialization of target temperature reading.

		widget= WM_GetDialogItem(pMsg->hWin, ID_TEXT_TEMPERATURE_TARGET);
		TEXT_SetTextAlign(widget, TEXT_CF_HCENTER);
		TEXT_SetFont(widget, THERMOSTAT_WINDOW_TARGET_TEMPERATURE_FONT);
		TEXT_SetTextColor(widget, DARK_BLUE);

		// Initialization of swipe list that displays room names.

		widget= WM_GetDialogItem(pMsg->hWin, ID_SWIPE_LIST_TEMPERATURE);

		SWIPELIST_SetBkColor(widget, SWIPELIST_CI_BK_ITEM_UNSEL, GUI_WHITE);

		// Set list item left padding to 20 px.
		SWIPELIST_SetBorderSize(widget, SWIPELIST_BI_LEFT, 20);

		// Set color of the line that separates list items.
		SWIPELIST_SetDefaultSepColor(GUI_WHITE);

		// Set list item header font and text color.
		
		SWIPELIST_SetFont(widget, SWIPELIST_FI_ITEM_HEADER, THERMOSTAT_WINDOW_LIST_HEADER_FONT);
		SWIPELIST_SetTextColor(widget, SWIPELIST_CI_ITEM_HEADER_SEL, GUI_WHITE);
		SWIPELIST_SetTextColor(widget, SWIPELIST_CI_ITEM_HEADER_UNSEL, LEMON);

		// Enable movability for a window.

		WM_MOTION_SetMoveable(widget, WM_CF_MOTION_Y, 1);

		// Populate list with thermostat widgets.

		Temperature_InitRoomList(widget);

		// Register callback.

		event_handler_register_callback(application_data, Temperature_OnWidgetStateChangedCallback);

		break;

	case WM_NOTIFY_PARENT:
		
		switch ( WM_GetId(pMsg->hWinSrc) ) 
		{

		case ID_IMAGE_VIEW_TEMPERATURE_RETURN:
			if ( pMsg->Data.v == WM_NOTIFICATION_RELEASED ) 
			{
				WM_HideWindow(hThisWindow);
				event_handler_unregister_callback(application_data, Temperature_OnWidgetStateChangedCallback);
			}
			break;
		case ID_SLIDER_TEMPERATURE:

			if ( pMsg->Data.v == WM_NOTIFICATION_VALUE_CHANGED ) 
				Temperature_UpdateTargetTemperature(1);

			else if ( pMsg->Data.v == WM_NOTIFICATION_RELEASED )
				Temperature_UpdateTargetTemperature(0);
			
			break;
		case ID_SWIPE_LIST_TEMPERATURE:

			if ( pMsg->Data.v == WM_NOTIFICATION_RELEASED ) 
			{
				widget= WM_GetDialogItem(pMsg->hWin, ID_SWIPE_LIST_TEMPERATURE);
				Temperature_SetFocusedTemperatureWidget(SWIPELIST_GetReleasedItem(widget));
			}
			break;
		}
		break;
	default:
		WM_DefaultProc(pMsg);
		break;
	}
}

// Temperature_CreateTemperatureWindow

WM_HWIN Temperature_CreateTemperatureWindow(void)
{
	hThisWindow= GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), Temperature_DialogCallback, WM_HBKWIN, 0, 0);

	Temperature_SetFocusedTemperatureWidget(selected_temperature_widget);
	Temperature_UpdateTargetTemperature(selected_temperature_widget);

	return(hThisWindow);
}
