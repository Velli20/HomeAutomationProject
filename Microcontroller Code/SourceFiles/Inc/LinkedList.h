#ifndef __LINKEDLIST_H
#define __LINKEDLIST_H

#include<stdlib.h>
#include<stdbool.h>
#include<stdio.h>

struct LIST_ITEM {
	const char       *text;
	struct LIST_ITEM *next;
};


struct LINKED_LIST {
	struct LIST_ITEM *head;
	struct LIST_ITEM *curr;
};

void LinkedList_create(char *text, struct LINKED_LIST *list);
void LinkedList_add(char *text, bool add_to_end, struct LINKED_LIST* list);

#endif /*__LINKEDLIST_H*/
