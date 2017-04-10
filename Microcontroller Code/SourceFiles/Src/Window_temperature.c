/* Includes */
#include "DIALOG.h"
#include "SWIPELIST.h"
#include "main.h"
#include "UI_resources.h"

/* Defines */
#define ID_WINDOW_TEMPERATURE  						(GUI_ID_USER + 0x00)
#define ID_IMAGE_VIEW_TEMPERATURE_RETURN 			(GUI_ID_USER + 0x01)
#define ID_TEXT_VIEW_TEMPERATURE_RETURN				(GUI_ID_USER + 0x02)
#define ID_SWIPE_LIST_TEMPERATURE  					(GUI_ID_USER + 0x03)
#define ID_SLIDER_TEMPERATURE  						(GUI_ID_USER + 0x04)
#define ID_TEXT_TEMPERATURE_TARGET  				(GUI_ID_USER + 0x05)
#define ID_TEXT_TEMPERATURE_CURRENT  				(GUI_ID_USER + 0x06)


/* Private function prototypes */
static void Temperature_DialogCallback(WM_MESSAGE * pMsg);
static void Temperature_OnWidgetStateChangedCallback(struct RoomWidget * widget, int notificationCode);
static void Temperature_InitRoomList(const WM_HWIN hItem);
static void Temperature_UpdateRoomList();
static void Temperature_SetFocusedTemperatureWidget(int position);
static void Temperature_UpdateTargetTemperature(int touchingSlider);

/* Private variables */
static WM_HWIN hThisWindow;
static int selectedTemperatureWidget = 0;
static int selectedTemperatureWidgetId;


static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
		{ WINDOW_CreateIndirect, NULL, ID_WINDOW_TEMPERATURE, 0, 0, 480, 272, 0, 0x0, 0 },
		{ IMAGE_CreateIndirect, NULL, ID_IMAGE_VIEW_TEMPERATURE_RETURN, 10, 5, 48, 48, 0, 0, 0 },
		{ TEXT_CreateIndirect, THERMOSTAT_WINDOW_TITLE, ID_TEXT_VIEW_TEMPERATURE_RETURN, 65, 19, 80, 20, 0, 0x0, 0 },
		{ SWIPELIST_CreateIndirect, NULL, ID_SWIPE_LIST_TEMPERATURE, 0, 59, 240, 214, 0, 0x0, 0 },
		{ SLIDER_CreateIndirect, NULL, ID_SLIDER_TEMPERATURE, 255, 200, 215, 40, 0, 0x0, 0 },
        { TEXT_CreateIndirect, NULL, ID_TEXT_TEMPERATURE_TARGET, 280, 160, 160, 40, 0, 0x0, 0 },
        { TEXT_CreateIndirect, NULL, ID_TEXT_TEMPERATURE_CURRENT, 320, 100, 80, 80, 0, 0x0, 0 },};

/* Private code */

/**
 * @brief  Handle callback events from parent window
 * @param  pMsg: callback message structure from parent window
 * @retval None
 */
