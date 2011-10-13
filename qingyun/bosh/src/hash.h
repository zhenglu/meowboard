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


#ifndef HASH_H
#define HASH_H

#include "allocator.h"

/* borrowed from g++ key_type##_hash implementation */
static const size_t prime_numbers[] = {
    53ul,         97ul,         193ul,        389ul,       769ul,
    1543ul,       3079ul,       6151ul,       12289ul,     24593ul,
    49157ul,      98317ul,      196613ul,     393241ul,    786433ul,
    1572869ul,    3145739ul,    6291469ul,    12582917ul,  25165843ul,
    50331653ul,   100663319ul,  201326611ul,  402653189ul, 805306457ul,
    1610612741ul, 3221225473ul, 4294967291ul
};

#define IMPLEMENT_HASH(key_type)                                            \
                                                                            \
IMPLEMENT_ALLOCATOR(_##key_type##_hash_node);                               \
IMPLEMENT_ALLOCATOR(key_type##_hash);



#define DECLARE_HASH(key_type, hash_function, compare_function)             \
                                                                            \
typedef void (*key_type##_hash_callback)(const key_type, void*);            \
                                                                            \
/*! \brief A node of the key_type##_hash table */                           \
typedef struct _##key_type##_hash_node {                                    \
    key_type key;                                                           \
    void* value;                                                            \
    struct _##key_type##_hash_node* next;                                   \
} _##key_type##_hash_node;                                                  \
                                                                            \
/*! \brief The hash table */                                                \
typedef struct key_type##_hash {                                            \
    _##key_type##_hash_node* table;                                         \
    size_t table_size;                                                      \
    size_t element_count;                                                   \
} key_type##_hash;                                                          \
                                                                            \
DECLARE_ALLOCATOR(_##key_type##_hash_node);                                 \
DECLARE_ALLOCATOR(key_type##_hash);                                         \
                                                                            \
/*! \brief Creates a new hash table. */                                     \
static inline key_type##_hash* key_type##_hash_new() {                      \
                                                                            \
    key_type##_hash* h;                                                     \
                                                                            \
    h = key_type##_hash_alloc();                                            \
                                                                            \
    h->element_count = 0;                                                   \
    h->table_size = prime_numbers[0];                                       \
                                                                            \
    h->table = calloc(h->table_size, sizeof(_##key_type##_hash_node));      \
                                                                            \
    return h;                                                               \
}                                                                           \
                                                                            \
/*! \brief Insert a node to the _hash and don't resize the table. */        \
static inline void _##key_type##_hash_insert_noresize                       \
        (key_type##_hash* h,                                                \
         _##key_type##_hash_node* node) {                                   \
    size_t hash_pos;                                                        \
                                                                            \
    hash_pos = hash_function(node->key) % h->table_size;                    \
                                                                            \
    node->next = h->table[hash_pos].next;                                   \
    h->table[hash_pos].next = node;                                         \
}                                                                           \
                                                                            \
/*! \brief Resize the table if necessary. */                                \
static inline void _##key_type##_hash_resize(key_type##_hash* h) {          \
    _##key_type##_hash_node* old_table;                                     \
    size_t old_size;                                                        \
    _##key_type##_hash_node* node;                                          \
    int i;                                                                  \
                                                                            \
    if(h->table_size < h->element_count) {                                  \
        old_table = h->table;                                               \
        old_size = h->table_size;                                           \
                                                                            \
        for(i = 0; prime_numbers[i] < h->element_count; ++i) ;              \
                                                                            \
        h->table_size = prime_numbers[i];                                   \
        h->table = calloc(h->table_size, sizeof(_##key_type##_hash_node));  \
                                                                            \
        for(i = 0; i < old_size; ++i) {                                     \
            while(old_table[i].next != NULL) {                              \
                node = old_table[i].next;                                   \
                old_table[i].next = node->next;                             \
                _##key_type##_hash_insert_noresize(h, node);                \
            }                                                               \
        }                                                                   \
        free(old_table);                                                    \
    }                                                                       \
}                                                                           \
                                                                            \
/*! \brief Insert a new item to the key_type##_hash.                        \
 *                                                                          \
 * \note This function does not check for repeated keys,                    \
 *      so if the already exists it will keep both elements.                \
 */                                                                         \
static inline void key_type##_hash_insert                                   \
        (key_type##_hash* h, key_type key, void* value) {                   \
    _##key_type##_hash_node* node;                                          \
                                                                            \
    node = _##key_type##_hash_node_alloc();                                 \
    node->key = key;                                                        \
    node->value = value;                                                    \
    node->next = NULL;                                                      \
                                                                            \
                                                                            \
    _##key_type##_hash_insert_noresize(h, node);                            \
    h->element_count ++;                                                    \
                                                                            \
    _##key_type##_hash_resize(h);                                           \
}                                                                           \
                                                                            \
/*! \brief Find a node in the table by its key                              \
 *                                                                          \
 * \return Returns the node whose next node has the                         \
 *          requested key or NULL if the key is not found.                  \
 * */                                                                       \
static inline _##key_type##_hash_node*                                      \
_##key_type##_hash_find_prev_node(key_type##_hash* h, key_type key) {       \
    _##key_type##_hash_node* node;                                          \
    size_t hash_pos;                                                        \
                                                                            \
    hash_pos = hash_function(key) % h->table_size;                          \
                                                                            \
    for(node = &h->table[hash_pos]; node->next != NULL; node = node->next) {\
        if(compare_function(node->next->key, key)) {                        \
            return node;                                                    \
        }                                                                   \
    }                                                                       \
                                                                            \
    return NULL;                                                            \
}                                                                           \
                                                                            \
/*! \brief Erase the next node of the given node.                           \
 *                                                                          \
 * \return Returns the node's value.                                        \
 * */                                                                       \
static inline void*                                                         \
_##key_type##_hash_erase_next_node(key_type##_hash* h,                      \
                                   _##key_type##_hash_node* node) {         \
    _##key_type##_hash_node* erased_node;                                   \
    void* value;                                                            \
                                                                            \
    erased_node = node->next;                                               \
    node->next = erased_node->next;                                         \
    value = erased_node->value;                                             \
                                                                            \
    _##key_type##_hash_node_free(erased_node);                              \
                                                                            \
    h->element_count--;                                                     \
                                                                            \
    return value;                                                           \
}                                                                           \
                                                                            \
/*! \brief Find a value in the table by its key                             \
 *                                                                          \
 * \return Returns the value or NULL if the key was not found               \
 */                                                                         \
static inline void* key_type##_hash_find(key_type##_hash* h, key_type key) {\
    _##key_type##_hash_node* node;                                          \
                                                                            \
    node = _##key_type##_hash_find_prev_node(h, key);                       \
                                                                            \
    if(node == NULL) {                                                      \
        return NULL;                                                        \
    } else {                                                                \
        return node->next->value;                                           \
    }                                                                       \
}                                                                           \
                                                                            \
/*! \brief Erase an element of the table by its key                         \
 *                                                                          \
 * \returns Returns the value of the erased element                         \
 *      or NULL if the key was not found                                    \
 */                                                                         \
static inline void* key_type##_hash_erase(key_type##_hash* h,               \
                                          key_type key) {                   \
    _##key_type##_hash_node* node;                                          \
                                                                            \
    node = _##key_type##_hash_find_prev_node(h, key);                       \
                                                                            \
    if(node == NULL) {                                                      \
        return NULL;                                                        \
    } else  {                                                               \
        return _##key_type##_hash_erase_next_node(h, node);                 \
    }                                                                       \
}                                                                           \
                                                                            \
/*! \brief Erase all elements of table */                                   \
static inline void key_type##_hash_clear(key_type##_hash* h) {              \
    int i;                                                                  \
                                                                            \
    for(i = 0; i < h->table_size; ++i) {                                    \
        while(h->table[i].next != NULL) {                                   \
            _##key_type##_hash_erase_next_node(h, &h->table[i]);            \
        }                                                                   \
    }                                                                       \
}                                                                           \
                                                                            \
