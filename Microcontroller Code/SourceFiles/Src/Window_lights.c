/* Includes */
#include "DIALOG.h"
#include "main.h"
#include "BITMAPS.h"

/* Defines */
#define ID_WINDOW_LIGHTS  				(GUI_ID_USER + 0x00)
#define ID_IMAGE_VIEW_LIGHTS_RETURN  	(GUI_ID_USER + 0x01)
#define ID_TEXT_LIGHTS_RETURN  			(GUI_ID_USER + 0x02)
#define ID_ICONVIEW_LIGHTS  			(GUI_ID_USER + 0x03)

/* Private typedef */
typedef struct {
	const GUI_BITMAP * pBitmap;
	const char * pText;
} BITMAP_ITEM;

/* Function prototypes */
static void Lights_SetItemSelected(WM_HWIN hItem, int selected);
WM_HWIN Lights_CreateLightsWindow(void);

/* Static data */
static WM_HWIN hThisWindow;

static const BITMAP_ITEM mBitmapsLightsOn[] = { { &bmBITMAP_lights_on, "Olohuone" }, {
		&bmBITMAP_lights_on, "Makuuhuone" }, { &bmBITMAP_lights_on, "Eteinen" },};
static const BITMAP_ITEM mBitmapsLightsOff[] = { { &bmBITMAP_lights_off, "Olohuone" }, {
		&bmBITMAP_lights_off, "Makuuhuone" }, { &bmBITMAP_lights_off, "Eteinen" },};

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
		{ WINDOW_CreateIndirect, "Window", ID_WINDOW_LIGHTS, 1, 0, 480, 240, 0, 0x0, 0 },
		{ IMAGE_CreateIndirect, "Return", ID_IMAGE_VIEW_LIGHTS_RETURN, 10, 10, 48, 48, 0, 0, 0 },
		{ TEXT_CreateIndirect, "Valaistus", ID_TEXT_LIGHTS_RETURN, 65, 24, 80, 20, 0, 0x0, 0 },
		{ ICONVIEW_CreateIndirect, "Lights", ID_ICONVIEW_LIGHTS, 66, 78, 348, 156, 0, 0x00600060, 0 },};

/* Static code */
static void Lights_DialogCallback(WM_MESSAGE * pMsg) {
	const void * pData;
	WM_HWIN hItem;
	int NCode;
	int Id;
	int selection;

	switch (pMsg->MsgId) {
	case WM_INIT_DIALOG:

		/* Initialization of return icon */
		hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_VIEW_LIGHTS_RETURN);
		IMAGE_SetBitmap(hItem, &bmBITMAP_return);

		/* Initialization of title */
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_LIGHTS_RETURN);
		TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
		TEXT_SetFont(hItem, GUI_FONT_13B_1);

		/* Initialization of icon view that displays light buttons */
		hItem = WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_LIGHTS);
		ICONVIEW_SetFont(hItem, GUI_FONT_13B_1);
		ICONVIEW_SetSel(hItem, -1);

		int i;
		for (i = 0; i < GUI_COUNTOF(mBitmapsLightsOn); i++) {
			ICONVIEW_AddBitmapItem(hItem, mBitmapsLightsOn[i].pBitmap, mBitmapsLightsOn[i].pText);
			ICONVIEW_SetTextColor(hItem, i, 0x000000);
		}

		break;
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);

		NCode = pMsg->Data.v;
		switch (Id) {

		   /* Notifications sent by ID_IMAGE_VIEW_LIGHTS_RETURN */
		case ID_IMAGE_VIEW_LIGHTS_RETURN:
			switch (NCode) {
			case WM_NOTIFICATION_CLICKED:
				break;
			case WM_NOTIFICATION_RELEASED:
				WM_HideWindow(hThisWindow);
				WM_DeleteWindow(hThisWindow);
				break;
			}
			break;
			/* Notifications sent by ID_ICONVIEW_LIGHTS */
		case ID_ICONVIEW_LIGHTS:
			hItem = WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_LIGHTS);
			switch (NCode) {
			case WM_NOTIFICATION_CLICKED:
				Lights_SetItemSelected(hItem, 1);
				break;
			case WM_NOTIFICATION_RELEASED:
				Lights_SetItemSelected(hItem, 0);
				selection = ICONVIEW_GetSel(hItem);
				break;
			}

		}
		break;
	default:
		WM_DefaultProc(pMsg);
		break;
	}
}


/**
 * @brief  Change menu item bitmap
 * @param  hItem: Handle of ICONVIEW widget
 * @param  selected: 1 if item is clicked or 0 if item is released
 */
static void Lights_SetItemSelected(WM_HWIN hItem, int selected) {
	int selection = ICONVIEW_GetSel(hItem);
	if(selection < 0 || selection > 2 || hItem == NULL) {
		return;
	}

	if(selected) {
		ICONVIEW_SetBitmapItem(hItem, selection, mBitmapsLightsOn[selection].pBitmap);
	} else {
		ICONVIEW_SetBitmapItem(hItem, selection, mBitmapsLightsOff[selection].pBitmap);
	}
}


WM_HWIN Lights_CreateLightsWindow(void) {

	hThisWindow = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), Lights_DialogCallback, WM_HBKWIN, 0, 0);
	return hThisWindow;
}
