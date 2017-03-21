#include "main.h"

#ifndef __ROOMWIDGET_H
#define __ROOMWIDGET_H


struct ROOMLIST {
	struct ROOM * head;
	struct ROOM * current;
};

struct ROOM {
	int id;
	char * name;
	struct RoomWidgetList * widgets;
	struct ROOM * next;
};

struct RoomWidgetList {
	struct RoomWidget * head;
    struct RoomWidget * current;
};

struct RoomWidget {
	int id;
	int type;
	char * name;
	int intValue;
	int boolValue;
	int status;

	struct RoomWidget * next;
};

void RoomWidget_CreateRoom(struct ROOMLIST * list, struct ROOM *room);
void RoomWidget_AddToRoomList(struct ROOMLIST * list, struct ROOM * room);

void RoomWidget_AddWidgetToRoom(struct ROOM * room, struct RoomWidget * widget);
void RoomWidget_PrintRoomList(struct ROOMLIST * list);
void RoomWidget_PopRoom(struct ROOMLIST * list, int position);

void RoomWidget_FreeRoomList(struct ROOMLIST * roomListToFree);
void RoomWidget_FreeRoom(struct ROOM * roomToFree);
void RoomWidget_FreeWidgetList(struct RoomWidgetList * listToFree);
void RoomWidget_FreeWidget(struct RoomWidget * widgetToFree);

int RoomWidget_GetRoomCount(struct ROOMLIST * list);
int RoomWidget_GetWidgetCount(struct RoomWidgetList * list);

#endif /* __ROOMWIDGET_H */

/* END OF FILE */
