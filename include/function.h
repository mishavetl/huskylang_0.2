/**
 * @file
 * @brief Function structure and functions to handle it
 */
#ifndef __function_h__
#define __function_h__

#define INFINITY_ARGS -1

#include "memory.h"
#include "call_tree.h"

typedef struct scope scope_t;
typedef struct data data_t;

typedef int argc_t;

typedef int (*fn_callback_t)(
    data_t **args, argc_t argc, data_t *ret, scope_t *scope);

/**
 * @brief Function structure
 */
typedef struct function {
    /** Number of arguments */
    int argc;
    /** Size of `argnames` */
    int argnames_size;
    /** Names of arguments */
    char **argnames;
    /** Function callback */
    fn_callback_t callback;
    /** Function call tree */
    call_tree_t *tree;
} fn_t;

/**
 * @brief Creates a function
 *
 * @param[in] fptr Pointer to a function
 * @param[in] tree Function call tree
 * @param[in] argc Number of arguments
 * @param[in] argnames Names of arguments
 * @param[in] argnames_size Size of `argnames`
 * @param[in] gc Garbage collector
 *
 * @return Function
 */
fn_t * create_function(
    fn_callback_t fptr, call_tree_t *tree, int argc,
    char *argnames[], int argnames_size,
    gc_t *gc
);

#endif
