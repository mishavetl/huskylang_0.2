/**
 * Number API
 */

int number__to_string(data_t **args, argc_t argc, data_t *ret, scope_t *scope)
{
    (void) argc;

    TYPE_NUMBER n = args[0]->value.num;
    if (n <= 0) {
        n = -n + 1;
    }
    
    char *string = gc_add(scope->gc, malloc(sizeof(char) * (log10(n) + 2)));

    check_mem(string);

    sprintf(string, "%d", args[0]->value.num);

    ret->type = tid_string;
    ret->value.string = string;

    return 0;

error:
    return -1;
}

int number__equal(data_t **args, argc_t argc, data_t *ret, scope_t *scope)
{
    (void) argc;

    ret->type = tid_atom;
    ret->value.atom = gc_add(scope->gc, strdup(
            (args[0]->value.num == args[1]->value.num) ? BOOLEAN_TRUE : BOOLEAN_FALSE
        )
    );

    check_mem(ret->value.atom);

    return 0;

error:
    return -1;
}
