#include <stdlib.h>
#include <string.h>
#include "call_tree.h"
#include "parser.h"
#include "memory.h"
#include "dbg.h"

call_tree_t *call_tree__duplicate(call_tree_t *call_tree, gc_t *gc)
{
    int i, j;
    size_t size = count_2d(call_tree->tokens);
    
    call_tree_t *copy = (call_tree_t *) gc_add(gc, malloc(sizeof(call_tree_t)));
    check_mem(copy);

    copy->map = (mapv_t **) gc_add(gc, malloc(sizeof(mapv_t *) * size));
    check_mem(copy->map);

    copy->tokens = (token_t **) gc_add(gc, malloc(sizeof(token_t *) * size));
    check_mem(copy->tokens);
    
    for (i = 0; i < size; i++) {
        if (call_tree->map[i]) {
            for (j = 0; call_tree->map[i][j]; j++);

            copy->map[i] = (mapv_t **) gc_add(gc, malloc(sizeof(mapv_t) * j));
            check_mem(copy->map[i]);
            memcpy(copy->map[i], call_tree->map[i], sizeof(mapv_t) * j);
        } else {
            copy->map[i] = NULL;
        }

        copy->tokens[i] = (token_t *) gc_add(gc, malloc(sizeof(token_t)));
        check_mem(copy->tokens[i]);
        memcpy(copy->tokens[i], call_tree->tokens[i], sizeof(token_t));

        copy->tokens[i]->value = (char *) gc_add(gc, strdup(call_tree->tokens[i]->value));
    }

    copy->is_saved = (int *) gc_add(gc, malloc(sizeof(int) * size));
    check_mem(copy->is_saved);
    memcpy(copy->is_saved, call_tree->is_saved, sizeof(int) * size);

    return copy;

    error:

    return NULL;
}
