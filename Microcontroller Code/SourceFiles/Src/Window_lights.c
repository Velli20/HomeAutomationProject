/* Includes */
#include "DIALOG.h"
#include "main.h"
#include "BITMAPS.h"
#include "SWIPELIST.h"
#include "RoomConfiguration.h"
#include "RoomWidgetConstants.h"
#include "RoomWidget.h"
#include "fonts.h"

/* Defines */
#define ID_WINDOW_LIGHTS  				(GUI_ID_USER + 0x00)
#define ID_IMAGE_VIEW_LIGHTS_RETURN  	(GUI_ID_USER + 0x01)
#define ID_TEXT_LIGHTS_RETURN  			(GUI_ID_USER + 0x02)
#define ID_LIGTHS_SWIPE_LIST  			(GUI_ID_USER + 0x03)
#define ID_IMAGE_VIEW_LIGHTS_SELECTION  (GUI_ID_USER + 0x04)
#define ID_TEXT_LIGHTS_SELECTION  		(GUI_ID_USER + 0x05)

/* Colors */
#define LEMON       					0x00d6d3
/* Height of the list item in pixels */
#define LIGHTS_LIST_ITEM_HEIGHT 		56
/* Title of the window */
#define LIGHTS_WINDOW_TITLE				"Valaistus"
/* List item text is that is displayed under header */
#define LIGHTS_ITEM_TEXT_LIGHTS_ON		"Päällä"
#define LIGHTS_ITEM_TEXT_LIGHTS_OFF		"Pois päältä"
/* List item text if light widget name is null or empty */
#define LIGHTS_NO_NAME					"NO NAME"

/* Private typedef */
typedef struct {
	const GUI_BITMAP * pBitmap;
	const char * pText;
} BITMAP_ITEM;

/* Private function prototypes */
static void Lights_DialogCallback(WM_MESSAGE * pMsg);
static void Lights_InitRoomList(const WM_HWIN hItem);
static void Lights_UpdateRoomList(const WM_HWIN hItem);
static void Lights_SetItemSelected(const int position);
static void Lights_SetItemClicked(const int position);

/* Public function prototypes */
WM_HWIN Lights_CreateLightsWindow(void);
void Lights_OnWidgetStateChangedCallback(struct RoomWidget * widget);

/* Static data */
static WM_HWIN hThisWindow;

/* Set pointer to a font, used for an easier exchange of fonts */
static GUI_CONST_STORAGE GUI_FONT * pFont23pBold = &GUI_Font23S_AA4;
static GUI_CONST_STORAGE GUI_FONT * pFont32pBold = &GUI_Font32S_AA4;
static GUI_CONST_STORAGE GUI_FONT * pFont23p     = &GUI_Font23SL_AA4;
static GUI_CONST_STORAGE GUI_FONT * pFont32p     = &GUI_Font32SL_AA4;

/* Variables */
int selectedLightWidget = 0;

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
		{ WINDOW_CreateIndirect, "", ID_WINDOW_LIGHTS, 0, 0, 480, 272, 0, 0x0, 0 },
		{ IMAGE_CreateIndirect, "", ID_IMAGE_VIEW_LIGHTS_RETURN, 10, 5, 48, 48, 0, 0, 0 },
		{ TEXT_CreateIndirect, LIGHTS_WINDOW_TITLE, ID_TEXT_LIGHTS_RETURN, 65, 19, 80, 20, 0, 0x0, 0 },
		{ SWIPELIST_CreateIndirect, "", ID_LIGTHS_SWIPE_LIST, 0, 59, 240, 214, 0, 0x0, 0 },
        { IMAGE_CreateIndirect, "", ID_IMAGE_VIEW_LIGHTS_SELECTION, 302, 78, 116, 116, 0, 0, 0 },
        { TEXT_CreateIndirect, "", ID_TEXT_LIGHTS_SELECTION, 322, 176, 80, 20, 0, 0x0, 0 },};

