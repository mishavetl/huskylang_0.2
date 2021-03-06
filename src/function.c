#include <stdlib.h>
#include <string.h>
#include "memory.h"
#include "function.h"
#include "dbg.h"
#include "call_tree.h"

fn_t * create_function(
    fn_callback_t fptr, call_tree_t *tree, argc_t argc,
    char *argnames[], int argnames_size,
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

    fn->argnames_size = argnames_size;
    fn->argnames = str_array_dup(argnames, argnames_size, gc);

    fn->callback = fptr;

    return fn;

    error:
    return NULL;
}
