#include "main.h"
#include "DIALOG.h"
#include "BITMAPS.h"
#include "WM.h"

/* Defines */
#define ID_WINDOW_ROOM_CONFIGURATION 		(GUI_ID_USER + 0x00)
#define ID_TEXT_CONFIGURATION 				(GUI_ID_USER + 0x01)
#define ID_TEXT_CONFIGURATION_SCROLLBAR 	(GUI_ID_USER + 0x02)
#define ID_TEXT_RETURN  					(GUI_ID_USER + 0x03)
#define ID_IMAGE_VIEW_RETURN  				(GUI_ID_USER + 0x04)

#define FONT_HEIGHT 14

/* Variables */
extern char * roomConfigurationString;

/* Funtion prototypes */
WM_HWIN WindowRoomConfiguration_CreateWindow(void);

/* Static data */

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
		{ WINDOW_CreateIndirect, "Window", ID_WINDOW_ROOM_CONFIGURATION, 0, 0, 480, 272, 0, 0x0, 0 },
		{ IMAGE_CreateIndirect, "Image", ID_IMAGE_VIEW_RETURN,10, 10, 48, 48, 0, 0, 0 },
		{ TEXT_CreateIndirect, "Huone konfiguratio", ID_TEXT_RETURN, 65, 24, 140, 20, 0, 0x0, 0 },
		{ TEXT_CreateIndirect, "Text", ID_TEXT_CONFIGURATION, 0, 58, 440, 214, 0, 0x0, 0 },
		{ SCROLLBAR_CreateIndirect, "Scrollbar", ID_TEXT_CONFIGURATION_SCROLLBAR, 440, 58, 40, 214, 8, 0x0, 0 }, };

static WM_HWIN hThisWindow;

/* Private code */


/**
 * @brief  Measures the text height.
 * @param  text: Pointer to the text to measure
 * 		   fontHeight: Height of the font that text is drawn with.
 * @retval Approximate height of the text.
 */
static int WindowRoomConfiguration_GetTextHeight(char * text, int fontHeight) {
	if(!text) {
		return fontHeight;
	}
	int lenght = strlen(text);

	int i;
	int height = fontHeight;

	for (i=0; i < lenght; i++) {
		if(text[i] == '\n') {
			height += fontHeight;
		}
	}

	return height;
}

static void _cbDialog(WM_MESSAGE * pMsg) {
	WM_HWIN hItem;
	int NCode;
	int Id;
	int     ScrollValue;
	int     yPos;
	int i;

	switch (pMsg->MsgId) {
	case WM_INIT_DIALOG:
		/* Initialization of return icon */
		hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_VIEW_RETURN);
		IMAGE_SetBitmap(hItem, &bmBITMAP_return);

		/* Initialization of title */
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_RETURN);
		TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
		TEXT_SetFont(hItem, GUI_FONT_13B_1);


		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_CONFIGURATION);
		TEXT_SetFont(hItem, GUI_FONT_13B_1);

		/* roomConfigurationString != NULL */
		if(roomConfigurationString) {
			TEXT_SetText(hItem, roomConfigurationString);

			int textHeight = WindowRoomConfiguration_GetTextHeight(roomConfigurationString, FONT_HEIGHT);

			/* If texts calculated y size is higher than text widgets
			 * default value, then resize it
			 */
			if (textHeight > _aDialogCreate[3].ySize) {
				/* Set new y size of the text widget */
				WM_SetYSize(hItem, textHeight);

				/* Calculate the y size that is drawn below screen */
				int scrollBarHeight = textHeight - _aDialogCreate[3].ySize;

				SCROLLBAR_SetThumbSizeMin(30);
				SCROLLBAR_SetNumItems(WM_GetDialogItem(pMsg->hWin, ID_TEXT_CONFIGURATION_SCROLLBAR), scrollBarHeight);

			} else {
				/* Hide scroll bar if the texts calculated height
				 * is less than the text widget default height
				 */
				WM_HideWindow(WM_GetDialogItem(pMsg->hWin, ID_TEXT_CONFIGURATION_SCROLLBAR));
			}
		} else {
			/* String is null, hide scroll bar */
			WM_HideWindow(WM_GetDialogItem(pMsg->hWin, ID_TEXT_CONFIGURATION_SCROLLBAR));
		}



		break;
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;

		switch (Id) {

		case ID_IMAGE_VIEW_RETURN:
			switch (NCode) {
			case WM_NOTIFICATION_CLICKED:
				break;
			case WM_NOTIFICATION_RELEASED:
				WM_HideWindow(hThisWindow);
				break;
			}
			break;
		case ID_TEXT_CONFIGURATION_SCROLLBAR:
			switch (NCode) {
			case WM_NOTIFICATION_CLICKED:
				break;
			case WM_NOTIFICATION_RELEASED:
				break;
			case WM_NOTIFICATION_VALUE_CHANGED:
				hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_CONFIGURATION_SCROLLBAR);

				ScrollValue = (SCROLLBAR_GetValue(hItem));

				hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_CONFIGURATION);
				yPos = _aDialogCreate[3].y0 - ScrollValue;
				WM_MoveTo(hItem, 0, yPos);

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

/*  Public code */

WM_HWIN WindowRoomConfiguration_CreateWindow(void) {
	hThisWindow = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate),_cbDialog, WM_HBKWIN, 0, 0);
	return hThisWindow;
}

/* End of file */
