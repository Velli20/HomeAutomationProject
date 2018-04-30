
#ifndef __ROOMWIDGET_H
#define __ROOMWIDGET_H

// RoomWidget

struct RoomWidget 
{
	char* name;

	uint32_t id;
	uint32_t type;

	uint32_t intValue;
	uint32_t intValueTarget;
	uint32_t status;
	uint32_t arduinoPin;
	uint8_t  boolValue;

	struct RoomWidget* next;
}
ROOM_WIDGET;

// RoomWidgetList

struct RoomWidgetList 
{
	struct RoomWidget* head;
    struct RoomWidget* current;
}
ROOM_WIDGET_LIST;

// Room

struct Room 
{
	uint32_t               id;
	char*                  name;
	struct RoomWidgetList* widgets;
	struct Room*           next;
}
ROOM;

// RoomList

struct RoomList 
{
	struct Room* head;
	struct Room* current;
}
ROOM_LIST;

// widget_list_add_to_room_list

void widget_list_add_to_room_list(struct RoomList* list, 
                                  struct Room*     room);

// widget_list_add_widget_to_room

void widget_list_add_widget_to_room(struct Room*       room, 
                                    struct RoomWidget* widget);

// widget_list_free

void widget_list_free(struct RoomList* roomListToFree);

// widget_list_get_room_count

uint32_t widget_list_get_room_count(struct RoomList* list);

// widget_list_get_room_widget_count

uint32_t widget_list_get_room_widget_count(struct RoomWidgetList * list);

// widget_list_find_widget_with_id

struct RoomWidget* widget_list_find_widget_with_id(struct RoomList* list, 
                                                  uint32_t         id);

// widget_list_find_parent_room

struct Room* widget_list_find_parent_room(struct RoomList*   list, 
                                             struct RoomWidget* target_widget);

// widget_list_delete_room

void widget_list_delete_room(struct RoomList* list, 
                             uint32_t         position);
						
#endif 
