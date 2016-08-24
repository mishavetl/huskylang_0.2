#include <string.h>
#include <stdlib.h>
#include "memory.h"
#include "type.h"
#include "variable.h"
#include "dbg.h"
#include "list/src/list.h"

size_t count_2d(void **ar)
{
    size_t len;

    for (len = 0; ar[len]; len++);

    return len;
}

inline gc_t gc_init()
{
    return (gc_t) {
        .ptrs = NULL,
        .lists = NULL,
        .size = 0,
        .lists_size = 0
    };
}

void gc_clean(gc_t *gc)
{
    size_t i;

    if (gc->ptrs) {
        for (i = 0; i < gc->size; i++) {
            FREE(gc->ptrs[i]);
        }

        FREE(gc->ptrs);
    } if (gc->lists) {
        for (i = 0; i < gc->lists_size; i++) {
            list_destroy(gc->lists[i]);
        }

        FREE(gc->lists);
    }
}

void *gc_add(gc_t *gc, void *ptr)
{
    gc->ptrs = realloc(gc->ptrs, sizeof(void *) * ++gc->size);

    return gc->ptrs[gc->size - 1] = ptr;
}

list_t *gc_add_list(gc_t *gc, list_t *ptr)
{
    gc->lists = realloc(gc->lists, sizeof(void *) * ++gc->lists_size);

    return gc->lists[gc->lists_size - 1] = ptr;
}

type_t *copy_type(type_t *src, scope_t *scope)
{
    int i;
    type_t *type = gc_add(scope->gc, malloc(sizeof(type_t)));

    check_mem(type);
    
    type->type = src->type;
    type->value = src->value;

    if (src->type == tid_string) {
        type->value.string = gc_add(scope->gc, strdup(src->value.string));
    } else if (src->type == tid_atom) {
        type->value.atom = gc_add(scope->gc, strdup(src->value.atom));
    } else if (src->type == tid_tuple) {
        for (i = 0; type->value.tuple[i]; i++) {
            type->value.tuple[i] = copy_type(src->value.tuple[i], scope);
        }
    } else if (src->type == tid_list) {
        list_t *copy = gc_add_list(scope->gc, list_new());
        list_node_t *node;
        list_iterator_t *it = list_iterator_new(src->value.list, LIST_HEAD);

        while (node = list_iterator_next(it)) {
            list_rpush(copy, list_node_new(copy_type(node->val, scope)));
        }

        type->value.list = copy;
        list_iterator_destroy(it);
    }

    return type;

    error:
    return NULL;
}
