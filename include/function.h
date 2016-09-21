#ifndef __function_h__
#define __function_h__

#define INFINITY_ARGS -1

#include "memory.h"

typedef struct scope scope_t;
typedef struct type type_t;

typedef int argc_t;

typedef int (*fn_callback_t)(
    type_t **args, argc_t argc, type_t *ret, scope_t *scope);

/**
 * STRUCT function
 *
 * @author mishavetl
 *
 * @member argc (number of arguments)
 * @member argtypes (types of arguments)
 * @member argtypes_size (size of `argtypes`)
 * @member callback (function callback)
 */
typedef struct function {
    int argc;
    int argtypes_size;
    unsigned *argtypes;
    fn_callback_t callback;
} fn_t;

/**
 * FUNCTION Creates a function
 *
 * @author mishavetl
 *
 * @param fptr (pointer to a function)
 * @param argc (number of arguments)
 * @param argtypes (types of arguments)
 * @param argtypes_size (size of `argtypes`)
 * @param gc (garbage collector)
 *
 * @return fn_t (function)
 */
fn_t *
create_function(
    fn_callback_t fptr, int argc,
    const unsigned argtypes[], int argtypes_size,
    gc_t *gc);

#endif
