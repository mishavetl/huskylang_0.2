#ifndef _LIST_H
#define _LIST_H

#include <stdlib.h>
#include "dbg.h"

#define INIT_LIST_STRUCT(typename, type) \
    typedef struct typename##_node { \
        type val; \
        struct typename##_node *next, *prev; \
    } typename##_node_t;

#define REG_LIST(typename, type) \
    typedef struct typename##_node typename##_node_t; \
    typename##_node_t *typename##_node__new(type val); \
    void typename##_node__append_after(typename##_node_t *node_prev, typename##_node_t *node); \
    void typename##_node__append_before(typename##_node_t *node_next, typename##_node_t *node); \
    typename##_node_t *typename##_node__remove(typename##_node_t *node);

#define INIT_LIST(typename, type) \
typename##_node_t *typename##_node__new(type val) { \
    typename##_node_t *node = (typename##_node_t *) malloc(sizeof(typename##_node_t)); \
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
    node->next = node_prev->next; \
    if (node->next) node->next->prev = node; \
    node_prev->next = node; \
    node->prev = node_prev; \
} \
\
void typename##_node__append_before(typename##_node_t *node_next, typename##_node_t *node) { \
    node->prev = node_next->prev; \
    if (node->prev) node->prev->next = node; \
    node_next->prev = node; \
    node->next = node_next; \
} \
\
typename##_node_t *typename##_node__remove(typename##_node_t *node) { \
    if (node->prev) node->prev->next = node->next; \
    if (node->next) node->next->prev = node->prev; \
    return node; \
} \
\
typename##_node_t *typename##_node__add(typename##_node_t *head1, typename##_node_t *head2) { \
    typename##_node_t *last; \
    typename##_node_t *head3 = head1; \
    while (head1) { \
        last = head1; \
        head1 = head1->next; \
    } \
    typename##_node__append_before(head2, last); \
    \
    return head3; \
} \
\
void typename##_node__clean(typename##_node_t *node) { \
    if (node) { \
        while (node->prev) node = node->prev; /* node is head now */ \
        while (node) { \
            typename##_node_t *next = node->next; \
            free(node); \
            node = next; \
        } \
    } \
}


#endif
