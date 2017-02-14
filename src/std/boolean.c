/**
 * Boolean API
 */

int atom_is_true(const char *atom)
{
    return strcmp(atom, BOOLEAN_FALSE) != 0 && strcmp(atom, BOOLEAN_ERR) != 0;
}

int boolean__if(data_t **args, argc_t argc, data_t *ret, scope_t *scope)
{
    (void) argc;

    if (atom_is_true(args[0]->value.atom)) {
        return performer__execute(args[1]->value.tree, scope, ret);
    } else {
        ret->type = tid_atom;
        ret->value.atom = gc_add(scope->gc, strdup("good"));
        check_mem(ret->value.atom);
    }

    return 0;

error:
    return -1;
}

int boolean__continuous_if(data_t **args, argc_t argc, data_t *ret, scope_t *scope)
{
    (void) argc;

    if (atom_is_true(args[0]->value.atom)) {
        return performer__execute(args[1]->value.tree, scope, ret);
    } else {
        return performer__execute(args[2]->value.tree, scope, ret);
    }

    return 0;

/* error: */
/*     return -1; */
}
