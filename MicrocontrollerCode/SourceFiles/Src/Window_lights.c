// Includes

#include "DIALOG.h"
#include "main.h"
#include "SWIPELIST.h"
#include "RoomConfiguration.h"
#include "RoomWidgetConstants.h"
#include "RoomWidget.h"
#include "UI_resources.h"

// Defines

#define ID_WINDOW_LIGHTS  				(GUI_ID_USER + 0x00)
#define ID_IMAGE_VIEW_LIGHTS_RETURN  	(GUI_ID_USER + 0x01)
#define ID_TEXT_LIGHTS_RETURN  			(GUI_ID_USER + 0x02)
#define ID_LIGTHS_SWIPE_LIST  			(GUI_ID_USER + 0x03)
#define ID_IMAGE_VIEW_LIGHTS_SELECTION  (GUI_ID_USER + 0x04)
#define ID_TEXT_LIGHTS_SELECTION  		(GUI_ID_USER + 0x05)

extern APP_DATA* application_data;

static WM_HWIN hThisWindow;

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[]= {
		{ WINDOW_CreateIndirect,    0,                   ID_WINDOW_LIGHTS,               0,   0,   480, 272, 0, 0x0, 0 },
		{ IMAGE_CreateIndirect,     0,                   ID_IMAGE_VIEW_LIGHTS_RETURN,    10,  5,   48,  48,  0, 0,   0 },
		{ TEXT_CreateIndirect,      LIGHTS_WINDOW_TITLE, ID_TEXT_LIGHTS_RETURN,          65,  19,  80,  20,  0, 0x0, 0 },
		{ SWIPELIST_CreateIndirect, 0,                   ID_LIGTHS_SWIPE_LIST,           0,   59,  240, 214, 0, 0x0, 0 },
        { IMAGE_CreateIndirect,     0,                   ID_IMAGE_VIEW_LIGHTS_SELECTION, 302, 78,  116, 116, 0, 0,   0 },
        { TEXT_CreateIndirect,      0,                   ID_TEXT_LIGHTS_SELECTION,       292, 176, 140, 20,  0, 0x0, 0 },};


// window_lights_init_room_list

static void window_lights_init_room_list(const WM_HWIN hItem)
{
	struct RoomWidget** lights;
	struct RoomWidget*  light_widget;
	uint16_t            y_size;
	uint8_t             count;
	uint8_t             i;

	if ( !hItem || !application_data )
		return;

	count= room_configuration_get_widget_count(application_data->room_configuration, WIDGET_TYPE_LIGHT);
	if ( count == 0 )
		return;

	lights= room_configuration_get_widgets(application_data, WIDGET_TYPE_LIGHT);
	if ( !lights )
		return;

	for ( i = 0; i < count; i++ )
	{
		light_widget= lights[i];
		if ( !light_widget )
			continue;

		// Add list item with a name of the light widget.

		if ( light_widget->name )
			SWIPELIST_AddItem(hItem, (light_widget->name), LIGHTS_LIST_ITEM_HEIGHT);

		// RoomWidget name is undefined. Use default name.

		else
			SWIPELIST_AddItem(hItem, LIGHTS_NO_NAME, LIGHTS_LIST_ITEM_HEIGHT);

		// Add item text to describe if light widget is turned on or off */

		SWIPELIST_AddItemText(hItem, i, (const char*)(light_widget->boolValue == 1 ? LIGHTS_ITEM_TEXT_LIGHTS_ON : LIGHTS_ITEM_TEXT_LIGHTS_OFF));

	}
	// Background color of the swipe list widget is black at the area where is no list items

	y_size= (count * LIGHTS_LIST_ITEM_HEIGHT) + count;
	if ( y_size < _aDialogCreate[3].ySize)
		WM_SetSize(hItem, _aDialogCreate[3].xSize, y_size);

	free(lights);
}

// window_lights_update_room_list

