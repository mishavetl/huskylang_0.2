/**
 * Errors API
 */

STDFUNCTION(error__except_string,
    call_tree_t *tree = args[0]->value.tree;

    performer__funcall(tree, scope, ret, tree->start);

    if (!scope->error) {
        scope->error = gc_add(scope->gc, malloc(sizeof(huserr_t)));
        scope->error->name = "exceptError";
        scope->error->msg = "scope has no error";
        goto error;
    }

    ret->type = tid_string;
    ret->value.string = huserr__to_string(scope);
    check_mem(ret->value.string);

    scope->error = NULL;

    return 0;

    error:
    return -1;
)
