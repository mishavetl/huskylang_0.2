#include <stdlib.h>
#include <string.h>
#include "memory.h"
#include "function.h"
#include "dbg.h"
#include "call_tree.h"

fn_t* create_function(
    fn_callback_t fptr, call_tree_t *tree, int argc,
    const unsigned argtypes[], int argtypes_size,
    gc_t *gc
) {
    fn_t *fn = (fn_t *) gc_add(gc, malloc(sizeof(fn_t)));

    fn->argc = argc;

    if (tree) {
        fn->tree = call_tree__duplicate(tree, gc);
        check_mem(fn->tree);
    } else {
        fn->tree = NULL;
    }

    fn->argtypes_size = argtypes_size;
    fn->argtypes = (unsigned *) gc_add(gc, malloc(sizeof(unsigned) * argtypes_size));
    memcpy(fn->argtypes, argtypes, sizeof(unsigned) * argtypes_size);

    fn->callback = fptr;

    return fn;

    error:
    return NULL;
}
