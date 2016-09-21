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

STDFUNCTION(minus,
    int i;

    ret->type = tid_num;
    ret->value.num = 0;

    for (i = 0; i < argc; i++) {
        ret->value.num -= args[i]->value.num;
    }

    return 0;
)
