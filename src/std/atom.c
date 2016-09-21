/**
 * Atom API
 */

STDFUNCTION(atom__to_string,
    ret->type = tid_string;
    ret->value.string = gc_add(scope->gc, strdup(args[0]->value.atom));
    check_mem(ret->value.string);

    return 0;

    error:

    return -1;
)
