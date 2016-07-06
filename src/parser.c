#include <stdlib.h>
#include "memory.h"
#include "parser.h"
#include "dbg.h"

int parser__funcall(call_tree_t *call_tree, token_t **tokens, mapv_t i)
{
    mapv_t size = 0;
    mapv_t *args = NULL;
    mapv_t i_start = i, fname_i = EMPTY_MAPV, fname_arg_i;
    int possible_fname = 1;

    call_tree->tokens = tokens;
    call_tree->map = malloc(sizeof(mapv_t *) * (i + 1));

    for (; tokens[i]; i++) {
        call_tree->map = realloc(call_tree->map, sizeof(mapv_t *) * (i + 1));
        call_tree->map[i] = NULL;
        check_mem(call_tree->map);

        if (tokens[i]->type != tid_del) {
            if (tokens[i + 1]) {
                if (tokens[i + 1]->type != tid_del) {
                    if (possible_fname) {
                        if (fname_i != EMPTY_MAPV) {
                            args[fname_arg_i] = fname_i;
                            possible_fname = 0;
                        } else {
                            args = realloc(args, sizeof(mapv_t) * ++size);
                            check_mem(args);
                            args[size - 1] = EMPTY_MAPV;
                            fname_arg_i = size - 1;
                        }

                        fname_i = i;

                        continue;
                    } else {
                        sentinel("syntax error: 'multiple function names in a function call' at column %ld, token: '%s'", tokens[i]->col, tokens[i]->value);
                    }
                }
            }

            args = realloc(args, sizeof(mapv_t) * ++size);
            check_mem(args);
            args[size - 1] = i;
        }
    }

    if (fname_i == -1) {
        sentinel("syntax error: 'no function name candidates' at column %ld, token: '%s'", tokens[i_start]->col, tokens[i_start]->value);
    }

    args = realloc(args, sizeof(int) * ++size);
    args[size - 1] = TERMINATE_MAPV;

    call_tree->map[fname_i] = args;
    call_tree->size = i;

    return 0;

    error:

    FREE(args);
    FREE(call_tree->map);
    return -1;
}
