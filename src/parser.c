#include <stdlib.h>
#include <string.h>
#include "memory.h"
#include "parser.h"
#include "dbg.h"

mapv_t parser__funcall(call_tree_t *call_tree, token_t **tokens)
{
    mapv_t i = 0;
    size_t tree_size = sizeof(mapv_t *) * count_2d(tokens);

    call_tree->map = malloc(tree_size);
    check_mem(call_tree->map);
    memset(call_tree->map, 0, tree_size);

    call_tree->tokens = tokens;

    return parser__funcall_(call_tree, tokens, &i);

    error:

    FREE(call_tree->map);

    return -1;
}

mapv_t parser__funcall_(call_tree_t *call_tree, token_t **tokens, mapv_t *i_)
{
    mapv_t size = 0;
    mapv_t *args = NULL;
    mapv_t i = *i_;
    mapv_t i_start = i, fname_i = EMPTY_MAPV, fname_arg_i;
    int possible_fname = 1;


    for (; tokens[i]; i++) {
        if (tokens[i]->type == tid_prior_end) {
            break;
        }

        if (tokens[i]->type != tid_del) {
            if (tokens[i + 1] && tokens[i]->type != tid_prior_start) {
                if (tokens[i + 1]->type != tid_del && tokens[i + 1]->type != tid_prior_end) {
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

            if (tokens[i]->type == tid_prior_start) {
                i++;
                args[size - 1] = parser__funcall_(call_tree, tokens, &i);

                check(args[size - 1] >= 0, "Nested function call parsing failed.");
            } else {
                args[size - 1] = i;
            }
        }
    }

    if (fname_i == -1) {
        sentinel("syntax error: 'no function name candidates' at column %ld, token: '%s'", tokens[i_start]->col, tokens[i_start]->value);
    }

    args = realloc(args, sizeof(mapv_t) * ++size);
    args[size - 1] = TERMINATE_MAPV;

    call_tree->map[fname_i] = args;
    call_tree->size = i;

    *i_ = i;

    return fname_i;

    error:

    FREE(args);
    return -1;
}
