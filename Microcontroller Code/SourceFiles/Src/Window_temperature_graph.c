/* Includes */
#include "DIALOG.h"
#include "main.h"
#include "BITMAPS.h"

#include <math.h>
/* Defines */
#define ID_WINDOW_0  (GUI_ID_USER + 0x00)
#define ID_IMAGE_0  (GUI_ID_USER + 0x01)
#define ID_TEXT_0  (GUI_ID_USER + 0x02)
#define ID_GRAPH_0  (GUI_ID_USER + 0x03)

#define ID_IMAGE_0_IMAGE_0  0x00

#define PI 3.1415926f
#define DEG2RAD (PI / 180)

/* Funtion prototypes */
WM_HWIN Temperature_CreateTemperatureWindow(void);

/* Static data */
static WM_HWIN hThisWindow;

static GUI_CONST_STORAGE GUI_COLOR Colors[] = { 0xFFFFFF, 0x0000AA };

static GUI_CONST_STORAGE GUI_LOGPALETTE Pal = { 2, 1, &Colors[0] };

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
		{ WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 1, 0, 480, 272, 0, 0x0,
				0 }, { IMAGE_CreateIndirect, "Image", ID_IMAGE_0, 10, 10, 36,
				36, 0, 0, 0 }, { TEXT_CreateIndirect, "Lämpötila ", ID_TEXT_0,
				56, 23, 80, 20, 0, 0x0, 0 }, { GRAPH_CreateIndirect, "Graph",
				ID_GRAPH_0, 10, 92, 460, 170, 0, 0x0, 0 }, };
static const int temp[] = { 18, 21, 22, 19, 22, 18, 17, 17, 17, 17, 18, 18, 18, 18, 21, 22, 23, 23, 22, 22, 21, 21, 22, 24 };
static GUI_POINT _aPoint[1][24];
GRAPH_DATA_Handle _ahDataXY[3];

/* Static code */
static void _InitPoints() {
	int i;

	for (i = 0; i < 24; i++) {
		_aPoint[0][i].x = i + (int)(i * 19.16f);
		_aPoint[0][i].y = (int)(temp[i] * 5.6666666666666666666666666666667f);
	}
}

static int _OwnerDraw(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo) {
	switch (pDrawItemInfo->Cmd) {
	case WIDGET_ITEM_DRAW:
		//GUI_DrawRect(pDrawItemInfo->x0 - 3, pDrawItemInfo->y0 - 3, pDrawItemInfo->x0 + 3, pDrawItemInfo->y0 + 3);
		break;
	}
	return 0;
}

static const void * _GetImageById(U32 Id, U32 * pSize) {
	return NULL;
}

static void _cbDialog(WM_MESSAGE * pMsg) {
	const void * pData;
	WM_HWIN hItem;
	U32 FileSize;
	int NCode;
	int Id;
	int i;

	switch (pMsg->MsgId) {
	case WM_INIT_DIALOG:

		hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_0);
		IMAGE_SetBitmap(hItem, &bmBITMAP_return);

		hItem = WM_GetDialogItem(pMsg->hWin, ID_GRAPH_0);

		_InitPoints();

		//GRAPH_SetGridVis(hItem, 1);
		GRAPH_SetLineStyleH(hItem, GUI_LS_DASH);
		GRAPH_SetLineStyleV(hItem, GUI_LS_DASH);
		GRAPH_SetGridDistX(hItem, 115);
		GRAPH_SetGridDistY(hItem, 42.5f);

		GRAPH_SetColor(hItem, GUI_WHITE, GRAPH_CI_BK);
		GRAPH_SCALE_Handle hVerticalScale = GRAPH_SCALE_Create(20, GUI_TA_RIGHT, GRAPH_SCALE_CF_VERTICAL, 20);
		GRAPH_SCALE_Handle hHorizontalScale = GRAPH_SCALE_Create(160, GUI_TA_LEFT, GRAPH_SCALE_CF_HORIZONTAL, 40);

		GRAPH_SCALE_SetTextColor(hVerticalScale, GUI_BLACK);
		GRAPH_SCALE_SetTextColor(hHorizontalScale, GUI_BLACK);

		GRAPH_SCALE_SetFactor(hVerticalScale, 0.17647058823529411764705882352941f);
		GRAPH_SCALE_SetFactor(hHorizontalScale, 0.05219206680584551148225469728601f);

		GRAPH_AttachScale(hItem, hVerticalScale);
		GRAPH_AttachScale(hItem, hHorizontalScale);

		_ahDataXY[0] = GRAPH_DATA_XY_Create(GUI_LIGHTBLUE, 24,_aPoint[0], 24);
		GRAPH_DATA_XY_SetLineStyle(_ahDataXY[0], GUI_LS_SOLID);
		GRAPH_AttachData(hItem, _ahDataXY[0]);

		break;

	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch (Id) {
		/* Notifications sent by 'Image' */
		switch (NCode) {
		case WM_NOTIFICATION_CLICKED:
			break;
		case WM_NOTIFICATION_RELEASED:
			if (hThisWindow != NULL) {
				WM_HideWindow(hThisWindow);
				hThisWindow = NULL;
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

/* Public code */

WM_HWIN Temperature_CreateTemperatureWindow(void) {
	WM_MOTION_Enable(1);
	hThisWindow = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
	return hThisWindow;
}


/* End of file */
