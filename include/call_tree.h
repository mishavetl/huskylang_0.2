#ifndef __call_tree_h__
#define __call_tree_h__

#define EMPTY_MAPV -1
#define TERMINATE_MAPV -2

#include "tokenizer.h"

/**
 * Type of map values
 *
 */
typedef int mapv_t;

/**
 * STRUCT Function Call Tree
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
    mapv_t start;
    size_t size;
} call_tree_t;

#endif
