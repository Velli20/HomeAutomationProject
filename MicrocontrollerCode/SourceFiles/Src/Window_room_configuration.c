// Includes

#include "main.h"
#include "DIALOG.h"
#include "SWIPELIST.h"
#include "WM.h"
#include "UI_resources.h"
#include "RoomConfiguration.h"

// Defines

#define ID_WINDOW_ROOM_CONFIGURATION 		(GUI_ID_USER + 0x22)
#define ID_ROOM_CONFIGURATION_LIST  		(GUI_ID_USER + 0x02)
#define ID_TEXT_RETURN  					(GUI_ID_USER + 0x03)
#define ID_IMAGE_VIEW_RETURN  				(GUI_ID_USER + 0x04)
#define LIST_ITEM_HEIGHT 56

extern APP_DATA* application_data;

static WM_HWIN hThisWindow;

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
		{ WINDOW_CreateIndirect,    0,                               ID_WINDOW_ROOM_CONFIGURATION, 0,  0,  480, 272, 0, 0x0, 0 },
		{ IMAGE_CreateIndirect,     0,                               ID_IMAGE_VIEW_RETURN,         10, 5,  48,  48,  0, 0,   0 },
		{ TEXT_CreateIndirect,      WINDOW_ROOM_CONFIGURATION_TITLE, ID_TEXT_RETURN,               65, 20, 140, 20,  0, 0x0, 0 },
		{ SWIPELIST_CreateIndirect, 0,                               ID_ROOM_CONFIGURATION_LIST,   0,  59, 480, 214, 0, 0x0, 0 }, };

// WindowRoomConfiguration_AppendHeaderText

static void WindowRoomConfiguration_AppendHeaderText(WM_HWIN     swipelist_widget, 
                                                     const char* string_format, ...) 
{
	char    value_text[100];
	va_list ap;

	if ( !swipelist_widget || !string_format )
	    return;

	memset(value_text, 0, 100);

	va_start(ap, string_format);
	vsprintf(value_text, string_format, ap);
	va_end(ap);

	SWIPELIST_AddItem(swipelist_widget, value_text, LIST_ITEM_HEIGHT);
}

// WindowRoomConfiguration_PrintRoomNames

static void WindowRoomConfiguration_PrintRoomNames(struct RoomList* list, 
                                                   WM_HWIN          text_widget, 
												   WM_HWIN          title_widget) 
{
	struct Room* room;

	if ( !text_widget || !list || !title_widget ) 
		return;

	// Change window title to default.

	TEXT_SetText(title_widget, WINDOW_ROOM_CONFIGURATION_TITLE);

	room= list->head;
	while ( room ) 
	{
		if ( room->name ) 
			WindowRoomConfiguration_AppendHeaderText(text_widget, "%s", room->name);
		
		room= room->next;
	}
}

// WindowRoomConfiguration_PrintWidgetNames

static void WindowRoomConfiguration_PrintWidgetNames(struct RoomList * list, 
                                                     uint32_t          room_index, 
													 WM_HWIN           text_widget, 
													 WM_HWIN           title_widget) 
{
	struct Room*       room;
	struct RoomWidget* widget;
	uint32_t           index;

	if ( !text_widget || !list || !title_widget ) 
		return;
	
	// Iterate trouch room structs.

	for ( room= list->head, index= 0; index != room_index && room; room= room->next, index++);

	// Check that we are at correct index.

	if ( !room || index != room_index )	
	    return;
			
	// Change window title to selected room name.

	if ( room->name ) 
		TEXT_SetText(title_widget, room->name);
	else
		TEXT_SetText(title_widget, WINDOW_ROOM_CONFIGURATION_ROOM_NO_NAME);
			
	if ( !room->widgets ) 
		return;
		
	widget= room->widgets->head;
	while ( widget ) 
	{
		if ( widget->name ) 
			WindowRoomConfiguration_AppendHeaderText(text_widget, "Widget: %s id: %d", widget->name, widget->id);
		else 
			WindowRoomConfiguration_AppendHeaderText(text_widget, "Widget: %s id: %d", WINDOW_ROOM_CONFIGURATION_WIDGET_NO_NAME, widget->id);
					
		widget= widget->next;
	}
}

// WindowRoomConfiguration_PrintWidgetValues

static void WindowRoomConfiguration_PrintWidgetValues(struct RoomList* list, 
                                                      WM_HWIN          text_widget, 
													  WM_HWIN          title_widget, 
													  uint32_t         widget_position, 
													  uint32_t         room_position) 
{
	struct Room*       room;
	struct RoomWidget* widget;
	uint32_t           pos;

	if ( !text_widget || !list || !title_widget ) 
		return;

	// Find target room pointer.

	for ( room= list->head, pos= 0; room && pos != room_position; pos++, room= room->next );

	if ( !room || pos != room_position || ! room->widgets )
	    return;

	// Find target widget pointer.

	for ( widget= room->widgets->head, pos= 0; widget && pos != widget_position; widget= widget->next, pos++ );

	if ( !widget || pos != widget_position )
	    return;

	// Change window title to selected RoomWidget name.

	if ( widget->name ) 
		TEXT_SetText(title_widget, widget->name);
	else 
		TEXT_SetText(title_widget, WINDOW_ROOM_CONFIGURATION_WIDGET_NO_NAME);
		
	// Print widget type.

	WindowRoomConfiguration_AppendHeaderText(text_widget, "Widget type: %d", widget->type);

	// Print widget int value.

	WindowRoomConfiguration_AppendHeaderText(text_widget, "Widget int value: %d", widget->intValue);

	// Print widget int traget value.

	WindowRoomConfiguration_AppendHeaderText(text_widget, "Widget int target value: %d", widget->intValueTarget);

	// Print widget bool value.

	WindowRoomConfiguration_AppendHeaderText(text_widget, "Widget bool value: %s", (widget->boolValue == 1 ? "true" : "false"));

	// Print widget status.

	WindowRoomConfiguration_AppendHeaderText(text_widget, "Widget status value: %d", widget->status);	
}

