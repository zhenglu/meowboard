/*
 *   Copyright (c) 2007-2008 C3SL.
 *
 *   This file is part of Bosh.
 *
 *   Bosh is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   any later version.
 *
 *   Bosh is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 */


#ifndef lista_h
#define lista_h

#include <stdlib.h>

#include "allocator.h"

/* Macro used to iterate through list elements */
#define list_foreach(it, l) for(it = list_begin(l); it != list_end(l); it = list_next(it))


/*! \brief A callback function that receies one element of the list */
typedef void(*proc)(void *v);

/*! \brief A callback function that compares two elements in the list */
typedef int(*compare_function)(void *v1,void *v2);

/*! \brief A node in the list */
typedef struct _list_node {
	struct _list_node *next, *prev;
	void *value;
} _list_node;

/*! \brief The list */
typedef struct list {
	_list_node head;
} list;

/*! \brief The list iterator's */
typedef _list_node* list_iterator;

DECLARE_ALLOCATOR(_list_node)
DECLARE_ALLOCATOR(list)

/*! \brief Create a new list */
static inline list* list_new() {
	list *l = list_alloc(sizeof(list));
	if(l == NULL)
		return NULL;
    l->head.next = l->head.prev = &l->head;
	return l;
}

/*! \brief Returns non-zero if the list is empty */
static inline int list_empty(list* l) {
    return &l->head == l->head.next;
}

/*! \brief Returns non-zero if the list has no more than one element */
static inline int list_single(list* l) {
    return &l->head == l->head.next->next;
}

/*! \brief Insert an element after the element pointed by the iterator*/
static inline list_iterator list_insert(list_iterator pos, void* value) {
    _list_node* node;

    node = _list_node_alloc();
    node->next = pos;
    node->prev = pos->prev;
    node->next->prev = node;
    node->prev->next = node;
    node->value = value;

	return node;
}

/*! \brief Erases the element pointed by the iterator 
 *
 * \return Returns the removed element
 */
static inline void* list_erase(list_iterator iterator) {
    void* value = iterator->value;

    iterator->prev->next = iterator->next;
    iterator->next->prev = iterator->prev;
    _list_node_free(iterator);

    return value;
}

/*! \brief Insert an element in the end of the list */
static inline list_iterator list_push_back(list* l, void *v) {
    return list_insert(&l->head, v);
}

/*! \brief Remove and return the last element */
static inline void* list_pop_back(list* l) {
    return list_erase(l->head.prev);
}

/*! \brief Insert an element in the begining of the list */
static inline list_iterator list_push_front(list* l,void *v) {
    return list_insert(l->head.next, v);
}

/*! \brief Remove and return the first element of the list */
static inline void *list_pop_front(list* l) {
    return list_erase(l->head.next);
}

/*! \brief Returns the first element */
static inline void* list_front(list* l) {
    return l->head.next->value;
}

/*! \brief Returns the last element */
static inline void* list_back(list* l) {
    return l->head.prev->value;
}

/*! \brief Return an iterator to the first element */
static inline list_iterator list_begin(list* l) {
    return l->head.next;
}

/*! \brief Return an iterator to the end of the list
 *
 * The returned iterator is not dereferenceable.
 */
static inline list_iterator list_end(list* l) {
    return &l->head;
}

/*! \breif Returns the iterator to the next element */
static inline list_iterator list_next(list_iterator iterator) {
    return iterator->next;
}

/*! \brief Returns the element pointed by the iterator */
static inline void* list_iterator_value(list_iterator iterator) {
    return iterator->value;
}

/*! \brief Erase all elements of the list */
static inline void list_clear(list* l, proc delete_function) {
    void* v;

    while(!list_empty(l)) {
        v = list_pop_front(l);
        if(delete_function != NULL)
            delete_function(v);
    }
}

/*! \brief Destroy a list
 *
 * \param l is the list to be destroyed
 * \param delete_function is a callback used
 *          to free the elements in the list,
 *          is NULL it will not be used.
 */
static inline void list_delete(list *l, proc delete_function) {
    list_clear(l, delete_function);
    list_free(l);
}

/*! \breif Find an element in the list
 *
 * \param l is the list
 * \param v is the element to be found
 * \param compare_function is the function to be used to compare v to the elements in the list.
 */
static inline list_iterator list_find(list* l, void* v, compare_function func) {
    _list_node* node;

    for(node = l->head.next; node != &l->head; node = node->next) {
        if(func(node->value, v)) {
            return node;
        }
    }
    return list_end(l);
}

/*! \brief Returns the number of elements in the list, this takes linear time */
static inline int list_size(list *l) {
    int size = 0;
    list_iterator it;
    list_foreach(it, l) {
        size ++;
    }
    return size;
}

#endif
