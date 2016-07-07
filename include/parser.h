#ifndef _PARSER_H
#define _PARSER_H

#include "tokenizer.h"

#define EMPTY_MAPV -1
#define TERMINATE_MAPV -2

/**
 * Type of map values
 *
 */
typedef int mapv_t;

/**
 * Function Call Tree structure
 *
 * @author mishavetl
 *
 * @member tokens (an array of tokens)
 * @member map ({{1, 2}, NULL, {3}, NULL})
 *
 */
typedef struct call_tree {
    token_t **tokens;
    mapv_t **map;
    size_t size;
} call_tree_t;

/**
 * Constructs a call tree
 *
 * @author mishavetl
 *
 * @param call_tree (pointer to an instance of the call_tree)
 * @param tokens (an array of tokens)
 *
 * @ret (status of the parse -1 for error, >= 0 for success)
 *
 */
mapv_t parser__funcall(call_tree_t *call_tree, token_t **tokens);
// call_tree_t *parser__parse(token_t **tokens);

#endif
