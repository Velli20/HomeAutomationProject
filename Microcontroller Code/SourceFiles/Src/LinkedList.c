
#include "LinkedList.h"

void LinkedList_create(char *text, struct LINKED_LIST *list) {
	struct LIST_ITEM *ptr = malloc(sizeof(struct LIST_ITEM));

	list->head = ptr;
	list->curr = ptr;

	ptr->text = text;
	ptr->next = NULL;
}


void LinkedList_add(char *text, bool add_to_end, struct LINKED_LIST* list) {
	if (NULL == list || NULL == list->head) {
		LinkedList_create(text, list);
	    return;
	}

	struct LIST_ITEM *ptr = malloc(sizeof(struct LIST_ITEM));

	ptr->text = text;
	ptr->next = NULL;

	if (add_to_end) {
		list->curr->next = ptr;
		list->curr = ptr;
	} else {
		ptr->next = list->head;
		list->head = ptr;
	}
}
