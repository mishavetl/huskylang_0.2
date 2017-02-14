/**
 * IO API
 */

STDFUNCTION(io__puts,
    size_t i;

    ret->type = tid_atom;

    for (i = 0; args[i]; i++) {
        printf("%s", args[i]->value.string);
    }

    puts("");

    ret->value.atom = "good";

    return 0;
)

int io__gets(data_t **args, argc_t argc, data_t *ret, scope_t *scope)
{
    (void) argc;
    (void) args;

    ret->type = tid_string;

    char *string = NULL;

    size_t i = 0;
    ssize_t count = getline(&string, &i, stdin);
    check(count > 0, "Error getting input.");
    check_mem(string);

    if (string[count - 1] == '\n') {
        string[count - 1] = '\0';
    }

    gc_add(scope->gc, string);
    ret->value.string = string;

    return 0;

error:
    return -1;
}

STDFUNCTION(io__putln,
    puts("");

    ret->type = tid_atom;
    ret->value.atom = "good";

    return 0;
)
