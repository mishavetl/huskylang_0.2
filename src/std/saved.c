/**
 * Saved Function Calls API
 */

STDFUNCTION(saved__call,
    call_tree_t *tree = args[0]->value.tree;

    performer__funcall(tree, scope, ret, tree->start);

    return 0;
)
