/**
 * @file
 * @brief Function call tree structure and functions to handle it
 */
#ifndef __call_tree_h__
#define __call_tree_h__

#define EMPTY_MAPV -1
#define TERMINATE_MAPV -2

#include "tokenizer.h"
#include "memory.h"

/**
 * @brief Map value
 */
typedef int mapv_t;

/**
 * @brief Function call tree
 */
typedef struct call_tree {
    /** Start index of token of function in map */
    mapv_t start;
    /** Size of the map */
    int size;
    /** Array of tokens */
    token_t **tokens;
    /** Tree map {{1, 2}, NULL, {3}, NULL} */
    mapv_t **map;
    /** Array of saved function calls */
    int *is_saved;
} call_tree_t;

/**
 * @brief Duplicates call tree
 *
 * @param call_tree Call tree
 * @param gc Garbage collector
 *
 * @return Call tree
 */
call_tree_t *call_tree__duplicate(call_tree_t *call_tree, gc_t *gc);

/**
 * @brief Initiates a call tree
 * @return Call tree
 */
call_tree_t call_tree__init();

/**
 * @brief Counts map values in an array
 *
 * @param[in] ar Array of map values
 *
 * @return Size of an array
 */
size_t count_mapv(mapv_t *ar);


#endif
