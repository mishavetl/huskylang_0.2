/**
 * Saved Function Calls API
 */

int saved__call(type_t **args, argc_t argc, type_t *ret, scope_t *scope)
{
    (void) argc;

    call_tree_t *tree = args[0]->value.tree;

    performer__funcall(tree, scope, ret, tree->start);

    return 0;
}
