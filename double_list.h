#ifndef DOUBLE_LIST_H
#define DOUBLE_LIST_H

#include <stdlib.h>

#define DOUBLE_LIST_GET_NEXT(double_list_ptr, typename) (typename *)double_list_ptr->next
#define DOUBLE_LIST_GET_PREV(double_list_ptr, typename) (typename *)double_list_ptr->prev

typedef struct double_list {
	struct double_list *next;
	struct double_list *prev;
} double_list;

int double_list_init(double_list *root) {
	int ret = 0;
	if (root == NULL) {
		ret = -1;
		goto done;
	}
	root->next = NULL;
	root->prev = NULL;
done:
	return ret;
}

int double_list_insert_before(double_list *at, 
		double_list *new_list_rec) {
	int ret = 0;
	if (!at || !new_list_rec) {
		ret = -1;
		goto done;
	}
	if (at->prev != NULL) {
		at->prev->next = new_list_rec;
	}
	new_list_rec->prev = at->prev;
	at->prev = new_list_rec;
	new_list_rec->next = at;
done:
	return ret;
}

int double_list_insert_after(double_list *at,
		double_list *new_list_rec) {
	int ret = 0;
	if (!at || !new_list_rec) {
		ret = -1;
		goto done;
	}
	if (at->next) {
		at->next->prev = new_list_rec;
	}
	new_list_rec->next = at->next;
	at->next = new_list_rec;
	new_list_rec->prev = at;
done:
	return ret;
}

int double_list_insert(double_list *root,
		unsigned int index,
		double_list *new_list_entry,
		unsigned int offset) {
	int ret = 0;
	if (root == NULL || index < -1 || new_list_entry == NULL) {
		ret = -1;
		goto done;
	}
	unsigned int i = 0;
	unsigned int index_lim = index-1;
	double_list *prev_list_entry = root;
	while (i < index_lim && prev_list_entry != NULL) {
		i++;
		prev_list_entry = prev_list_entry->next;
	}
	if (prev_list_entry == NULL) {
		ret = -2;
		goto done;
	}
	new_list_entry->next = prev_list_entry->next;
	prev_list_entry->next->prev = new_list_entry;
	prev_list_entry->next = new_list_entry;
	new_list_entry->prev = prev_list_entry - offset;
done:
	return ret;
}



int double_list_insert_at_tail(double_list *root,
		double_list *new_tail) {
	int ret = 0;
	if (root == NULL || new_tail == NULL) {
		goto done;
	}
	double_list *tail = root;
	while (tail->next != NULL) {
		tail = tail->next;
	}
	tail->next = new_tail;
done:
	return ret;
}

int double_list_remove(double_list *element) {
	int ret = 0;
	if (!element) {
		ret = -1;
		goto done;
	}
	if (element->next) {
		element->next->prev = element->prev;
	}
	if (element->prev) {
		element->prev->next = element->next;
	}
done:
	return ret;
}

#endif