static void Temperature_DialogCallback(WM_MESSAGE * pMsg) {
	WM_HWIN hItem;
	int NCode;
	int Id;

	switch (pMsg->MsgId) {
	case WM_PAINT:
		/* Draw black line to separate title and window content*/
		GUI_SetColor(GUI_BLACK);
		GUI_SetPenSize(1);
		GUI_DrawLine(0, 58, 480, 58);
		GUI_DrawLine(240, 58, 240, 272);
		WM_DefaultProc(pMsg);
		break;
	case WM_INIT_DIALOG:

		/* Initialization of return icon */
		hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_VIEW_TEMPERATURE_RETURN);
		IMAGE_SetBitmap(hItem, &bmBITMAP_return);

		/* Initialization of title */
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_VIEW_TEMPERATURE_RETURN);
		TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
		TEXT_SetFont(hItem, WINDOW_TITLE_FONT);

		/* Initialization of slider */
		hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_TEMPERATURE);
		SLIDER_SetFocusColor(hItem, GUI_WHITE);
		SLIDER_SetRange(hItem, MIN_THERMOSTAT_TEMPERATURE, MAX_THERMOSTAT_TEMPERATURE);
		SLIDER_SetWidth(hItem, TEMPERATURE_SLIDER_HANDLE_WIDTH);

		/* Initialization of current temperature reading */
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_TEMPERATURE_CURRENT);
		TEXT_SetTextAlign(hItem, TEXT_CF_HCENTER);
		TEXT_SetFont(hItem, THERMOSTAT_WINDOW_CURRENT_TEMPERATURE_FONT);
		TEXT_SetTextColor(hItem, LEMON);

		/* Initialization of target temperature reading */
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_TEMPERATURE_TARGET);
		TEXT_SetTextAlign(hItem, TEXT_CF_HCENTER);
		TEXT_SetFont(hItem, THERMOSTAT_WINDOW_TARGET_TEMPERATURE_FONT);
		TEXT_SetTextColor(hItem, DARK_BLUE);

		/* Initialization of swipe list that displays room names */
		hItem = WM_GetDialogItem(pMsg->hWin, ID_SWIPE_LIST_TEMPERATURE);

		SWIPELIST_SetBkColor(hItem, SWIPELIST_CI_BK_ITEM_UNSEL, GUI_WHITE);
		/* Set list item left padding to 20 px */
		SWIPELIST_SetBorderSize(hItem, SWIPELIST_BI_LEFT, 20);
		/* Set color of the line that separates list items */
		SWIPELIST_SetDefaultSepColor(GUI_WHITE);
		/* Set list item header font. This dosen't affect on item text */
		SWIPELIST_SetFont(hItem, SWIPELIST_FI_ITEM_HEADER, THERMOSTAT_WINDOW_LIST_HEADER_FONT);
		//SWIPELIST_SetFont(hItem, SWIPELIST_FI_ITEM_TEXT, pFont23p);

		SWIPELIST_SetTextColor(hItem, SWIPELIST_CI_ITEM_HEADER_SEL, GUI_WHITE);
		SWIPELIST_SetTextColor(hItem, SWIPELIST_CI_ITEM_HEADER_UNSEL, LEMON);
		/* Enable movability for a window */
		WM_MOTION_SetMoveable(hItem, WM_CF_MOTION_Y, 1);
		/* Populate list with thermostat widgets */
		Temperature_InitRoomList(hItem);
		/* Register callback */
		RoomConfiguration_SetOnWidgetStateChangedCallback(Temperature_OnWidgetStateChangedCallback);

		break;

	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch (Id) {

		case ID_IMAGE_VIEW_TEMPERATURE_RETURN:
			switch (NCode) {
			case WM_NOTIFICATION_RELEASED:
				/* User pressed return icon */
				WM_HideWindow(hThisWindow);
				/* Unregister callback */
				RoomConfiguration_SetOnWidgetStateChangedCallback(NULL);
				break;
			}
			break;
		case ID_SLIDER_TEMPERATURE:
			switch(NCode) {

			case WM_NOTIFICATION_VALUE_CHANGED:
				/* Slider value has changed */
				Temperature_UpdateTargetTemperature(1);
				break;
			case WM_NOTIFICATION_RELEASED:
				/* User has released slider handle */
				Temperature_UpdateTargetTemperature(0);
				break;
			}
			break;
		case ID_SWIPE_LIST_TEMPERATURE:
			hItem = WM_GetDialogItem(pMsg->hWin, ID_SWIPE_LIST_TEMPERATURE);
			switch(NCode) {

			case WM_NOTIFICATION_RELEASED:
				/* User has selected widget */
				selectedTemperatureWidget = SWIPELIST_GetReleasedItem(hItem);
				Temperature_SetFocusedTemperatureWidget(selectedTemperatureWidget);
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
 * @brief  Function that is registered for the RoomWidget status changes at Lights_DialogCallback()
 * 		   function. When RoomConfiguration status updates are received, this function is called.
 * @param  widget: Pointer to the RoomWidget that status is changed
 */
static void Temperature_OnWidgetStateChangedCallback(struct RoomWidget * widget, int notificationCode) {
	switch(notificationCode) {
	case RC_WIDGET_UPDATED_FROM_USER:
		break;
	case RC_WIDGET_UPDATED_FROM_REMOTE:
		/* Update target temperature reading */
		Temperature_SetFocusedTemperatureWidget(selectedTemperatureWidget);
		break;
	case RC_WIDGET_UPDATED_FROM_DEVICE:
		/* Update actual temperature readings */
		Temperature_UpdateRoomList();
		break;
	}
}


/**
 * @brief  Populate swipe list widget with thermostat widget names
 * @param  hItem: Handle of SwipeList widget
 * @retval None
 */
static void Temperature_InitRoomList(const WM_HWIN hItem) {
	if(!hItem) {
		return;
	}
	/* Get count of available thermostat widgets */
	const int count = RoomConfiguration_GetThermostatWidgetCount();

	if (count > 0) {
		/* Get pointer array for thermostat widgets */
		struct RoomWidget ** thermostats = RoomConfiguration_GetPointerArrayForWidgetType(WIDGET_TYPE_THERMOSTAT);

		for (int i = 0; i < count; i++) {
			struct RoomWidget * thermostatWidget = thermostats[i];

			if (thermostatWidget) {
				char temperatureText[20];
				sprintf(temperatureText, "%s %d %s", THERMOSTAT_MEASURED_TEMPERATURE, thermostatWidget->intValue, TEMPERATURE_UNIT);
				if((thermostatWidget->name) && strlen((thermostatWidget->name)) > 0){
					/* Add list item with a name of the thermostat */
					SWIPELIST_AddItem(hItem, (thermostatWidget->name), ROOMS_LIST_ITEM_HEIGHT);
				} else {
					/* If name is NULL or empty then use default name */
					SWIPELIST_AddItem(hItem, THERMOSTAT_NO_NAME, ROOMS_LIST_ITEM_HEIGHT);
				}
				/* Add item text to describe the temperature reading of the thermostat */
				SWIPELIST_AddItemText(hItem, i, temperatureText);
			}

		}
		/* Background color of the swipe list widget is black
		 * at the area where is no list items
		 */
		int ySize = (count * ROOMS_LIST_ITEM_HEIGHT) +count;
		if(ySize < _aDialogCreate[3].ySize) {
			WM_SetSize(hItem, _aDialogCreate[3].xSize, ySize);
		}
		free(thermostats);
	}

}

/**
 * @brief  Update item values on the swipe list
 * @param  hItem: Handle of SwipeList widget
 * @retval None
 */
static void Temperature_UpdateRoomList() {
	if(!hThisWindow) {
		return;
	}

	/* Get window handle of the swipe list widget */
	const WM_HWIN hItem = WM_GetDialogItem(hThisWindow, ID_SWIPE_LIST_TEMPERATURE);

	/* Get count of available thermostat widgets */
	const int count = RoomConfiguration_GetThermostatWidgetCount();
	/* Get pointer array for thermostat widgets */
	struct RoomWidget ** thermostats = RoomConfiguration_GetPointerArrayForWidgetType(WIDGET_TYPE_THERMOSTAT);

	if(!thermostats) {
		return;
	}

	for (int i = 0; i < count; i++) {
		struct RoomWidget * thermostatWidget = thermostats[i];

		if (thermostatWidget) {
			char temperatureText[20];
			sprintf(temperatureText, "%s %d %s", THERMOSTAT_MEASURED_TEMPERATURE, thermostatWidget->intValue, TEMPERATURE_UNIT);

			/* Update item text based on temperature reading*/
			SWIPELIST_SetText(hItem, i, 1, temperatureText);
		}

	}

	free(thermostats);


}

/**
 * @brief  Set widget that is displayed on the right side of
 *         the window.
 * @param  position: Position of the widget in list
 * @retval None
 */
static void Temperature_SetFocusedTemperatureWidget(int position) {
	if(!hThisWindow) {
		return;
	}
	WM_HWIN hItemTargetTemp = WM_GetDialogItem(hThisWindow, ID_TEXT_TEMPERATURE_TARGET);
	WM_HWIN hItemCurrentTemp = WM_GetDialogItem(hThisWindow, ID_TEXT_TEMPERATURE_CURRENT);
	WM_HWIN hItemTempSlider = WM_GetDialogItem(hThisWindow, ID_SLIDER_TEMPERATURE);

	/* Get count of thermostat widgets */
	int count = RoomConfiguration_GetThermostatWidgetCount();
	/* Get pointer array of RoomWidgets with a type of WIDGET_TYPE_LIGHT */
	struct RoomWidget ** thermostats = RoomConfiguration_GetPointerArrayForWidgetType(WIDGET_TYPE_THERMOSTAT);

	if(position > count || count == 0 || !thermostats || !hItemTargetTemp || !hItemCurrentTemp || !hItemTempSlider) {
		return;
	}
	/* Get pointer to the thermostat widget based on list position */
	struct RoomWidget * thermWidget = thermostats[position];

	char temperatureString[15];

	if(thermWidget) {
		/* Store id of the selected widget so we can access it later easily */
		selectedTemperatureWidgetId = thermWidget->id;
		/* Set slider value */
		SLIDER_SetValue(hItemTempSlider, thermWidget->intValueTarget);
		/* Format target temperature string */
		sprintf(temperatureString, "%s %d %s", THERMOSTAT_TARGET_TEMPERATURE, thermWidget->intValueTarget, TEMPERATURE_UNIT);
		TEXT_SetText(hItemTargetTemp, temperatureString);
		/* Format current temperature string */
		sprintf(temperatureString, "%d %s", thermWidget->intValue, TEMPERATURE_UNIT);
		TEXT_SetText(hItemCurrentTemp, temperatureString);
	}
	free(thermostats);
}

/**
 * @brief  Update thermostat widget target temperature value
 * @param  position: Position of the widget in list
 * @retval None
 */
static void Temperature_UpdateTargetTemperature(int touchingSlider) {
	if (!hThisWindow) {
		return;
	}
	WM_HWIN hItemTargetTemp = WM_GetDialogItem(hThisWindow, ID_TEXT_TEMPERATURE_TARGET);
	WM_HWIN hItemTempSlider = WM_GetDialogItem(hThisWindow, ID_SLIDER_TEMPERATURE);

	struct RoomList * roomConf = RoomConfiguration_GetRoomConfiguration();
	struct RoomWidget * thermWidget = RoomWidget_GetRoomWidgetWithId(roomConf, selectedTemperatureWidgetId);

	if(!hItemTargetTemp || !hItemTempSlider || !thermWidget) {
		return;
	}
	/* Get slider value */
	int targetTemperature = SLIDER_GetValue(hItemTempSlider);
	/* Set target temperature value */
	thermWidget->intValueTarget = targetTemperature;

	/* Update target temperature text value */
	char temperatureString[15];
	sprintf(temperatureString, "%s %d %s", THERMOSTAT_TARGET_TEMPERATURE, targetTemperature, TEMPERATURE_UNIT);
	TEXT_SetText(hItemTargetTemp, temperatureString);
	if(!touchingSlider) {
		/* Notify rest of the UI that value has been updated */
		RoomConfiguration_NotifyWidgetUpdated(thermWidget, RC_WIDGET_UPDATED_FROM_USER);
	}
}

/* Public code */


/**
  * @brief Create temperature window
  * @param None
  * @retval Handle pointing to temperature window
  */
WM_HWIN Temperature_CreateTemperatureWindow(void) {
	WM_MOTION_Enable(1);
	hThisWindow = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), Temperature_DialogCallback, WM_HBKWIN, 0, 0);

	Temperature_SetFocusedTemperatureWidget(selectedTemperatureWidget);
	Temperature_UpdateTargetTemperature(selectedTemperatureWidget);

	return hThisWindow;
}



/* End of file */
