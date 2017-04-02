/* Includes */
#include "RoomWidget.h"


/* Public code */

/**
 * @brief  Link Room struct at the end of the RoomList linked list
 * @param  list: Pointer to the RoomList struct to add the Room struct
 * @param  room: Pointer to the Room struct to add at the end of the list
 * @retval None
 */
void RoomWidget_AddToRoomList(struct RoomList * list, struct Room * room) {
	if (!room || !list) {
		return;
	}

	if (!(list->head)) {
		list->head = room;
	} else if(list->current){
		list->current->next = room;
	}
	list->current = room;
}

/**
 * @brief  Link pointer of the RoomWidget struct to the Room struct
 * @param  widget: Pointer of the RoomWidget struct to be linked to Room struct
 * @param  room: Pointer to the Room struct to add at the end of the list
 * @retval None
 */
void RoomWidget_AddWidgetToRoom(struct Room * room, struct RoomWidget * widget) {
	if (!room || !widget) {
		return;
	}

	if (!(room->widgets->head)) {
		room->widgets->head = widget;
	} else if(room->widgets->current){
		((room->widgets)->current)->next = widget;
	}

	room->widgets->current = widget;
}


/**
 * @brief  Unlink at Room struct position and free
 * 		   all allocated memory related to this pointer
 * @param  list: Pointer to the RoomList struct
 * @param  position: Position of the RoomStruct to be unlinked
 * @retval None
 */
void RoomWidget_PopRoom(struct RoomList * list, int position) {
	if(!list || !(list->head)) {
		return;
	}

	struct Room* head = list->head;
	struct Room* previous = NULL;
	struct Room* roomToPop = NULL;
	struct Room* next = NULL;
	struct Room* last = NULL;

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
	if(last) {
		list->current = last;
	}

	if(roomToPop) {

		if(!previous && next) {
			list->head = next;
		} else if(previous) {
			if(next) {
				previous->next = next;
			} else {
				previous->next = NULL;
			}
		}
		roomToPop->next = NULL;
		RoomWidget_FreeRoom(roomToPop);
		roomToPop = NULL;
	}

}

/**
 * @brief  Free memory that is allocated for RoomList struct
 * @param  roomListToFree: Pointer to the RoomList struct to be freed
 * @retval None
 */
void RoomWidget_FreeRoomList(struct RoomList * roomListToFree) {
	if (!roomListToFree) {
		return;
	}

	if(roomListToFree->head) {
		RoomWidget_FreeRoom(roomListToFree->head);
		roomListToFree->head = NULL;
	}
	if (roomListToFree->current) {
		roomListToFree->current = NULL;
	}
	free(roomListToFree);
}

/**
 * @brief  Free memory that is allocated for Room struct
 * @param  roomToFree: Pointer to the Room struct to be freed
 * @retval None
 */
void RoomWidget_FreeRoom(struct Room * roomToFree) {
	if(!roomToFree) {
		return;
	}

	if((roomToFree->name)) {
		free(roomToFree->name);
		roomToFree->name = NULL;
	}
	if((roomToFree->widgets)) {
		RoomWidget_FreeWidgetList(roomToFree->widgets);
		roomToFree->widgets = NULL;
	}
	if((roomToFree->next)) {
		RoomWidget_FreeRoom(roomToFree->next);
		roomToFree->next = NULL;
	}
	free(roomToFree);
}

/**
 * @brief  Free memory that is allocated for RoomWidgetList struct
 * @param  listToFree: Pointer to the RoomWidgetList struct to be freed
 * @retval None
 */
void RoomWidget_FreeWidgetList(struct RoomWidgetList * listToFree) {
	if(!listToFree) {
		return;
	}

	if((listToFree->head)) {
		RoomWidget_FreeWidget(listToFree->head);
		listToFree->head = NULL;

	}
	if((listToFree->current)) {
		listToFree->current = NULL;
	}
	free(listToFree);
}

/**
 * @brief  Free memory that is allocated for RoomWidget struct
 * @param  widgetToFree: Pointer to the RoomWidget struct to be freed
 * @retval None
 */
void RoomWidget_FreeWidget(struct RoomWidget * widgetToFree) {
	if(!widgetToFree) {
		return;
	}

	if((widgetToFree->name)) {
		free(widgetToFree->name);
		widgetToFree->name = NULL;
	}
	if(widgetToFree->next) {
		RoomWidget_FreeWidget(widgetToFree->next);
		free(widgetToFree->next);
		widgetToFree->next = NULL;
	}
}

/**
 * @brief  Get amount of the Room structs linked to RoomList struct
 * @param  list: Pointer to the RoomWidget struct
 * @retval count of the pointers linked to RoomList struct
 */
int RoomWidget_GetRoomCount(struct RoomList * list) {
	if (!list) {
		return 0;
	}
	struct Room* ptr = list->head;
	int count = 0;
	while (ptr) {
		ptr = ptr->next;
		count++;
	}
	return count;
}

/**
 * @brief  Get amount of the RoomWidget structs linked to RoomWidgetList struct
 * @param  list: Pointer to the RoomWidgetList struct
 * @retval count of the pointers linked to RoomWidgetList struct
 */
int RoomWidget_GetWidgetCount(struct RoomWidgetList * list) {
	if (!list) {
		return 0;
	}
	struct RoomWidget* ptr = list->head;
	int count = 0;
	while (ptr) {
		ptr = ptr->next;
		count++;
	}
	return count;
}

/**
 * @brief  Get pointer for RoomWidget with id
 * @param  list: Pointer to RoomWidgetList
 * @param  id: Id of the RoomWidget
 * @retval Pointer to the RoomWidget
 */
struct RoomWidget * RoomWidget_GetRoomWidgetWithId(struct RoomWidgetList * list, int id) {
	if(!list) {
		return NULL;
	}

	struct Room* ptrRoom = list->head;

	while(ptrRoom) {
		if (ptrRoom->widgets) {

			struct RoomWidget* ptrWidget = ptrRoom->widgets->head;

			while (ptrWidget) {
				if (ptrWidget->id == id) {
					return ptrWidget;
				}
				ptrWidget = ptrWidget->next;
			}

		}
		ptrRoom = ptrRoom->next;
	}

	return NULL;
}


/* End of file */
