/**
 * Tuples API
 */

int list__construct(data_t **args, argc_t argc, data_t *ret, scope_t *scope)
{
    argc_t i;
    data_t *data;

    ret->value.list = gc_add_list(scope->gc, list_new());
    check_mem(ret->value.list);

    for (i = 0; i < argc; i++) {
        if (!types_identical(args[0]->type, args[i]->type)) {
            huserr__raise("typeErr", "list items must have one type", NULL, scope);
            goto error;
        }
        
        data = gc_add(scope->gc, malloc(sizeof(data_t)));
        check_mem(data);

        data->type = typedup(args[i]->type, scope->gc);
        data->value = args[i]->value;

        check_mem(list_rpush(ret->value.list, list_node_new(data)));
    }

    ret->type = construct_type(tid_list, (argc > 0) ? itta(stt{
        typedup(args[0]->type, scope->gc)
    }, 1, scope->gc) : NULL, scope->gc);

    return 0;

error:
    return -1;
}

int list__head(data_t **args, argc_t argc, data_t *ret, scope_t *scope)
{
    (void) argc;
    
    if (args[0]->value.list->len == 0) {
        scope->error = gc_add(scope->gc, malloc(sizeof(huserr_t)));
        scope->error->token = NULL;
        scope->error->name = "listErr";
        scope->error->msg = "empty list has no head";
        goto error;
    }

    ret->type = typedup(((data_t *) args[0]->value.list->head->val)->type, scope->gc);
    ret->value = ((data_t *) args[0]->value.list->head->val)->value;

    return 0;

error:
    return -1;
}

int list__tail(data_t **args, argc_t argc, data_t *ret, scope_t *scope)
{
    (void) argc;

    list_node_t *node;
    list_iterator_t *it = list_iterator_new(args[0]->value.list, LIST_HEAD);
    check_mem(it);

    ret->type = typedup(args[0]->type, scope->gc);
    ret->value.list = gc_add_list(scope->gc, list_new());

    node = list_iterator_next(it);

    while ((node = list_iterator_next(it))) {
        list_rpush(ret->value.list, list_node_new(node->val));
    }

    if (it) list_iterator_destroy(it);

    return 0;

error:
    if (it) list_iterator_destroy(it);
    return -1;
}

int list__length(data_t **args, argc_t argc, data_t *ret, scope_t *scope)
{
    (void) argc;
    (void) scope;

    list_node_t *node;
    list_iterator_t *it = list_iterator_new(args[0]->value.list, LIST_HEAD);

    check_mem(it);

    int length = 0;

    while ((node = list_iterator_next(it))) {
        ++length;
    }
    
    ret->type = construct_type(tid_integral, NULL, scope->gc);
    ret->value.integral = length;

    if (it) list_iterator_destroy(it);

    return 0;

error:
    if (it) list_iterator_destroy(it);
    return -1;
}

