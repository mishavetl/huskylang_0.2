/**
 * Functions API
 */

STDFUNCTION(function__return,
    type_t *copy = copy_type(args[0], scope);

    ret->type = copy->type;
    ret->value = copy->value;

    return 0;
)
