#include "RoomWidget.h"





void RoomWidget_AddToRoomList(struct ROOMLIST * list, struct ROOM * room) {
	if(!room || !list ) {
		return;
	}

	if (!(list->head)) {
		list->head = room;
	} else {
		list->current->next = room;
	}
	list->current = room;
}


void RoomWidget_AddWidgetToRoom(struct ROOM * room, struct RoomWidget * widget) {
	if (!room || !widget) {
		return;
	}

	if (!(room->widgets)) {
		room->widgets = (struct RoomWidgetList *) malloc(sizeof(struct RoomWidgetList));
		(room->widgets)->head = widget;
	} else {
		((room->widgets)->current)->next = widget;
	}

	(room->widgets)->current = widget;
}


void RoomWidget_PopRoom(struct ROOMLIST * list, int position) {
	if(!list) {
		//printf("WidgetList_popRoom: error, list is null\n");
		return;
	} else if(!(list->head)){
		//printf("WidgetList_popRoom: error, list head pointer == NULL\n");
	}

	//printf("WidgetList_popRoom\n");

	struct ROOM* head = list->head;
	struct ROOM* previous = NULL;
	struct ROOM* roomToPop = NULL;
	struct ROOM* next = NULL;
	struct ROOM* last = NULL;

	int i = 0;

	while (head != NULL) {
		if ((i == position) && (i == 0)) {
			roomToPop = head;
			goto SKIP;
		} else if (i == (position - 1)) {
			previous = head;
		} else if (i == (position +1)) {
			next = head;
		} else if (i == position) {
			roomToPop = head;
			goto SKIP;
		}

		last = head;

		SKIP:
		i++;
		head = head->next;
	}
	if(last != NULL) {
		list->current = last;
	}

	if(roomToPop != NULL) {
		//printf("WidgetList_popRoom: room to pop id: %d\n", roomToPop->id);

		if(previous == NULL && next != NULL) {
			list->head = next;
		} else if(previous != NULL) {
			if(next != NULL) {
				previous->next = next;
			} else {
				previous->next = NULL;
			}
		}
		roomToPop->next = NULL;
		RoomWidget_FreeRoom(roomToPop);
		roomToPop = NULL;
	} else {
		//printf("WidgetList_popRoom: error, room to pop == NULL\n");
	}

}

void RoomWidget_FreeRoomList(struct ROOMLIST * roomListToFree) {
	if (roomListToFree == NULL) {
		//printf("WidgetList_freeRoomList: error, room list is null\n");
		return;
	}
	//printf("WidgetList_freeRoomList\n");

	if(roomListToFree->head != NULL) {
		RoomWidget_FreeRoom(roomListToFree->head);
		roomListToFree->head = NULL;

		if(roomListToFree->current != NULL) {
			roomListToFree->current = NULL;
		}
	}

	free(roomListToFree);
}

void RoomWidget_FreeRoom(struct ROOM * roomToFree) {
	if(roomToFree == NULL) {
		//printf("WidgetList_freeRoom: error, room is null\n");
		return;
	}
	//printf("WidgetList_freeWidget: room to free id: %d\n", roomToFree->id);

	if(roomToFree->name != NULL) {
		free(roomToFree->name);
	}
	if(roomToFree->widgets != NULL) {
		RoomWidget_FreeWidgetList(roomToFree->widgets);
		roomToFree->widgets = NULL;
	}
	if(roomToFree->next != NULL) {
		RoomWidget_FreeRoom(roomToFree->next);
		roomToFree->next = NULL;
	}
	free(roomToFree);
}

void RoomWidget_FreeWidgetList(struct RoomWidgetList * listToFree) {
	if(listToFree == NULL) {
		//printf("WidgetList_freeWidgetList: error, widget list is null\n");
		return;
	}
	//printf("WidgetList_freeWidgetList\n");

	if(listToFree->head != NULL) {
		RoomWidget_FreeWidget(listToFree->head);
		listToFree->head = NULL;
		if(listToFree->current != NULL) {
			listToFree->current = NULL;
		}
	}
	free(listToFree);
}

void RoomWidget_FreeWidget(struct RoomWidget * widgetToFree) {
	if(widgetToFree == NULL) {
		//printf("WidgetList_freeWidget: error, widget is null\n");
		return;
	}

	//printf("WidgetList_freeWidget: widget to free id: %d\n", widgetToFree->id);

	if(widgetToFree->name != NULL) {
		free(widgetToFree->name);
		widgetToFree->name = NULL;
	}
	if(widgetToFree->next != NULL) {
		RoomWidget_FreeWidget(widgetToFree->next);
		free(widgetToFree->next);
		widgetToFree->next = NULL;
	}
}


int RoomWidget_GetRoomCount(struct ROOMLIST * list) {
	if (list == NULL) {
		//printf("WidgetList_getRoomCount list NULL \n");
		return 0;
	}
	struct ROOM* ptr = list->head;
	int count = 0;
	while (ptr != NULL) {
		ptr = ptr->next;
		count++;
	}
	return count;
}

int RoomWidget_GetWidgetCount(struct RoomWidgetList * list) {
	if (list == NULL) {
		//printf("WidgetList_getWidgetCount list NULL \n");
		return 0;
	}
	struct RoomWidget* ptr = list->head;
	int count = 0;
	while (ptr != NULL) {
		ptr = ptr->next;
		count++;
	}
	return count;
}






