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

STDFUNCTION(io__putln,
    puts("");

    ret->type = tid_atom;
    ret->value.atom = "good";

    return 0;
)