static void window_lights_update_room_list(const WM_HWIN hItem)
{
	struct RoomWidget** lights;
	struct RoomWidget*  light_widget;
	uint16_t            count;
	uint16_t            i;

	if ( !hItem )
		return;

	count= room_configuration_get_widget_count(application_data->room_configuration, WIDGET_TYPE_LIGHT);
	if ( count == 0 )
		return;

	lights= room_configuration_get_widgets(application_data, WIDGET_TYPE_LIGHT);
	if ( !lights )
		return;

	for ( i = 0; i < count; i++ )
	{
		light_widget= lights[i];
		if ( !light_widget )
			continue;

		// Update list item text to match the new status of the RoomWidget.

		SWIPELIST_SetText(hItem, i, 1, light_widget->boolValue == 0 ? LIGHTS_ITEM_TEXT_LIGHTS_OFF : LIGHTS_ITEM_TEXT_LIGHTS_ON);
	}

	// Notify SwipeList widget that content is changed.

	WM_InvalidateWindow(hItem);

	free(lights);
}

// window_lights_toggle_light_widget

static void window_lights_toggle_light_widget(void)
{
	struct RoomWidget* light_widget;
	WM_HWIN            hItemIcon;
	WM_HWIN            hItemList;

	if ( !application_data || !application_data->active_window )
		return;

	hItemIcon= WM_GetDialogItem(*(application_data->active_window), ID_IMAGE_VIEW_LIGHTS_SELECTION);
	hItemList= WM_GetDialogItem(*(application_data->active_window), ID_LIGTHS_SWIPE_LIST);

	light_widget= widget_list_find_widget_with_id(application_data->room_configuration, application_data->selected_light_widget_id);

    if ( !hItemIcon || !hItemList || !light_widget )
    	return;

	// Toggle widget state.

    light_widget->boolValue = (light_widget->boolValue == 0 ? 1 : 0);

	IMAGE_SetBitmap(hItemIcon, light_widget->boolValue == 0 ? &bmBITMAP_lights_off : &bmBITMAP_lights_on);

	// Change list item text to match the state of the widget.

	SWIPELIST_SetText(hItemList, application_data->selected_light_widget, 1, light_widget->boolValue == 0 ? LIGHTS_ITEM_TEXT_LIGHTS_OFF : LIGHTS_ITEM_TEXT_LIGHTS_ON);
	WM_InvalidateWindow(hItemList);

	// Notify callbacks.

	event_handler_send_event(application_data, light_widget, EVENT_ROOM_WIDGET_UPDATED_LOCALLY);
}

// window_lights_set_item_selected

static void window_lights_set_item_selected(const uint8_t position)
{
	WM_HWIN             hItemIcon;
	WM_HWIN             hItemText;
	struct RoomWidget** lights;
	struct RoomWidget*  lightWidget;
	uint16_t            count;

	if ( !application_data || !application_data->active_window )
		return;

	// Save position of the clicked item.

	application_data->selected_light_widget= position;

	hItemIcon= WM_GetDialogItem(*(application_data->active_window), ID_IMAGE_VIEW_LIGHTS_SELECTION);
	hItemText= WM_GetDialogItem(*(application_data->active_window), ID_TEXT_LIGHTS_SELECTION);
	if ( !hItemIcon || !hItemText )
		return;

	// Get count of lights widget.

	count= room_configuration_get_widget_count(application_data->room_configuration, WIDGET_TYPE_LIGHT);
	if ( position > count || count == 0 )
		return;

	// Get pointer array of RoomWidgets with a type of WIDGET_TYPE_LIGHT.

	lights= room_configuration_get_widgets(application_data, WIDGET_TYPE_LIGHT);
	if ( !lights )
		return;

	// Get pointer to the light widget based on list position.

	lightWidget= lights[position];
	if ( lightWidget )
	{
		IMAGE_SetBitmap(hItemIcon, lightWidget->boolValue == 0 ? &bmBITMAP_lights_off : &bmBITMAP_lights_on);
		TEXT_SetText(hItemText, lightWidget->name);

		// Store selected RoomWidget id so we can access it later easily.

		application_data->selected_light_widget_id= lightWidget->id;
	}

	free(lights);
}

// window_lights_event_callback

static void window_lights_event_callback(APP_DATA*          data,
		                                 struct RoomWidget* widget,
		                                 uint8_t            event)
{
	if ( !application_data || !application_data->active_window )
		return;

	// Client device has updated RoomWidget with a device type matching Light.

	if ( event == EVENT_ROOM_WIDGET_UPDATED_FROM_REMOTE )
	{
		window_lights_set_item_selected(data->selected_light_widget);
	    window_lights_update_room_list(WM_GetDialogItem(*(application_data->active_window), ID_LIGTHS_SWIPE_LIST));
	}
}

