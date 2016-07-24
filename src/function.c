#include <stdlib.h>
#include <string.h>
#include "memory.h"
#include "function.h"
#include "dbg.h"

inline fn_t*
create_function(
    fn_callback_t fptr, int argc,
    const int argtypes[], int argtypes_size,
    gc_t *gc)
{
    fn_t *fn = (fn_t *) gc_add(gc, malloc(sizeof(fn_t)));

    fn->argc = argc;

    fn->argtypes_size = argtypes_size;
    fn->argtypes = (int *) gc_add(gc, malloc(sizeof(argtypes)));
    memcpy(fn->argtypes, argtypes, sizeof(argtypes));

    fn->callback = fptr;

    return fn;
}
