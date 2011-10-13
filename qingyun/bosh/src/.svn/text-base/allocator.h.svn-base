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

#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stdlib.h>

/* This is an implementation of a specialized allocator,
 * this is more efficient for frequently allocated objects.
 * Unused objects are stored on a buffer, and when an alloc
 * is requestes, it returns an object from the buffer. If
 * the buffer is empty, it allocates a bunch of objects at
 * once, reducing memory and time overhead. When an object
 * is free'd it is put back into the buffer instead of really
 * free'ing it.
 * */

#ifndef DONT_USE_ALLOCATOR

#define ALLOCATOR_BLOCK_SIZE 4096

#define IMPLEMENT_ALLOCATOR(type)                                              \
    _##type##_allocator_node* _##type##_allocator_buffer = NULL;

#define DECLARE_ALLOCATOR(type)                                                \
                                                                               \
static const size_t _##type##alloc_step = (((sizeof(type) + (128*1024) - 1)    \
            / sizeof(type)) > ALLOCATOR_BLOCK_SIZE) ? ALLOCATOR_BLOCK_SIZE:    \
            ((sizeof(type) + (128*1024) - 1) / sizeof(type));                  \
typedef struct _##type##_allocator_node {                                      \
    struct _##type##_allocator_node *next, *prev;                              \
    size_t size;                                                               \
    type* objs[ALLOCATOR_BLOCK_SIZE];                                          \
} _##type##_allocator_node;                                                    \
extern _##type##_allocator_node* _##type##_allocator_buffer;                   \
static inline type* type##_alloc() {                                           \
    int i;                                                                     \
                                                                               \
    /* check if the buffer has not been initialized yet */                     \
    if(_##type##_allocator_buffer == NULL) {                                   \
        _##type##_allocator_buffer =                                           \
            malloc(sizeof(_##type##_allocator_node));                          \
        _##type##_allocator_buffer->next = _##type##_allocator_buffer;         \
        _##type##_allocator_buffer->prev = _##type##_allocator_buffer;         \
        _##type##_allocator_buffer->size = 0;                                  \
    }                                                                          \
                                                                               \
    /* if there is no unused obj, alloc a bunch at once */                     \
    if(_##type##_allocator_buffer->size == 0) {                                \
        /* go back on the stack to see if there is any unused obj */           \
        _##type##_allocator_buffer = _##type##_allocator_buffer->prev;         \
        if(_##type##_allocator_buffer->size == 0) {                            \
            type* objs = malloc(_##type##alloc_step*sizeof(type));             \
            for(i = 0; i < _##type##alloc_step; ++i) {                         \
                _##type##_allocator_buffer->objs[i] = objs + i;                \
            }                                                                  \
            _##type##_allocator_buffer->size = _##type##alloc_step;            \
        }                                                                      \
    }                                                                          \
                                                                               \
    return _##type##_allocator_buffer->objs                                    \
        [--_##type##_allocator_buffer->size];                                  \
}                                                                              \
                                                                               \
static inline void type##_free(type* obj) {                                    \
    /* check if the buffer is full */                                          \
    if(_##type##_allocator_buffer->size == (ALLOCATOR_BLOCK_SIZE)) {           \
        /* go foward on the stack to see if the is any empty space */          \
        _##type##_allocator_buffer = _##type##_allocator_buffer->next;         \
        if(_##type##_allocator_buffer->size == (ALLOCATOR_BLOCK_SIZE)) {       \
            _##type##_allocator_node* node =                                   \
                malloc(sizeof(_##type##_allocator_node));                      \
            node->next = _##type##_allocator_buffer;                           \
            node->prev = _##type##_allocator_buffer->prev;                     \
            node->size = 0;                                                    \
            node->next->prev = node;                                           \
            node->prev->next = node;                                           \
            _##type##_allocator_buffer = node;                                 \
        }                                                                      \
    }                                                                          \
    _##type##_allocator_buffer->objs                                           \
    [_##type##_allocator_buffer->size++] = obj;                                \
    }



#else

#define IMPLEMENT_ALLOCATOR(type)

#define DECLARE_ALLOCATOR(type)                                                \
static inline type* type##_alloc() {                                           \
    return malloc(sizeof(type));                                               \
}                                                                              \
                                                                               \
static inline void type##_free(type* obj) {                                    \
    free(obj);                                                                 \
}
#endif

#endif
