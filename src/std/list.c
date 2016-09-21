/**
 * Tuples API
 */

STDFUNCTION(list__construct,
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
)

STDFUNCTION(list__head,
    if (args[0]->value.list->len == 0) {
        scope->error = gc_add(scope->gc, malloc(sizeof(huserr_t)));
        scope->error->name = "listErr";
        scope->error->msg = "empty list has no head";
        goto error;
    }

    ret->type = ((type_t *) args[0]->value.list->head->val)->type;
    ret->value = ((type_t *) args[0]->value.list->head->val)->value;

    return 0;

    error:
    return -1;
)

STDFUNCTION(list__tail,
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
)