/*! \bief Delete a hash table */                                            \
static inline void key_type##_hash_delete(key_type##_hash* h) {             \
    key_type##_hash_clear(h);                                               \
    free(h->table);                                                         \
    key_type##_hash_free(h);                                                \
}                                                                           \
                                                                            \
/*! \brief Ask if the table contain a given key. */                         \
static inline int key_type##_hash_has_key(key_type##_hash* h,               \
                                          key_type key) {                   \
    return key_type##_hash_find(h, key) != NULL;                            \
}                                                                           \
                                                                            \
/*! \brief The size of the table */                                         \
static inline size_t key_type##_hash_size(key_type##_hash* h) {             \
    return h->element_count;                                                \
}                                                                           \
                                                                            \
                                                                            \
/*! \brief Insert a element or replace an elemente if it already exists    \
 *                                                                          \
 * \return Returns the value of the                                         \
 *      replaced element or NULL if the key didn't exist.                   \
 */                                                                         \
static inline void*                                                         \
key_type##_hash_insert_replace(key_type##_hash* h, key_type key,            \
                               void* value) {                               \
    _##key_type##_hash_node* node;                                          \
    void* v;                                                                \
                                                                            \
    node = _##key_type##_hash_find_prev_node(h, key);                       \
    if(node == NULL) {                                                      \
        v = NULL;                                                           \
        key_type##_hash_insert(h, key, value);                              \
    } else {                                                                \
        node = node->next;                                                  \
        v = node->value;                                                    \
        node->value = value;                                                \
    }                                                                       \
                                                                            \
    return v;                                                               \
}                                                                           \
                                                                            \
/*! \brief Iterate through all elements in the hash table                   \
 *                                                                          \
 * The function callback is called for every element in the table           \
 */                                                                         \
static inline void                                                          \
key_type##_hash_iterate(key_type##_hash* h,                                 \
                        key_type##_hash_callback callback) {                \
    int i;                                                                  \
    _##key_type##_hash_node* node;                                          \
                                                                            \
    for(i = 0; i < h->table_size; ++i) {                                    \
        for(node = h->table[i].next; node != NULL; node = node->next) {     \
            callback(node->key, node->value);                               \
        }                                                                   \
    }                                                                       \
}

#endif
