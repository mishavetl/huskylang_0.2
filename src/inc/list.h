#ifndef _LIST_H
#define _LIST_H

#include <stdlib.h>
#include "dbg.h"

#define REG_LIST(typename, type) \
typedef struct typename##_node { \
    type val; \
    struct typename##_node *next, *prev; \
} typename##_node_t; \
\
typename##_node_t *typename##_node__new(type val) { \
    typename##_node_t* node = malloc(sizeof(typename##_node_t)); \
    check_mem(node); \
    node->val = val; \
    node->prev = NULL; \
    node->next = NULL; \
    return node; \
    \
    error: \
    return NULL;\
} \
\
void typename##_node__append_after(typename##_node_t *node_prev, typename##_node_t *node) { \
    node_prev->next = node; \
    node->prev = node_prev; \
} \
\
void typename##_node__append_before(typename##_node_t *node_next, typename##_node_t *node) { \
    node_next->prev = node; \
    node->next = node_next; \
} \
\
typename##_node_t *typename##_node__remove(typename##_node_t *node) { \
    if (node->prev) node->prev->next = node->next; \
    if (node->next) node->next->prev = node->prev; \
    return node; \
}

#endif
