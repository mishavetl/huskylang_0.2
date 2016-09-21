/**
 * Number API
 */

STDFUNCTION(number__to_string,
    char *string = gc_add(scope->gc, malloc(sizeof(char) * (log10(args[0]->value.num) + 2)));

    check_mem(string);

    sprintf(string, "%d", args[0]->value.num);

    ret->type = tid_string;
    ret->value.string = string;

    return 0;

    error:

    return -1;
)
