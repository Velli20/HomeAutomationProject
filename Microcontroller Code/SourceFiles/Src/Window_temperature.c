/* Includes */
#include "DIALOG.h"
#include "SWIPELIST.h"
#include "main.h"
#include "BITMAPS.h"
#include "fonts.h"

/* Defines */
#define ID_WINDOW_TEMPERATURE  						(GUI_ID_USER + 0x00)
#define ID_IMAGE_VIEW_TEMPERATURE_RETURN 			(GUI_ID_USER + 0x01)
#define ID_TEXT_VIEW_TEMPERATURE_RETURN				(GUI_ID_USER + 0x02)
#define ID_SWIPE_LIST_TEMPERATURE  					(GUI_ID_USER + 0x03)

/* Colors */
#define LEMON       					0x00d6d3
#define DARK_BLUE   					0x613600
/* Height of the list item in pixels */
#define ROOMS_LIST_ITEM_HEIGHT 			56
/* List item text if thermostat widget name is null or empty */
#define THERMOSTAT_NO_NAME				"NO NAME"
/* Maximum and minimum room temperature */
#define MAX_TEMPERATURE					30
#define MIN_TEMPERATURE					2

/* Public function prototypes */
WM_HWIN Temperature_CreateTemperatureWindow(void);

/* Private function prototypes */
static void Temperature_DialogCallBack(WM_MESSAGE * pMsg);
static void Temperature_InitRoomList(const WM_HWIN hItem);


/* Private variables */
static WM_HWIN hThisWindow;

/* Set pointer to a font, used for an easier exchange of fonts */
static GUI_CONST_STORAGE GUI_FONT * pFont23pBold = &GUI_Font23S_AA4;
static GUI_CONST_STORAGE GUI_FONT * pFont32pBold = &GUI_Font32S_AA4;
static GUI_CONST_STORAGE GUI_FONT * pFont23p     = &GUI_Font23SL_AA4;
static GUI_CONST_STORAGE GUI_FONT * pFont32p     = &GUI_Font32SL_AA4;


static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
		{ WINDOW_CreateIndirect, NULL, ID_WINDOW_TEMPERATURE, 1, 0, 480, 272, 0, 0x0, 0 },
		{ IMAGE_CreateIndirect, NULL, ID_IMAGE_VIEW_TEMPERATURE_RETURN, 10, 5, 48, 48, 0, 0, 0 },
		{ TEXT_CreateIndirect, "Lämpötila ", ID_TEXT_VIEW_TEMPERATURE_RETURN, 65, 19, 80, 20, 0, 0x0, 0 },
		{ SWIPELIST_CreateIndirect, "", ID_SWIPE_LIST_TEMPERATURE, 0, 59, 240, 214, 0, 0x0, 0 },};

/* Private code */

/**
 * @brief  Handle callback events realted to knob widget
 * @param  pMsg: callback message structure from parent window
 * @retval None
 */
static void Temperature_DialogCallBack(WM_MESSAGE * pMsg) {
	static GUI_MEMDEV_Handle hMemKnob;
	WM_HWIN hItem;
	WM_HWIN hKnob;
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
		TEXT_SetFont(hItem, GUI_FONT_13B_1);

		/* Initialization of swipe list that displays room names */
		hItem = WM_GetDialogItem(pMsg->hWin, ID_SWIPE_LIST_TEMPERATURE);

		SWIPELIST_SetBkColor(hItem, SWIPELIST_CI_BK_ITEM_UNSEL, GUI_WHITE);
		/* Set list item left padding to 20 px */
		SWIPELIST_SetBorderSize(hItem, SWIPELIST_BI_LEFT, 20);
		/* Set color of the line that separates list items */
		SWIPELIST_SetDefaultSepColor(GUI_WHITE);
		/* Set list item header font. This dosen't affect on item text */
		SWIPELIST_SetFont(hItem, SWIPELIST_FI_ITEM_HEADER, pFont23p);
		//SWIPELIST_SetFont(hItem, SWIPELIST_FI_ITEM_TEXT, pFont23p);

		SWIPELIST_SetTextColor(hItem, SWIPELIST_CI_ITEM_HEADER_SEL, GUI_WHITE);
		SWIPELIST_SetTextColor(hItem, SWIPELIST_CI_ITEM_HEADER_UNSEL, LEMON);
		/* Enable movability for a window */
		WM_MOTION_SetMoveable(hItem, WM_CF_MOTION_Y, 1);
		/* Populate list with thermostat widgets */
		Temperature_InitRoomList(hItem);


		break;

	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch (Id) {

		/* Notifications sent by ID_IMAGE_VIEW_TEMPERATURE_RETURN */
		case ID_IMAGE_VIEW_TEMPERATURE_RETURN:
			switch (NCode) {
			case WM_NOTIFICATION_RELEASED:
				/* User pressed return icon */
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
				sprintf(temperatureText, "%d °C", thermostatWidget->intValue);
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


/* Public code */

WM_HWIN Temperature_CreateTemperatureWindow(void) {
	WM_MOTION_Enable(1);
	hThisWindow = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), Temperature_DialogCallBack, WM_HBKWIN, 0, 0);
	return hThisWindow;
}


/* End of file */
