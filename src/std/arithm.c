/**
 * Arithmetic API
 */

int plus(type_t **args, argc_t argc, type_t *ret, scope_t *scope)
{
    size_t i;

    ret->type = tid_num;
    ret->value.num = 0;

    for (i = 0; i < argc; i++) {
        ret->value.num += args[i]->value.num;
    }

    return 0;

    error:

    return -1;
}

int minus(type_t **args, argc_t argc, type_t *ret, scope_t *scope)
{
    size_t i;

    ret->type = tid_num;
    ret->value.num = 0;

    for (i = 0; i < argc; i++) {
        ret->value.num -= args[i]->value.num;
    }

    return 0;

    error:

    return -1;
}
