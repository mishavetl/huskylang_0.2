/**
 * Tuples API
 */

int list__construct(type_t **args, argc_t argc, type_t *ret, scope_t *scope)
{
    argc_t i;
    type_t *type;

    ret->type = tid_list;
    ret->value.list = gc_add_list(scope->gc, list_new());
    check_mem(ret->value.list);

    for (i = 0; i < argc; i++) {
        type = gc_add(scope->gc, malloc(sizeof(type_t)));
        check_mem(type);

        type->type = args[i]->type;
        type->value = args[i]->value;

        check_mem(list_rpush(ret->value.list, list_node_new(type)));
    }

    return 0;

error:
    return -1;
}

int list__head(type_t **args, argc_t argc, type_t *ret, scope_t *scope)
{
    (void) argc;
    
    if (args[0]->value.list->len == 0) {
        scope->error = gc_add(scope->gc, malloc(sizeof(huserr_t)));
        scope->error->token = NULL;
        scope->error->name = "listErr";
        scope->error->msg = "empty list has no head";
        goto error;
    }

    ret->type = ((type_t *) args[0]->value.list->head->val)->type;
    ret->value = ((type_t *) args[0]->value.list->head->val)->value;

    return 0;

error:
    return -1;
}

int list__tail(type_t **args, argc_t argc, type_t *ret, scope_t *scope)
{
    (void) argc;

    list_node_t *node;
    list_iterator_t *it = list_iterator_new(args[0]->value.list, LIST_HEAD);
    check_mem(it);

    ret->type = tid_list;
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

int list__length(type_t **args, argc_t argc, type_t *ret, scope_t *scope)
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
    
    ret->type = tid_num;
    ret->value.num = length;

    if (it) list_iterator_destroy(it);

    return 0;

error:
    if (it) list_iterator_destroy(it);
    return -1;
}

int list__unzip(type_t **args, argc_t argc, type_t *ret, scope_t *scope)
{
    (void) argc;

    type_t length_in_lang;
    list__length(args, 1, &length_in_lang, scope);

    int length = length_in_lang.value.num;

    ret->type = tid_tuple;
    ret->value.tuple = gc_add(scope->gc, malloc(sizeof(type_t *) * 3));
    check_mem(ret->value.tuple);

    ret->value.tuple[0] = gc_add(scope->gc, malloc(sizeof(type_t)));
    check_mem(ret->value.tuple[0]);
    
    ret->value.tuple[0]->type = tid_tuple;
    ret->value.tuple[0]->value.tuple = gc_add(scope->gc, malloc(sizeof(type_t *) * (length + 1)));
    check_mem(ret->value.tuple[0]->value.tuple);

    ret->value.tuple[1] = gc_add(scope->gc, malloc(sizeof(type_t)));
    check_mem(ret->value.tuple[1]);
    
    ret->value.tuple[1]->type = tid_tuple;
    ret->value.tuple[1]->value.tuple = gc_add(scope->gc, malloc(sizeof(type_t *) * (length + 1)));
    check_mem(ret->value.tuple[1]->value.tuple);
    
    ret->value.tuple[2] = NULL;

    list_node_t *node;
    list_iterator_t *it = list_iterator_new(args[0]->value.list, LIST_HEAD);

    int i = 0;

    while ((node = list_iterator_next(it))) {
        if (((type_t *) node->val)->type != tid_tuple) {
            scope->error = gc_add(scope->gc, malloc(sizeof(huserr_t)));
            scope->error->name = "typeErr";
            scope->error->msg = "must be a zipped list";
            scope->error->token = NULL;
            goto error;
        }

        ret->value.tuple[0]->value.tuple[i] = copy_type(((type_t *) node->val)->value.tuple[0], scope);
        check_mem(ret->value.tuple[0]->value.tuple[i]);

        ret->value.tuple[1]->value.tuple[i] = copy_type(((type_t *) node->val)->value.tuple[1], scope);
        check_mem(ret->value.tuple[1]->value.tuple[i]);

        ++i;
    }

    ret->value.tuple[0]->value.tuple[i] = NULL;
    ret->value.tuple[1]->value.tuple[i] = NULL;

    if (it) list_iterator_destroy(it);

    return 0;

error:
    if (it) list_iterator_destroy(it);
    return -1;
}
