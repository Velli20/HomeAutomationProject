// Includes

#include <stdio.h>
#include <stdlib.h>
#include "RoomWidget.h"

// widget_list_add_to_room_list

void widget_list_add_to_room_list(struct RoomList* list, 
                                  struct Room*     room) 
{
	if ( !room || !list ) 
		return;

	if ( !(list->head) ) 
		list->head= room;
	else if ( list->current )
		list->current->next= room;
	
	list->current = room;
}

// widget_list_add_widget_to_room

void widget_list_add_widget_to_room(struct Room*       room, 
                                    struct RoomWidget* widget) 
{
	if ( !room || !widget ) 
		return;

	if ( !(room->widgets->head) ) 
		room->widgets->head= widget;

	else if ( room->widgets->current )
		((room->widgets)->current)->next= widget;

	room->widgets->current= widget;
}

// widget_list_delete_room

void widget_list_delete_room(struct RoomList* list, 
                             uint32_t         position) 
{
	struct Room* head;
	struct Room* previous;
	struct Room* roomToPop;
	struct Room* next;
	uint32_t     i;

	if ( !list || !(list->head) ) 
		return;

	if ( position == 0 ) {
		roomToPop= list->head;
		goto delete;
	} 

	head=      list->head;
	previous=  0;
	roomToPop= 0;
	i=         0;
	do
	{
		if ( i == (position - 1) ) 
		{
			previous=  head;
			roomToPop= head->next;
			break;
		}

		i++;
		head= head->next;
	}
	while ( head && head != list->head );

	if ( !roomToPop )
	    return;

	delete:

	next= roomToPop->next;

	if ( roomToPop == list->current )
		list->current= previous ? previous : list->head;
	
	if ( !previous ) 
		list->head= next ? next : 0;
	else
		previous->next= next ? next : 0;

	if ( roomToPop->name )
	    free(roomToPop->name);

	free(roomToPop);
}

// widget_list_free_room_widgets

static void widget_list_free_room_widgets(struct RoomWidgetList* listToFree) 
{
	struct RoomWidget* item;
	struct RoomWidget* previous_item;

	if ( !listToFree ) 
		return;

	item= listToFree->head;
	while ( item )
	{
		if ( item->name )
		    free(item->name);

		previous_item= item;
		item=          item->next;

		free(previous_item);
	}
	
	free(listToFree);
}

// widget_list_free

void widget_list_free(struct RoomList* roomListToFree) 
{
	struct Room* item;
	struct Room* previous_item;

	if ( !roomListToFree ) 
		return;

	item= roomListToFree->head;
	while ( item )
	{
		if ( item->name )
		    free(item->name);

		if ( item->widgets )
		    widget_list_free_room_widgets(item->widgets);

		previous_item= item;
		item=          item->next;

		free(previous_item);
	}
	free(roomListToFree);
}

// widget_list_get_room_count

uint32_t widget_list_get_room_count(struct RoomList* list) 
{
	struct Room* item;
	uint32_t     count;

	if ( !list ) 
		return 0;
	
	for ( item= list->head, count= 0; item && item != list->head; item= item->next, count++ );

	return(count);
}

// widget_list_get_room_widget_count

uint32_t widget_list_get_room_widget_count(struct RoomWidgetList* list) 
{
	struct RoomWidget* item;
	uint32_t           count;

	if ( !list ) 
		return 0;
	
	for ( item= list->head, count= 0; item && item != list->head; item= item->next, count++ );

	return(count);
}

// widget_list_find_widget_with_id

struct RoomWidget* widget_list_find_widget_with_id(struct RoomList* list, 
                                                   uint32_t         id) 
{
	struct Room*       room;
	struct RoomWidget* widget;

	if ( !list )
		return(0);

	for ( room= list->head; room; room= room->next ) 
	{
		if ( !room->widgets )
		    continue;

		for ( widget= room->widgets->head; widget; widget= widget->next )
		{
			if ( widget->id == id )
		         return(widget);
		}
	}

	return(0);
}

// widget_list_find_parent_room

struct Room* widget_list_find_parent_room(struct RoomList*   list, 
                                          struct RoomWidget* target_widget) 
{ 
	struct Room*       room;
	struct RoomWidget* widget;

	if ( !list || !target_widget )
		return(0);

	for ( room= list->head; room; room= room->next )
	{
		if ( !room->widgets )
		    continue;

		for ( widget= room->widgets->head; widget; widget= widget->next )
		{
			if ( widget->id == target_widget->id )
			    return(room);
		}
	}

	return(0);
}
