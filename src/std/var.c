/**
 * Variables API
 */

STDFUNCTION(var__set,
    ret->type = tid_atom;

    /* Check that variable does not exist. */
    if (getvar(scope, args[0]->value.atom) >= 0) {
        scope->error = gc_add(scope->gc, malloc(sizeof(huserr_t)));
        scope->error->name = "nameErr";
        scope->error->msg = "variable already defined";
        goto error;
    }

    /* Set variable. */
    check(setvar(scope, args[0]->value.atom, args[1]) >= 0, "");

    ret->value.atom = "good";

    return 0;

    error:

    return -1;
)

STDFUNCTION(var__get,
    int i;

    /* Check that variable exists. */
    if ((i = getvar(scope, args[0]->value.atom)) < 0) {
        scope->error = gc_add(scope->gc, malloc(sizeof(huserr_t)));
        scope->error->name = "nameErr";
        scope->error->msg = "undefined variable";
        goto error;
    }

    ret->type = scope->vars[i]->value->type;
    ret->value = scope->vars[i]->value->value;

    return 0;

    error:

    return -1;
)
