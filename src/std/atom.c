/**
 * Atom API
 */

STDFUNCTION(atom__to_string,
    ret->type = construct_type(tid_string, NULL, scope->gc);
    ret->value.string = gc_add(scope->gc, strdup(args[0]->value.atom));
    check_mem(ret->value.string);

    return 0;

    error:

    return -1;
)
