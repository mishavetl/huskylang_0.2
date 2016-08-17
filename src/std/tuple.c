/**
 * Tuples API
 */

int tuple__construct(type_t **args, argc_t argc, type_t *ret, scope_t *scope)
{
    argc_t i;

    ret->type = tid_tuple;
    ret->value.tuple = gc_add(scope->gc, malloc(sizeof(type_t *) * (argc + 1)));
    check_mem(ret->value.tuple);

    for (i = 0; i < argc; i++) {
        ret->value.tuple[i] = gc_add(scope->gc, malloc(sizeof(type_t)));
        check_mem(ret->value.tuple[i]);

        ret->value.tuple[i]->type = args[i]->type;
        ret->value.tuple[i]->value = args[i]->value;
    }

    ret->value.tuple[i] = NULL;

    return 0;

    error:
    return -1;
}

int tuple__get(type_t **args, argc_t argc, type_t *ret, scope_t *scope)
{
    auto i = args[1]->value.num;

    ret->type = args[0]->value.tuple[i]->type;
    ret->value = args[0]->value.tuple[i]->value;

    return 0;
}