int list__unzip(data_t **args, argc_t argc, data_t *ret, scope_t *scope)
{
    (void) argc;

    ret->type = construct_type(tid_tuple, itta(stt{
        construct_type(tid_list, itta(stt{
            typedup(((data_t *) args[0]->value.list->head->val)->value.tuple[0]->type, scope->gc)
        }, 1, scope->gc), scope->gc),
        construct_type(tid_list, itta(stt{
            typedup(((data_t *) args[0]->value.list->head->val)->value.tuple[1]->type, scope->gc)
        }, 1, scope->gc), scope->gc),
    }, 2, scope->gc), scope->gc);
    check_mem(ret->type);

    ret->value.tuple = gc_add(scope->gc, malloc(sizeof(data_t *) * 3));
    check_mem(ret->value.list);

    ret->value.tuple[0] = gc_add(scope->gc, malloc(sizeof(data_t)));
    check_mem(ret->value.tuple[0]);
    ret->value.tuple[1] = gc_add(scope->gc, malloc(sizeof(data_t)));
    check_mem(ret->value.tuple[0]);
    
    ret->value.tuple[0]->type = construct_type(tid_list, itta(stt{
        typedup(((data_t *) args[0]->value.list->head->val)->value.tuple[0]->type, scope->gc)
    }, 1, scope->gc), scope->gc);
    ret->value.tuple[0]->value.list = gc_add(scope->gc, list_new());
    check_mem(ret->value.tuple[0]->value.list);

    ret->value.tuple[1]->type = construct_type(tid_list, itta(stt{
        typedup(((data_t *) args[0]->value.list->head->val)->value.tuple[1]->type, scope->gc)
    }, 1, scope->gc), scope->gc);
    ret->value.tuple[1]->value.list = gc_add(scope->gc, list_new());
    check_mem(ret->value.tuple[1]->value.list);

    ret->value.tuple[2] = NULL;

    list_node_t *node;
    list_iterator_t *it = list_iterator_new(args[0]->value.list, LIST_HEAD);

    while ((node = list_iterator_next(it))) {
        list_rpush(ret->value.tuple[0]->value.list, gc_add(scope->gc, list_node_new(
            copy_data(((data_t *) node->val)->value.tuple[0], scope))));
        list_rpush(ret->value.tuple[1]->value.list, gc_add(scope->gc, list_node_new(
            copy_data(((data_t *) node->val)->value.tuple[1], scope))));
    }

    // ret->value.tuple = gc_add(scope->gc, malloc(sizeof(data_t *) * 3));
    // check_mem(ret->value.tuple);

    // ret->value.tuple[0] = gc_add(scope->gc, malloc(sizeof(data_t)));
    // check_mem(ret->value.tuple[0]);
    
    // ret->value.tuple[0]->type = tid_tuple;
    // ret->value.tuple[0]->value.tuple = gc_add(scope->gc, malloc(sizeof(data_t *) * (length + 1)));
    // check_mem(ret->value.tuple[0]->value.tuple);

    // ret->value.tuple[1] = gc_add(scope->gc, malloc(sizeof(data_t)));
    // check_mem(ret->value.tuple[1]);
    
    // ret->value.tuple[1]->type = tid_tuple;
    // ret->value.tuple[1]->value.tuple = gc_add(scope->gc, malloc(sizeof(data_t *) * (length + 1)));
    // check_mem(ret->value.tuple[1]->value.tuple);
    
    // ret->value.tuple[2] = NULL;

    // list_node_t *node;
    // list_iterator_t *it = list_iterator_new(args[0]->value.list, LIST_HEAD);

    // int i = 0;

    // while ((node = list_iterator_next(it))) {
    //     if (((data_t *) node->val)->type != tid_tuple) {
    //         scope->error = gc_add(scope->gc, malloc(sizeof(huserr_t)));
    //         scope->error->name = "typeErr";
    //         scope->error->msg = "must be a zipped list";
    //         scope->error->token = NULL;
    //         goto error;
    //     }

    //     ret->value.tuple[0]->value.tuple[i] = copy_data(((data_t *) node->val)->value.tuple[0], scope);
    //     check_mem(ret->value.tuple[0]->value.tuple[i]);

    //     ret->value.tuple[1]->value.tuple[i] = copy_data(((data_t *) node->val)->value.tuple[1], scope);
    //     check_mem(ret->value.tuple[1]->value.tuple[i]);

    //     ++i;
    // }

    // ret->value.tuple[0]->value.tuple[i] = NULL;
    // ret->value.tuple[1]->value.tuple[i] = NULL;

    if (it) list_iterator_destroy(it);

    return 0;

error:
    if (it) list_iterator_destroy(it);
    return -1;
}

int list__to_tuple(data_t **args, argc_t argc, data_t *ret, scope_t *scope)
{
    (void) argc;
    data_t ret1;
    checkf(list__length(args, 1, &ret1, scope) == 0, "Failed to find list length.");
    int len = ret1.value.integral;

    struct type **multiple = gc_add(scope->gc, malloc(sizeof(struct type *) * (len + 1)));
    multiple[len] = NULL;
    
    ret->value.tuple = gc_add(scope->gc, malloc(sizeof(data_t *) * (len + 1)));
    ret->value.tuple[len] = NULL;

    list_node_t *node;
    list_iterator_t *it = list_iterator_new(args[0]->value.list, LIST_HEAD);

    int i = 0;

    while ((node = list_iterator_next(it))) {
        ret->value.tuple[i] = copy_data((data_t *) node->val, scope);
        multiple[i] = ret->value.tuple[i]->type;
        check_mem(ret->value.tuple[i]);

        ++i;
    }

    ret->type = construct_type(tid_tuple, multiple, scope->gc);

    if (it) list_iterator_destroy(it);
    return 0;

error:
    if (it) list_iterator_destroy(it);
    return -1;
}