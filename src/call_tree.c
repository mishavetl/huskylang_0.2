#include <stdlib.h>
#include <string.h>
#include "call_tree.h"
#include "parser.h"
#include "memory.h"
#include "dbg.h"

size_t count_mapv(mapv_t *ar)
{
    size_t len;

    for (len = 0; ar[len] != TERMINATE_MAPV; len++);

    return len;
}

call_tree_t call_tree__init()
{
    return (call_tree_t) {
        .start = -1,
        .size = 0,
        .tokens = NULL,
        .map = NULL,
        .is_saved = NULL
    };
}

call_tree_t *call_tree__duplicate(call_tree_t *call_tree, gc_t *gc)
{
    unsigned int i, j;
    size_t size = count_2d((void **) call_tree->tokens);

    call_tree_t *copy = (call_tree_t *) gc_add(gc, malloc(sizeof(call_tree_t)));
    check_mem(copy);

    copy->start = call_tree->start;
    copy->size = call_tree->size;

    copy->map = (mapv_t **) gc_add(gc, malloc(sizeof(mapv_t *) * size));
    check_mem(copy->map);

    copy->tokens = (token_t **) gc_add(gc, malloc(sizeof(token_t *) * (size + 1)));
    check_mem(copy->tokens);

    copy->is_saved = (int *) gc_add(gc, malloc(sizeof(int) * size));
    check_mem(copy->is_saved);

    for (i = 0; i < size; i++) {
        if (call_tree->map[i]) {
            j = count_mapv(call_tree->map[i]) + 1;

            copy->map[i] = (mapv_t *) gc_add(gc, malloc(sizeof(mapv_t) * j));
            check_mem(copy->map[i]);
            memcpy(copy->map[i], call_tree->map[i], sizeof(mapv_t) * j);
        } else {
            copy->map[i] = NULL;
        }

        copy->is_saved[i] = call_tree->is_saved[i];

        copy->tokens[i] = (token_t *) gc_add(gc, malloc(sizeof(token_t)));
        check_mem(copy->tokens[i]);
        memcpy(copy->tokens[i], call_tree->tokens[i], sizeof(token_t));

        copy->tokens[i]->value = (char *) gc_add(gc, strdup(call_tree->tokens[i]->value));
    }

    copy->tokens[size] = NULL;

    return copy;

    error:

    return NULL;
}
