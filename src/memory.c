#include <string.h>
#include <stdlib.h>
#include "memory.h"
#include "data.h"
#include "variable.h"
#include "dbg.h"
#include "list/src/list.h"
#include "call_tree.h"

size_t count_2d(void **ar)
{
    if (!ar) return 0;
    size_t len;
    for (len = 0; ar[len]; len++);
    return len;
}

gc_t gc_init()
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

data_t *copy_data(data_t *src, scope_t *scope)
{
    int i;
    data_t *data = gc_add(scope->gc, malloc(sizeof(data_t)));

    check_mem(data);

    data->type = typedup(src->type, scope->gc);
    data->value = src->value;

    if (src->type->single == tid_string) {
        data->value.string = gc_add(scope->gc, strdup(src->value.string));
    } else if (src->type->single == tid_atom) {
        data->value.atom = gc_add(scope->gc, strdup(src->value.atom));
    } else if (src->type->single == tid_tuple) {
        for (i = 0; src->value.tuple[i]; i++);
        data->value.tuple = gc_add(scope->gc, malloc(sizeof(data_t *) * (i + 1)));
        
        for (i = 0; src->value.tuple[i]; i++) {
            data->value.tuple[i] = copy_data(src->value.tuple[i], scope);
        }
        data->value.tuple[i] = NULL;
    } else if (src->type->single == tid_list) {
        list_t *copy = gc_add_list(scope->gc, list_new());
        list_node_t *node;
        list_iterator_t *it = list_iterator_new(src->value.list, LIST_HEAD);

        while ((node = list_iterator_next(it))) {
            list_rpush(copy, list_node_new(copy_data(node->val, scope)));
        }

        data->value.list = copy;
        list_iterator_destroy(it);
    } else if (src->type->single == tid_saved) {
        data->value.tree = call_tree__duplicate(src->value.tree, scope->gc);
        check_mem(data->value.tree);
    }

    return data;

    error:
    return NULL;
}

char **str_array_dup(char **src, int n, gc_t *gc)
{
    if (!src) {
        return NULL;
    }

    char **dest = gc_add(gc, malloc(sizeof(char *) * n));

    check_mem(dest);

    for (int i = 0; i < n; ++i) {
        dest[i] = gc_add(gc, strdup(src[i]));
    }

    return dest;

error:
    return NULL;
}