static int WindowRoomConfiguration_MoveInRoomConfigurationList(WM_HWIN swipe_list_widget, 
                                                               WM_HWIN title_widget, 
                                                               int selection, 
															   int back)
{
	static int room_position;
	static int menu_position;

	menu_position+= back ? -1 : 1;
    if ( menu_position > 3 ) 
	{
		menu_position= 3;
		return(0);
	}

	switch ( menu_position ) 
	{

	// Print room names.

	case 1: 
		WindowRoomConfiguration_PrintRoomNames(application_data->room_configuration, swipe_list_widget, title_widget);
		return(1);

	// Print widget names.

	case 2: 
		if ( selection >= 0 ) 
			room_position= selection;
		
		WindowRoomConfiguration_PrintWidgetNames(application_data->room_configuration, room_position, swipe_list_widget, title_widget);
		return(1);

	// Print widget values.

	case 3: 
		WindowRoomConfiguration_PrintWidgetValues(application_data->room_configuration, swipe_list_widget, title_widget, selection, room_position);
		return(1);
	}

	return(0);
}

// WindowRoomConfiguration_DialogCallback

static void WindowRoomConfiguration_DialogCallback(WM_MESSAGE* pMsg) 
{
	WM_HWIN icon_widget;
	WM_HWIN swipe_list_widget;
	WM_HWIN title_widget;
	int     Id;
	int     selected_list_item;
	int     result;

	switch ( pMsg->MsgId ) 
	{

	case WM_INIT_DIALOG:
		WINDOW_SetBkColor(pMsg->hWin, GUI_BLACK);

		// Initialization of return icon.

		icon_widget= WM_GetDialogItem(pMsg->hWin, ID_IMAGE_VIEW_RETURN);
		IMAGE_SetBitmap(icon_widget, &bmBITMAP_return);

		// Initialization of title.

		title_widget= WM_GetDialogItem(pMsg->hWin, ID_TEXT_RETURN);
		TEXT_SetTextAlign(title_widget, GUI_TA_LEFT | GUI_TA_VCENTER);
		TEXT_SetFont(title_widget, WINDOW_TITLE_FONT);
		TEXT_SetTextColor(title_widget, GUI_WHITE);

		// Initialization of SWIPELIST widget.

		swipe_list_widget= WM_GetDialogItem(pMsg->hWin, ID_ROOM_CONFIGURATION_LIST);
		WM_MOTION_SetMoveable(swipe_list_widget, WM_CF_MOTION_Y, 1);
		
		// Print room names.

		WindowRoomConfiguration_MoveInRoomConfigurationList(swipe_list_widget, title_widget, 0, 0);

		break;
	case WM_NOTIFY_PARENT:
		Id=                WM_GetId(pMsg->hWinSrc);
		swipe_list_widget= WM_GetDialogItem(pMsg->hWin, ID_ROOM_CONFIGURATION_LIST);
		title_widget=      WM_GetDialogItem(pMsg->hWin, ID_TEXT_RETURN);
			
		if  ( Id == ID_IMAGE_VIEW_RETURN && pMsg->Data.v == WM_NOTIFICATION_RELEASED ) 
		{
			// Move back in RoomConfiguration list.

			result= WindowRoomConfiguration_MoveInRoomConfigurationList(swipe_list_widget, title_widget, -1, 1);
			if ( result == 0 ) 
				WM_HideWindow(hThisWindow);
				
		}
		else if ( Id == ID_ROOM_CONFIGURATION_LIST && pMsg->Data.v == WM_NOTIFICATION_RELEASED ) 
		{
			// User has pressed list item. Get position of selected list item.

			selected_list_item= SWIPELIST_GetReleasedItem(swipe_list_widget);
				
			// Inflate SWIPELIST widget with selected item.

			WindowRoomConfiguration_MoveInRoomConfigurationList(swipe_list_widget, title_widget, selected_list_item, 0);
		}

		break;

	default:
		WM_DefaultProc(pMsg);
		break;
	}
}

// WindowRoomConfiguration_CreateWindow

WM_HWIN WindowRoomConfiguration_CreateWindow(void) 
{
	hThisWindow= GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate),WindowRoomConfiguration_DialogCallback, WM_HBKWIN, 0, 0);
	return(hThisWindow);
}
