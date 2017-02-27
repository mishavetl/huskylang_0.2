/**
 * Functions API
 */

int function__return(data_t **args, argc_t argc, data_t *ret, scope_t *scope)
{
    (void) argc;
    (void) scope;

    data_t *data = copy_data(args[argc - 1], scope);

    ret->type = data->type;
    ret->value = data->value;
    
    return 0;
}

int function__create(data_t **args, argc_t argc, data_t *ret, scope_t *scope)
{
    (void) argc;

    struct type **multiple = NULL;
    char **argnames = NULL;
    int args_size = 0;

    data_t length;

    list__length(args, 1, &length, scope);
    int len = length.value.integral;

    multiple = gc_add(scope->gc, malloc(sizeof(struct type *) * (len + 2)));
    multiple[0] = construct_type(tid_alpha, NULL, scope->gc);

    if (scope->error) {
        goto error;
    }

    if (len > 0) {
        data_t unzipped, argnames1, argtypes1;
        
        list__unzip(args, 1, &unzipped, scope); 

        data_t *args1[] = {unzipped.value.tuple[0]};
        list__to_tuple(args1, 1, &argnames1, scope);
        data_t **argnames_in_lang = argnames1.value.tuple;

        data_t *args2[] = {unzipped.value.tuple[1]};
        list__to_tuple(args2, 1, &argtypes1, scope);        
        data_t **argtypes_in_lang = argtypes1.value.tuple;
        
        for (; argnames_in_lang[args_size]; ++args_size);

        argnames = gc_add(scope->gc, malloc(sizeof(char *) * args_size));

        for (int i = 0; i < args_size; ++i) {
            data_t *argname_in_lang = argnames_in_lang[i];
            data_t *argtype_in_lang = argtypes_in_lang[i];

            argnames[i] = gc_add(scope->gc, strdup(argname_in_lang->value.atom));
            multiple[i + 1] = typedup(argtype_in_lang->type, scope->gc);
        }

        ret->value.fn = create_function(
            NULL, args[1]->value.tree, args_size,
            argnames, args_size,
            scope->gc
        );
    } else {
        ret->value.fn = create_function(
            NULL, args[1]->value.tree, 0, argnames, args_size, scope->gc
        );
        /* argtypes = gc_add(scope->gc, malloc(sizeof(unsigned))); */
    }

    /* ret->value.fn = create_function( */
    /*     NULL, args[1]->value.tree, 0, */
    /*     (const unsigned []) argtypes, argtypes_size, */
    /*     argnames, argnames_size, */
    /*     scope->gc */
    /* ); */

    multiple[len + 1] = NULL;
    ret->type = construct_type(tid_fn, multiple, scope->gc);

    check_mem(ret->type);
    check_mem(ret->value.fn);

    return 0;

error:
    return -1;
}

int function__define(data_t **args, argc_t argc, data_t *ret, scope_t *scope)
{
    data_t *args1[] = {args[0], ret};
    data_t ret1;

    checkf(function__create(args + 1, argc - 1, ret, scope) == 0, "Function creation failed.");
    checkf(var__set(args1, 2, &ret1, scope) == 0, "Variable setting failed.");

    return 0;

error:
    return -1;
}

int function__define_without_args(data_t **args, argc_t argc, data_t *ret, scope_t *scope)
{
    data_t data, list;
    data.type = construct_type(tid_atom, NULL, scope->gc);
    data.value.atom = "atom";
    data_t *args1[] = {&data}, *args2[] = {&list};
    checkf(list__construct(args1, 1, &list, scope) == 0, "List construction failed.");
    checkf(list__tail(args2, 1, &list, scope) == 0, "List tail failed.");

    data_t *args3[] = {args[0], &list, args[1]};

    checkf(function__define(args3, argc, ret, scope) == 0, "Function definition failed.");

    return 0;

error:
    return -1;
}