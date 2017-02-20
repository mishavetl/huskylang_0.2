/**
 * Integral API
 */

STDFUNCTION(integral__plus,
    int i;

    ret->type = construct_type(tid_integral, NULL, scope->gc);
    ret->value.integral = 0;

    for (i = 0; i < argc; i++) {
        ret->value.integral += args[i]->value.integral;
    }

    return 0;
)

int integral__to_string(data_t **args, argc_t argc, data_t *ret, scope_t *scope)
{
    (void) argc;

    INTEGRAL_TYPE n = args[0]->value.integral;
    if (n <= 0) {
        n = -n + 1;
    }
    
    char *string = gc_add(scope->gc, malloc(sizeof(char) * (log10(n) + 2)));
    check_mem(string);

    sprintf(string, "%d", args[0]->value.integral);

    ret->type = construct_type(tid_string, NULL, scope->gc);
    ret->value.string = string;

    return 0;

error:
    return -1;
}