// window_lights_gui_callback

static void window_lights_gui_callback(WM_MESSAGE * pMsg)
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

		// Register event callback.

		event_handler_register_callback(application_data, window_lights_event_callback);

		// Initialization of return icon.

		widget= WM_GetDialogItem(pMsg->hWin, ID_IMAGE_VIEW_LIGHTS_RETURN);
		IMAGE_SetBitmap(widget, &bmBITMAP_return);

		// Initialization of title.

		widget= WM_GetDialogItem(pMsg->hWin, ID_TEXT_LIGHTS_RETURN);
		TEXT_SetTextAlign(widget, GUI_TA_LEFT | GUI_TA_VCENTER);
		TEXT_SetFont(widget, WINDOW_TITLE_FONT);

		// Initialization of swipe list that displays room names.

		widget= WM_GetDialogItem(pMsg->hWin, ID_LIGTHS_SWIPE_LIST);

		SWIPELIST_SetBkColor(widget, SWIPELIST_CI_BK_ITEM_UNSEL, GUI_WHITE);
		SWIPELIST_SetBorderSize(widget, SWIPELIST_BI_LEFT, 20);
		SWIPELIST_SetDefaultSepColor(GUI_WHITE);
		SWIPELIST_SetFont(widget, SWIPELIST_FI_ITEM_HEADER, LIGHTS_WINDOW_LIST_HEADER_FONT);
		SWIPELIST_SetTextColor(widget, SWIPELIST_CI_ITEM_HEADER_SEL, GUI_WHITE);
		SWIPELIST_SetTextColor(widget, SWIPELIST_CI_ITEM_HEADER_UNSEL, LEMON);

		// Enable movability for a window.

		WM_MOTION_SetMoveable(widget, WM_CF_MOTION_Y, 1);

		// Populate swipe list with room names.

		window_lights_init_room_list(widget);

		// Initialization of selection icon.

		widget= WM_GetDialogItem(pMsg->hWin, ID_IMAGE_VIEW_LIGHTS_SELECTION);
		IMAGE_SetBitmap(widget, &bmBITMAP_lights_off);


		// Initialization of selection icon title.

		widget= WM_GetDialogItem(pMsg->hWin, ID_TEXT_LIGHTS_SELECTION);
		TEXT_SetTextAlign(widget, GUI_TA_HCENTER | GUI_TA_VCENTER);
		TEXT_SetFont(widget, LIGHTS_WINDOW_LIGTH_TOGGLE_FONT);
		break;

	case WM_DELETE:
		event_handler_unregister_callback(application_data, window_lights_event_callback);
		WM_DefaultProc(pMsg);
		break;

	case WM_NOTIFY_PARENT:

		switch ( WM_GetId(pMsg->hWinSrc) )
		{

		case ID_IMAGE_VIEW_LIGHTS_RETURN:

			// Return to main window on back button click.

			if ( pMsg->Data.v == WM_NOTIFICATION_RELEASED && application_data && application_data->active_window )
			{
				WM_HideWindow(*(application_data->active_window));
				application_data->active_window= 0;

			}
			break;
		case ID_LIGTHS_SWIPE_LIST:

			// User has clicked on a list item.

			if ( pMsg->Data.v == WM_NOTIFICATION_RELEASED )
			{
				widget= WM_GetDialogItem(pMsg->hWin, ID_LIGTHS_SWIPE_LIST);
				window_lights_set_item_selected(((uint8_t)SWIPELIST_GetReleasedItem(widget)));
			}
			break;
		case ID_IMAGE_VIEW_LIGHTS_SELECTION:

			// Toggle light widget.

			if ( pMsg->Data.v == WM_NOTIFICATION_RELEASED )
				window_lights_toggle_light_widget();

			break;
		}
		break;
	default:
		WM_DefaultProc(pMsg);
		break;
	}
}

// window_lights_create_window

WM_HWIN window_lights_create_window(void)
{

	hThisWindow= GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), window_lights_gui_callback, WM_HBKWIN, 0, 0);

	application_data->active_window= &hThisWindow;
	window_lights_set_item_selected(0);

	return(hThisWindow);
}
