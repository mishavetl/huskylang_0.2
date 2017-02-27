/**
 * Real API
 */

int real__plus(data_t **args, argc_t argc, data_t *ret, scope_t *scope)
{
    int i;

    ret->type = construct_type(tid_real, NULL, scope->gc);
    ret->value.real = 0;

    for (i = 0; i < argc; i++) {
        ret->value.real += args[i]->value.real;
    }

    return 0;
}

int real__minus(data_t **args, argc_t argc, data_t *ret, scope_t *scope)
{
    int i;

    ret->type = construct_type(tid_real, NULL, scope->gc);
    ret->value.real = 0;

    for (i = 0; i < argc; i++) {
        ret->value.real -= args[i]->value.real;
        if (argc > 1 && i == 0) ret->value.real *= -1;
    }

    return 0;
}

int real__multiply(data_t **args, argc_t argc, data_t *ret, scope_t *scope)
{
    int i;

    ret->type = construct_type(tid_real, NULL, scope->gc);
    ret->value.real = 1;

    for (i = 0; i < argc; i++) {
        ret->value.real *= args[i]->value.real;
    }

    return 0;
}

int real__to_string(data_t **args, argc_t argc, data_t *ret, scope_t *scope)
{
    (void) argc;

    REAL_TYPE n = args[0]->value.real;
    if (n <= 0) {
        n = -n + 1;
    }
    
    char *string = gc_add(scope->gc, malloc(sizeof(char) * (log10(n) + 2 + 20)));
    check_mem(string);

    sprintf(string, ((n < 0) ? "-%lf" : "%lf"), args[0]->value.real);

    ret->type = construct_type(tid_string, NULL, scope->gc);
    ret->value.string = string;

    return 0;

error:
    return -1;
}

int real__equal(data_t **args, argc_t argc, data_t *ret, scope_t *scope)
{
    (void) argc;

    ret->type = construct_type(tid_atom, NULL, scope->gc);
    ret->value.atom = gc_add(scope->gc, strdup(
            (args[0]->value.real == args[1]->value.real) ? BOOLEAN_TRUE : BOOLEAN_FALSE
        )
    );

    return 0;
}

int real__bigger(data_t **args, argc_t argc, data_t *ret, scope_t *scope)
{
    (void) argc;

    ret->type = construct_type(tid_atom, NULL, scope->gc);
    ret->value.atom = gc_add(scope->gc, strdup(
            (args[0]->value.real > args[1]->value.real) ? BOOLEAN_TRUE : BOOLEAN_FALSE
        )
    );
    
    return 0;
}

int real__smaller(data_t **args, argc_t argc, data_t *ret, scope_t *scope)
{
    (void) argc;

    ret->type = construct_type(tid_atom, NULL, scope->gc);
    ret->value.atom = gc_add(scope->gc, strdup(
            (args[0]->value.real < args[1]->value.real) ? BOOLEAN_TRUE : BOOLEAN_FALSE
        )
    );
    
    return 0;
}

int real__to_integral(data_t **args, argc_t argc, data_t *ret, scope_t *scope)
{
    (void) argc;

    ret->type = construct_type(tid_integral, NULL, scope->gc);
    ret->value.integral = (INTEGRAL_TYPE) args[0]->value.real;

    return 0;
}