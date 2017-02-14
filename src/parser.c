#include <stdlib.h>
#include <string.h>
#include "memory.h"
#include "parser.h"
#include "dbg.h"

mapv_t parser__funcall_(call_tree_t *call_tree, token_t **tokens, mapv_t *i_);

mapv_t parser__funcall(call_tree_t *call_tree, token_t **tokens)
{
    mapv_t i = 0;
    size_t size = count_2d((void **) tokens);
    size_t tree_size = sizeof(mapv_t *) * size;

    call_tree->map = (mapv_t **) malloc(tree_size);
    check_mem(call_tree->map);
    memset(call_tree->map, 0, tree_size);

    call_tree->is_saved = (mapv_t *) malloc(sizeof(int) * size);
    check_mem(call_tree->is_saved);
    memset(call_tree->is_saved, 0, sizeof(int) * size);

    call_tree->tokens = tokens;

    return parser__funcall_(call_tree, tokens, &i);

    error:

    FREE(call_tree->map);

    return -1;
}

mapv_t parser__funcall_(call_tree_t *call_tree, token_t **tokens, mapv_t *i_)
{
    mapv_t size = 1;
    mapv_t *args;
    mapv_t i = *i_;
    mapv_t saved, fname_pos;
    mapv_t i_start = i, fname_i = EMPTY_MAPV, fname_arg_i;
    int possible_fname = 1;
    int is_saved = 0;

    check_mem(args = (mapv_t *) malloc(sizeof(mapv_t)));

    for (; tokens[i]; i++) {
        if (tokens[i]->type == tok_prior_end) {
            break;
        }

        if (tokens[i]->type != tok_del) {
            saved = i;
            fname_pos = i;

            if (false
                || tokens[i]->type == tok_prior_start
                || tokens[i]->type == tok_prior_start_saved
            ) {
                if (tokens[i]->type == tok_prior_start_saved) {
                    is_saved = 1;
                } else {
                    is_saved = 0;
                }

                i++;
                fname_pos = parser__funcall_(call_tree, tokens, &i);
                checkf(fname_pos >= 0, "Nested function call parsing failed.");
                call_tree->is_saved[fname_pos] = is_saved;
            }

            if (tokens[i + 1]) {
                if (tokens[i + 1]->type != tok_del && tokens[i + 1]->type != tok_prior_end) {
                    if (possible_fname) {
                        if (fname_i != EMPTY_MAPV) {
                            args[fname_arg_i] = args[0];
                            possible_fname = 0;
                        } else {
                            args = (mapv_t *) realloc(args, sizeof(mapv_t) * ++size);
                            check_mem(args);
                            args[size - 1] = EMPTY_MAPV;
                            fname_arg_i = size - 1;
                        }

                        args[0] = fname_pos;
                        fname_i = saved;

                        continue;
                    } else {
                        sentinel("syntax error: 'multiple function names in a function call' at column %ld, line %ld-%ld, token: '%s'", tokens[i]->col, tokens[i_start]->linefrom, tokens[i_start]->lineto, tokens[i]->value);
                    }
                }
            }

            if ((!tokens[i + 1] || tokens[i + 1]->type == tok_prior_end)
                && fname_i == EMPTY_MAPV
            ) {
                args = (mapv_t *) realloc(args, sizeof(mapv_t) * ++size);
                check_mem(args);
                args[size - 1] = EMPTY_MAPV;
                fname_arg_i = size - 1;

                args[0] = fname_pos;
                fname_i = saved;

                continue;
            }

            args = (mapv_t *) realloc(args, sizeof(mapv_t) * ++size);
            check_mem(args);

            args[size - 1] = fname_pos;
        }
    }

    args = (mapv_t *) realloc(args, sizeof(mapv_t) * ++size);
    args[size - 1] = TERMINATE_MAPV;

    if (i - i_start == 1) {
        fname_i = i_start;
        args[size - 2] = EMPTY_MAPV;
    }

    if (fname_i == EMPTY_MAPV) {
        sentinel("syntax error: 'no function name candidates' at column %ld, line %ld-%ld, token: '%s'", tokens[i_start]->col, tokens[i_start]->linefrom, tokens[i_start]->lineto, tokens[i_start]->value);
    }

    call_tree->map[fname_i] = args;
    call_tree->size = i;
    call_tree->start = fname_i;

    *i_ = i;

    return fname_i;

    error:

    FREE(args);
    return -1;
}
