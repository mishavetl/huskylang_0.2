/**
 * Integral API
 */

int integral__plus(data_t **args, argc_t argc, data_t *ret, scope_t *scope)
{
    int i;

    ret->type = construct_type(tid_integral, NULL, scope->gc);
    ret->value.integral = 0;

    for (i = 0; i < argc; i++) {
        ret->value.integral += args[i]->value.integral;
    }

    return 0;
}

int integral__minus(data_t **args, argc_t argc, data_t *ret, scope_t *scope)
{
    int i;

    ret->type = construct_type(tid_integral, NULL, scope->gc);
    ret->value.integral = 0;

    for (i = 0; i < argc; i++) {
        ret->value.integral -= args[i]->value.integral;
        if (argc > 1 && i == 0) ret->value.integral *= -1;
    }

    return 0;
}

int integral__multiply(data_t **args, argc_t argc, data_t *ret, scope_t *scope)
{
    int i;

    ret->type = construct_type(tid_integral, NULL, scope->gc);
    ret->value.integral = 1;

    for (i = 0; i < argc; i++) {
        ret->value.integral *= args[i]->value.integral;
    }

    return 0;
}

int integral__to_string(data_t **args, argc_t argc, data_t *ret, scope_t *scope)
{
    (void) argc;

    INTEGRAL_TYPE n = args[0]->value.integral;
    if (n <= 0) {
        n = -n + 1;
    }
    
    char *string = gc_add(scope->gc, malloc(sizeof(char) * (log10(n) + 2)));
    check_mem(string);

    sprintf(string, ((n < 0) ? "-%d" : "%d"), args[0]->value.integral);

    ret->type = construct_type(tid_string, NULL, scope->gc);
    ret->value.string = string;

    return 0;

error:
    return -1;
}

int integral__to_real(data_t **args, argc_t argc, data_t *ret, scope_t *scope)
{
    (void) argc;

    ret->type = construct_type(tid_real, NULL, scope->gc);
    ret->value.real = (REAL_TYPE) args[0]->value.integral;

    return 0;
}

int integral__equal(data_t **args, argc_t argc, data_t *ret, scope_t *scope)
{
    (void) argc;

    ret->type = construct_type(tid_atom, NULL, scope->gc);
    ret->value.atom = gc_add(scope->gc, strdup(
            (args[0]->value.integral == args[1]->value.integral) ? BOOLEAN_TRUE : BOOLEAN_FALSE
        )
    );

    return 0;
}

int integral__bigger(data_t **args, argc_t argc, data_t *ret, scope_t *scope)
{
    (void) argc;

    ret->type = construct_type(tid_atom, NULL, scope->gc);
    ret->value.atom = gc_add(scope->gc, strdup(
            (args[0]->value.integral > args[1]->value.integral) ? BOOLEAN_TRUE : BOOLEAN_FALSE
        )
    );
    
    return 0;
}

int integral__smaller(data_t **args, argc_t argc, data_t *ret, scope_t *scope)
{
    (void) argc;

    ret->type = construct_type(tid_atom, NULL, scope->gc);
    ret->value.atom = gc_add(scope->gc, strdup(
            (args[0]->value.integral < args[1]->value.integral) ? BOOLEAN_TRUE : BOOLEAN_FALSE
        )
    );
    
    return 0;
}