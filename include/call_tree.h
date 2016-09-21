#ifndef __call_tree_h__
#define __call_tree_h__

#define EMPTY_MAPV -1
#define TERMINATE_MAPV -2

#include "tokenizer.h"
#include "memory.h"

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
 * @member meta (array for meta programming features)
 * @member map ({{1, 2}, NULL, {3}, NULL})
 *
 */
typedef struct call_tree {
    mapv_t start;
    int size;
    token_t **tokens;
    mapv_t **map;
    int *is_saved;
} call_tree_t;

/**
 * FUNCTION Duplicates call tree
 *
 * @author mishavetl
 *
 * @param call_tree (call tree)
 * @param gc (garbage collector)
 *
 * @return call tree
 */
call_tree_t *call_tree_duplicate(call_tree_t *call_tree, gc_t *gc);

/**
 * FUNCTION Counts map values in an array
 *
 * @author mishavetl
 *
 * @param ar (array of map values)
 *
 * @return size of an array
 */
size_t count_mapv(mapv_t *ar);


#endif
