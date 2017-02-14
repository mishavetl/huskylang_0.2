/**
 * Arithmetic API
 */

STDFUNCTION(plus,
    int i;

    ret->type = construct_type(tid_integral, NULL, scope->gc);
    ret->value.integral = 0;

    for (i = 0; i < argc; i++) {
        ret->value.integral += args[i]->value.integral;
    }

    return 0;
)

// int minus(data_t **args, argc_t argc, data_t *ret, scope_t *scope)
// {
//     (void) scope;

//     int i = 0;

//     ret->type = tid_integral;
//     ret->value.num = 0;

//     if (argc > 1) {
//         ret->value.num = args[0]->value.num;
//         ++i;
//     }

//     for (; i < argc; i++) {
//         ret->value.num -= args[i]->value.num;
//     }

//     return 0;
// }

// int multiply(data_t **args, argc_t argc, data_t *ret, scope_t *scope)
// {
//     (void) scope;
    
//     int i;

//     ret->type = tid_integral;
//     ret->value.num = 1;

//     for (i = 0; i < argc; i++) {
//         ret->value.num *= args[i]->value.num;
//     }

//     return 0;
// }
