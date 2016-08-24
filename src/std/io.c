/**
 * IO API
 */

int io__puts(type_t **args, argc_t argc, type_t *ret, scope_t *scope)
{
    size_t i;

    ret->type = tid_atom;

    for (i = 0; args[i]; i++) {
        printf("%s", args[i]->value.string);
    }

    puts("");

    ret->value.atom = "good";

    return 0;
}

int io__putln(type_t **args, argc_t argc, type_t *ret, scope_t *scope)
{
    puts("");

    ret->type = tid_atom;
    ret->value.atom = "good";

    return 0;
}
