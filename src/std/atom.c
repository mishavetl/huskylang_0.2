/**
 * Atom API
 */

int atom__to_string(type_t **args, argc_t argc, type_t *ret, scope_t *scope)
{
    ret->type = tid_string;
    ret->value.string = gc_add(scope->gc, strdup(args[0]->value.atom));
    check_mem(ret->value.string);

    return 0;

    error:

    return -1;
}
