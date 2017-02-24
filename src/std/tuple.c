/**
 * Tuples API
 */

int tuple__construct(data_t **args, argc_t argc, data_t *ret, scope_t *scope)
{
    argc_t i;

    struct type **multiple = gc_add(scope->gc, malloc(sizeof(struct type *) * (argc + 1)));
    check_mem(multiple);
    ret->value.tuple = gc_add(scope->gc, malloc(sizeof(data_t *) * (argc + 1)));
    check_mem(ret->value.tuple);

    for (i = 0; i < argc; i++) {
        data_t *data = copy_data(args[i], scope);
        ret->value.tuple[i] = gc_add(scope->gc, malloc(sizeof(data_t)));
        check_mem(ret->value.tuple[i]);

        ret->value.tuple[i] = data;
        multiple[i] = data->type;
    }

    ret->value.tuple[i] = NULL;
    multiple[i] = NULL;
    ret->type = construct_type(tid_tuple, multiple, scope->gc);

    return 0;

    error:
    return -1;
}

int tuple__get(data_t **args, argc_t argc, data_t *ret, scope_t *scope)
{
    (void) argc;
    int i = args[1]->value.integral;

    data_t *data = copy_data(args[0]->value.tuple[i], scope);
    ret->type = data->type;
    ret->value = data->value;

    return 0;
}