/**
 * String API
 */

int string__to_integral(data_t **args, argc_t argc, data_t *ret, scope_t *scope)
{
    (void) argc;
    (void) scope;

    ret->type = construct_type(tid_integral, NULL, scope->gc);

    unsigned i = 0;
    INTEGRAL_TYPE number = 0;

    const char *string = args[0]->value.string;

    if (string[0] == '-') {
        ++i;
    }

    for (; i < strlen(string); ++i) {
        if (!isdigit(string[i])) {
            scope->error = gc_add(scope->gc, malloc(sizeof(huserr_t)));
            scope->error->name = "convertionErr";
            scope->error->msg = "string format is not correct";
            scope->error->token = NULL;
            goto error;
        }

        number *= 10;
        number += string[i] - '0';
    }

    if (string[0] == '-') {
        number *= -1;
    }

    ret->value.integral = number;

    return 0;

error:
    return -1;
}

int string__add(data_t **args, argc_t argc, data_t *ret, scope_t *scope)
{
    (void) argc;
    ret->type = construct_type(tid_string, NULL, scope->gc);
    const char *src1 = args[0]->value.string, *src2 = args[1]->value.string;
    char *str1 = gc_add(scope->gc, malloc(sizeof(src1) + sizeof(src2) + 1));
    strcpy(str1, src1);
    ret->value.string = strcat(str1, src2);
    return 0;
}