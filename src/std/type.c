/**
 * Types API
 */

STDFUNCTION(typename,
    const char *typenames[] = TYPE_NAMES_MAP;

    ret->type = tid_tuple;

    ret->value.tuple = gc_add(scope->gc, malloc(sizeof(type_t *) * 3));

    ret->value.tuple[0] = gc_add(scope->gc, malloc(sizeof(type_t)));
    ret->value.tuple[0]->type = tid_atom;
    ret->value.tuple[0]->value.atom =
        gc_add(scope->gc, strdup("type"));

    ret->value.tuple[1] = gc_add(scope->gc, malloc(sizeof(type_t)));
    ret->value.tuple[1]->type = tid_atom;
    ret->value.tuple[1]->value.atom =
        gc_add(scope->gc, strdup(typenames[args[0]->type]));

    ret->value.tuple[2] = NULL;

    return 0;
)