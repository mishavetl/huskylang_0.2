/**
 * Errors API
 */

int error__except_string(data_t **args, argc_t argc, data_t *ret, scope_t *scope)
{
    (void) argc;

    call_tree_t *tree = args[0]->value.tree;

    performer__funcall(tree, scope, ret, tree->start);

    if (!scope->error) {
        scope->error = gc_add(scope->gc, malloc(sizeof(huserr_t)));
        scope->error->name = "exceptError";
        scope->error->msg = "scope has no error";
        scope->error->token = NULL;
        goto error;
    }

    ret->type = tid_string;
    ret->value.string = huserr__to_string(scope);
    check_mem(ret->value.string);

    scope->error = NULL;

    return 0;

    error:
    return -1;
}
