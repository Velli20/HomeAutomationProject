/* Includes */
#include "DIALOG.h"
#include "main.h"
#include "BITMAPS.h"
#include "RoomConfiguration.h"
#include "RoomWidget.h"


/* Funtion prototypes */
void Commands_PrintCommandsList(WM_HWIN * listView);
WM_HWIN Commands_CreateCommandsWindow(void);

/* Defines */
#define ID_WINDOW_0  (GUI_ID_USER + 0x00)
#define ID_BUTTON_0  (GUI_ID_USER + 0x01)
#define ID_LISTVIEW_0  (GUI_ID_USER + 0x02)

/* Static data */
static WM_HWIN hThisWindow;
/* LINKED_LIST declared in main.c */
extern struct LINKED_LIST UartCommands;

/* ROOMLIST declared in RoomConfiguration.c */
extern struct RoomList* roomConfiguration;


static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
		{ WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 480, 272, 0, 0x0,
				0 }, { IMAGE_CreateIndirect, "Return", ID_BUTTON_0, 11, 10, 80,
				72, 0, 0x0, 0 }, { LISTVIEW_CreateIndirect, "Listview",
				ID_LISTVIEW_0, 22, 82, 400, 117, 0, 0x0, 0 }, };

/* Static code */

static void _cbDialog(WM_MESSAGE * pMsg) {
	WM_HWIN hItem;
	int NCode;
	int Id;
	switch (pMsg->MsgId) {
	case WM_DELETE:
		hThisWindow = NULL;
		break;
	case WM_INIT_DIALOG:
		/* Initialization of 'Listview' */

		hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
		LISTVIEW_AddColumn(hItem, 60, "Päivämäärä",GUI_TA_HCENTER | GUI_TA_VCENTER);
		LISTVIEW_AddColumn(hItem, 40, "Aika", GUI_TA_HCENTER | GUI_TA_VCENTER);
		LISTVIEW_AddColumn(hItem, 300, "Komento",GUI_TA_HCENTER | GUI_TA_VCENTER);
		LISTVIEW_AddRow(hItem, NULL);
		LISTVIEW_SetGridVis(hItem, 1);
		Commands_PrintCommandsList(hItem);
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
		IMAGE_SetBitmap(hItem, &bmBITMAP_return);
		break;
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch (Id) {

		/* Notifications sent by 'Button' */
		case ID_BUTTON_0:
			switch (NCode) {
			case WM_NOTIFICATION_CLICKED:
				break;
			case WM_NOTIFICATION_RELEASED:
				WM_DeleteWindow(hThisWindow);
				break;
			}
			break;

		/* Notifications sent by 'Listview' */
		case ID_LISTVIEW_0:
			switch (NCode) {
			case WM_NOTIFICATION_CLICKED:
				break;
			case WM_NOTIFICATION_RELEASED:
				break;
			case WM_NOTIFICATION_SEL_CHANGED:
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

WM_HWIN Commands_CreateCommandsWindow(void) {

	hThisWindow = GUI_CreateDialogBox(_aDialogCreate,
			GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
	return hThisWindow;
}



void Commands_PrintCommandsList(WM_HWIN * listView) {

	struct LIST_ITEM* ptr = UartCommands.head;

	int row = 0;

	while (ptr != NULL) {
		LISTVIEW_SetItemText(listView, 2, row, ptr->text);
		ptr = ptr->next;
		row++;
		if (ptr != NULL) {
			LISTVIEW_AddRow(listView, NULL);
		}
	}
	if(roomConfiguration && roomConfiguration->current && roomConfiguration->current->widgets && roomConfiguration->current->widgets->current) {

		if(roomConfiguration->current->widgets->current->name) {
			LISTVIEW_AddRow(listView, NULL);
			LISTVIEW_SetItemText(listView, 2, row, roomConfiguration->current->widgets->current->name);
		}
	}
}


/* End of file */
