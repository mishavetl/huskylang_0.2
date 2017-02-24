/**
 * Variables API
 */

int var__set(data_t **args, argc_t argc, data_t *ret, scope_t *scope)
{
    (void) argc;
    ret->type = construct_type(tid_atom, NULL, scope->gc);

    const var_t *var;
    getvar(scope, args[0]->value.atom, &var);

    // /* Check that variable does not exist. */
    // if (var) {
    //     scope->error = gc_add(scope->gc, malloc(sizeof(huserr_t)));
    //     scope->error->name = "nameErr";
    //     scope->error->msg = "variable already defined";
    //     scope->error->token = NULL;
    //     goto error;
    // }

    /* Set variable. */
    checkf(setvar(scope, args[0]->value.atom, args[1]) >= 0, "Can't set a variable.");

    ret->value.atom = "good";

    return 0;

error:
    return -1;
}

int var__get(data_t **args, argc_t argc, data_t *ret, scope_t *scope)
{
    (void) argc;
    const var_t *var;
    getvar(scope, args[0]->value.atom, &var);

    /* Check that variable exists. */
    if (!var) {
        scope->error = gc_add(scope->gc, malloc(sizeof(huserr_t)));
        scope->error->name = "nameErr";
        scope->error->msg = "undefined variable";
        scope->error->token = NULL;
        goto error;
    }

    data_t *data = copy_data(var->value, scope);

    ret->type = data->type;
    ret->value = data->value;

    return 0;

error:
    return -1;
}
