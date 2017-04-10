#ifndef __ROOMWIDGET_H
#define __ROOMWIDGET_H

/* Includes */
#include <stdio.h>

struct RoomList {
	struct Room * head;
	struct Room * current;
};

struct Room {
	int id;
	char * name;
	struct RoomWidgetList * widgets;
	struct Room * next;
};

struct RoomWidgetList {
	struct RoomWidget * head;
    struct RoomWidget * current;
};

struct RoomWidget {
	char * name;

	int id;
	int type;

	int intValue;
	int intValueTarget;
	int boolValue;
	int status;
	int arduinoPin;

	struct RoomWidget * next;
};

/* Public function prototypes */
void RoomWidget_AddToRoomList(struct RoomList * list, struct Room * room);
void RoomWidget_AddWidgetToRoom(struct Room * room, struct RoomWidget * widget);


void RoomWidget_FreeRoomList(struct RoomList * roomListToFree);
void RoomWidget_FreeRoom(struct Room * roomToFree);
void RoomWidget_FreeWidgetList(struct RoomWidgetList * listToFree);
void RoomWidget_FreeWidget(struct RoomWidget * widgetToFree);

int RoomWidget_GetRoomCount(struct RoomList * list);
int RoomWidget_GetWidgetCount(struct RoomWidgetList * list);

struct RoomWidget * RoomWidget_GetRoomWidgetWithId(struct RoomWidgetList * list, int id);
struct Room * RoomWidget_GetRoomForRoomWidget(struct RoomWidgetList * list, struct RoomWidget * widget);

void RoomWidget_PopRoom(struct RoomList * list, int position);
#endif /* __ROOMWIDGET_H */

/* End of file */
