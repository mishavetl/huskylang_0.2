/**
 * Functions API
 */

STDFUNCTION(function__return,
    if (!scope->parent) {
        scope->error = gc_add(scope->gc, malloc(sizeof(huserr_t)));
        scope->error->name = "returnError";
        scope->error->msg = "scope has no parent";
        goto error;
    }

    type_t *copy = copy_type(args[0], scope->parent);

    ret->type = copy->type;
    ret->value = copy->value;
    
    return 0;

    error:
    return -1;
)

/** @todo */
STDFUNCTION(function__create,
    ret->type = tid_fn;
    ret->value.fn = create_function(
        NULL, args[0]->value.tree, 0, (const unsigned []) {}, 0, scope->gc
    );

    check_mem(ret->value.fn);

    return 0;

    error:
    return -1;
)