/* Static code */
static void Lights_DialogCallback(WM_MESSAGE * pMsg) {
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
		/* Register for RoomWidget status updates */
		RoomConfiguration_SetOnWidgetStateChangedCallback(Lights_OnWidgetStateChangedCallback);

		/* Initialization of return icon */
		hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_VIEW_LIGHTS_RETURN);
		IMAGE_SetBitmap(hItem, &bmBITMAP_return);

		/* Initialization of title */
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_LIGHTS_RETURN);
		TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
		TEXT_SetFont(hItem, GUI_FONT_13B_1);

		/* Initialization of swipe list that displays room names  */
		hItem = WM_GetDialogItem(pMsg->hWin, ID_LIGTHS_SWIPE_LIST);


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
		/* Populate swipe list with room names */
		Lights_InitRoomList(hItem);

		/* Initialization of selection icon */
		hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_VIEW_LIGHTS_SELECTION);
		IMAGE_SetBitmap(hItem, &bmBITMAP_lights_off);


		/* Initialization of selection icon title */
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_LIGHTS_SELECTION);
		TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		TEXT_SetFont(hItem, GUI_FONT_13B_1);

		break;
	case WM_DELETE:
		/* WM_DELETE: Sent just before a window is deleted, telling
		 * the window to free its data structures (if any).
		 */
		/* Unregister for RoomWidget status updates */
		RoomConfiguration_SetOnWidgetStateChangedCallback(NULL);
		WM_DefaultProc(pMsg);
		break;
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);

		NCode = pMsg->Data.v;
		switch (Id) {

		   /* Notifications sent by ID_IMAGE_VIEW_LIGHTS_RETURN */
		case ID_IMAGE_VIEW_LIGHTS_RETURN:
			switch (NCode) {
			/* User pressed return icon */
			case WM_NOTIFICATION_RELEASED:
				/* Unregister for RoomWidget status updates */
				RoomConfiguration_SetOnWidgetStateChangedCallback(NULL);
				WM_HideWindow(hThisWindow);
				break;
			}
			break;
			/* Notifications sent by ID_ICONVIEW_LIGHTS */
		case ID_LIGTHS_SWIPE_LIST:
			hItem = WM_GetDialogItem(pMsg->hWin, ID_LIGTHS_SWIPE_LIST);
			switch (NCode) {
			case WM_NOTIFICATION_CLICKED:
				break;
			case WM_NOTIFICATION_RELEASED:
				selectedLightWidget = SWIPELIST_GetReleasedItem(hItem);
				Lights_SetItemSelected(selectedLightWidget);
				break;
			}
			break;
		case ID_IMAGE_VIEW_LIGHTS_SELECTION:
			hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_VIEW_LIGHTS_SELECTION);
			switch (NCode) {
			case WM_NOTIFICATION_RELEASED:
				Lights_SetItemClicked(selectedLightWidget);
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



/* Static code */

/**
 * @brief  Inflate SwipeList widget with a names of RoomWidgets with a type of WIDGET_TYPE_LIGHT
 * @param  hItem: Handle of SwipeList widget
 */
static void Lights_InitRoomList(const WM_HWIN hItem) {
	if(!hItem) {
		return;
	}

	const int count = RoomConfiguration_GetLightWidgetCount();

	if (count > 0) {
		struct RoomWidget ** lights = RoomConfiguration_GetPointerArrayForWidgetType(WIDGET_TYPE_LIGHT);

		for (int i = 0; i < count; i++) {
			struct RoomWidget * lightWidget = lights[i];
			if (lightWidget) {
				/* Add list item with a name of the light widget */
				if((lightWidget->name)) {
					SWIPELIST_AddItem(hItem, (lightWidget->name), LIGHTS_LIST_ITEM_HEIGHT);
				} else {
					/* RoomWidget name is undefined. Use default */
					SWIPELIST_AddItem(hItem, LIGHTS_NO_NAME, LIGHTS_LIST_ITEM_HEIGHT);
				}
				/* Add item text to describe weather light widget is turned on or off */
				SWIPELIST_AddItemText(hItem, i, (lightWidget->boolValue == 1 ?
						LIGHTS_ITEM_TEXT_LIGHTS_ON : LIGHTS_ITEM_TEXT_LIGHTS_OFF));
			}

		}
		/* Background color of the swipe list widget is black
		 * at the area where is no list items
		 */
		int ySize = (count * LIGHTS_LIST_ITEM_HEIGHT) + count;
		if(ySize < _aDialogCreate[3].ySize) {
			WM_SetSize(hItem, _aDialogCreate[3].xSize, ySize);
		}
		free(lights);
	}
}

/**
 * @brief  Update SwipeList widget content. This function is called when remote
 * 		   device has made changes to RoomConfiguration.
 * @param  hItem: Handle of SwipeList widget
 */
static void Lights_UpdateRoomList(const WM_HWIN hItem) {
	if (!hItem) {
		return;
	}

	const int count = RoomConfiguration_GetLightWidgetCount();

	if(count == 0) {
		return;
	}
	struct RoomWidget ** lights = RoomConfiguration_GetPointerArrayForWidgetType(WIDGET_TYPE_LIGHT);
	if (!lights) {
		return;
	}
	for (int i = 0; i < count; i++) {
		struct RoomWidget * lightWidget = lights[i];
		if (lightWidget) {
			/* Update list item text to match the new status of the RoomWidget */
			SWIPELIST_SetText(hItem, i, 1, lightWidget->boolValue == 0 ?
							LIGHTS_ITEM_TEXT_LIGHTS_OFF :
							LIGHTS_ITEM_TEXT_LIGHTS_ON);
		}
	}
	/* Notify SwipeList widget that content is changed */
	WM_InvalidateWindow(hItem);

	free(lights);

}

/**
 * @brief  Change menu item bitmap
 * @param  position: Position of selected item in SwipeList widget
 */
static void Lights_SetItemClicked(const int position) {
	WM_HWIN hItemIcon = WM_GetDialogItem(hThisWindow, ID_IMAGE_VIEW_LIGHTS_SELECTION);
	WM_HWIN hItemList = WM_GetDialogItem(hThisWindow, ID_LIGTHS_SWIPE_LIST);

	int count = RoomConfiguration_GetLightWidgetCount();

	if(position < 0 || position > count || !hItemIcon || !hItemList || count == 0) {
		return;
	}

	struct RoomWidget ** lights = RoomConfiguration_GetPointerArrayForWidgetType(WIDGET_TYPE_LIGHT);
	struct RoomWidget * lightWidget = lights[position];

	if (lightWidget) {
		/* Change widget state to opposite to its current state */
		int newState = (lightWidget->boolValue == 0 ? 1 : 0);

		lightWidget->boolValue = newState;
		IMAGE_SetBitmap(hItemIcon, newState == 0? &bmBITMAP_lights_off : &bmBITMAP_lights_on);
		/* Change list item text to match the state of the widget */
		SWIPELIST_SetText(hItemList, position, 1, newState == 0? LIGHTS_ITEM_TEXT_LIGHTS_OFF : LIGHTS_ITEM_TEXT_LIGHTS_ON);
		WM_InvalidateWindow(hItemList);

	}

	free(lights);

}

/**
 * @brief  Set current selected item
 * @param  hItem: Handle of ICONVIEW widget
 */
static void Lights_SetItemSelected(const int position) {
	WM_HWIN hItemIcon = WM_GetDialogItem(hThisWindow, ID_IMAGE_VIEW_LIGHTS_SELECTION);
	WM_HWIN hItemText = WM_GetDialogItem(hThisWindow, ID_TEXT_LIGHTS_SELECTION);

	int count = RoomConfiguration_GetLightWidgetCount();

	if(position < 0 || position > count || !hItemIcon || count == 0 || !hItemText) {
		return;
	}
	/* Get pointer array of RoomWidgets with a type of WIDGET_TYPE_LIGHT */
	struct RoomWidget ** lights = RoomConfiguration_GetPointerArrayForWidgetType(WIDGET_TYPE_LIGHT);
	if (lights) {
		struct RoomWidget * lightWidget = lights[position];

		if (lightWidget) {
			IMAGE_SetBitmap(hItemIcon, lightWidget->boolValue == 0 ? &bmBITMAP_lights_off : &bmBITMAP_lights_on);
			TEXT_SetText(hItemText, lightWidget->name);
		}
	}

	/* Free allocated memory */
	free(lights);
}

/**
 * @brief  Function that is registered for the RoomWidget status changes at Lights_DialogCallback()
 * 		   function. When RoomConfiguration status updates are received, this function is called.
 * @param  widget: Pointer to the RoomWidget that status is changed
 */
void Lights_OnWidgetStateChangedCallback(struct RoomWidget * widget) {
	/* Client device has updated RoomWidget with a device type matching Light */
	//Lights_SetItemSelected(selectedLightWidget);
	Lights_UpdateRoomList(WM_GetDialogItem(hThisWindow, ID_LIGTHS_SWIPE_LIST));

}

/**
 * @brief  Create window that displays all available RoomWidgets with a type of WIDGET_TYPE_LIGHT
 * @param  None
 * @retval Handle of created window.
 */
WM_HWIN Lights_CreateLightsWindow(void) {

	hThisWindow = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), Lights_DialogCallback, WM_HBKWIN, 0, 0);
	Lights_SetItemSelected(0);


	return hThisWindow;
}
