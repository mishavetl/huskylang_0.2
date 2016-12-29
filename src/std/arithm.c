/**
 * Arithmetic API
 */

STDFUNCTION(plus,
    int i;

    ret->type = tid_num;
    ret->value.num = 0;

    for (i = 0; i < argc; i++) {
        ret->value.num += args[i]->value.num;
    }

    return 0;
)

int minus(type_t **args, argc_t argc, type_t *ret, scope_t *scope)
{
    (void) scope;

    int i = 0;

    ret->type = tid_num;
    ret->value.num = 0;

    if (argc > 1) {
        ret->value.num = args[0]->value.num;
        ++i;
    }

    for (; i < argc; i++) {
        ret->value.num -= args[i]->value.num;
    }

    return 0;
}

int multiply(type_t **args, argc_t argc, type_t *ret, scope_t *scope)
{
    (void) scope;
    
    int i;

    ret->type = tid_num;
    ret->value.num = 1;

    for (i = 0; i < argc; i++) {
        ret->value.num *= args[i]->value.num;
    }

    return 0;
}
