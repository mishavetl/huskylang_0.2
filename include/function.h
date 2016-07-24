#ifndef __function_h__
#define __function_h__

#define INFINITY_ARGS -1

#include "memory.h"

typedef struct scope scope_t;
typedef struct type type_t;

typedef int argc_t;

typedef void (*fn_callback_t)(
    type_t **args, argc_t argc, type_t *ret, scope_t *scope);

typedef struct function {
    int argc;
    int argtypes_size;
    int *argtypes;
    fn_callback_t callback;
} fn_t;

inline fn_t *
create_function(
    fn_callback_t fptr, int argc,
    const int argtypes[], int argtypes_size,
    gc_t *gc);

#endif
