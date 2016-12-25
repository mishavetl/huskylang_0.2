/**
 * Functions API
 */

STDFUNCTION(function__return,
    if (!scope->parent) {
        scope->error = gc_add(scope->gc, malloc(sizeof(huserr_t)));
        scope->error->name = "returnError";
        scope->error->msg = "scope has no parent";
        goto error;
    }

    type_t *copy = copy_type(args[0], scope->parent);

    ret->type = copy->type;
    ret->value = copy->value;
    
    return 0;

error:
    return -1;
)

/* TODO: function arguments */
int function__create(type_t **args, argc_t argc, type_t *ret, scope_t *scope)
{
    (void) argc;

    ret->type = tid_fn;

    char **argnames = NULL;
    unsigned *argtypes = NULL;

    int args_size = 0;

    type_t length;

    list__length(args, 1, &length, scope);

    if (scope->error) {
        goto error;
    }

    if (length.value.num > 0) {
        type_t unzipped;
        
        list__unzip(args, 1, &unzipped, scope); 

        type_t **argnames_in_lang = unzipped.value.tuple[0]->value.tuple;
        type_t **argtypes_in_lang = unzipped.value.tuple[1]->value.tuple;
        
        for (; argnames_in_lang[args_size]; ++args_size);

        argnames = gc_add(scope->gc, malloc(sizeof(char *) * args_size));
        argtypes = gc_add(scope->gc, malloc(sizeof(unsigned) * args_size));

        for (int i = 0; i < args_size; ++i) {
            type_t *argname_in_lang = argnames_in_lang[i];
            type_t *argtype_in_lang = argtypes_in_lang[i];
            
            if (argname_in_lang->type != tid_atom) {
                char msg[ERROR_MSG_BUFFER_MAX];
                snprintf(msg, ERROR_MSG_BUFFER_MAX, "argument #%d identifier is not an atom", i + 1);

                scope->error = gc_add(scope->gc, malloc(sizeof(huserr_t)));
                scope->error->name = "typeErr";
                scope->error->msg = strdup(msg);

                goto error;
            }

            if (argtype_in_lang->type != tid_atom) {
                char msg[ERROR_MSG_BUFFER_MAX];
                snprintf(msg, ERROR_MSG_BUFFER_MAX, "argument #%d type indentifier is not an atom", i + 1);

                scope->error = gc_add(scope->gc, malloc(sizeof(huserr_t)));
                scope->error->name = "typeErr";
                scope->error->msg = gc_add(scope->gc, strdup(msg));

                goto error;
            }

            argnames[i] = gc_add(scope->gc, strdup(argname_in_lang->value.atom));

            int tid = typename_to_tid(argtype_in_lang->value.atom);
            if (tid < 0) {
                char msg[ERROR_MSG_BUFFER_MAX];
                snprintf(msg, ERROR_MSG_BUFFER_MAX, "argument #%d type is not known", i + 1);

                scope->error = gc_add(scope->gc, malloc(sizeof(huserr_t)));
                scope->error->name = "typeErr";
                scope->error->msg = gc_add(scope->gc, strdup(msg));

                goto error;
            }

            argtypes[i] = tid;
        }

        ret->value.fn = create_function(
            NULL, args[1]->value.tree, args_size,
            argtypes, args_size,
            argnames, args_size,
            scope->gc
        );
    } else {
        ret->value.fn = create_function(
            NULL, args[1]->value.tree, 0,
            (const unsigned []) {}, args_size,
            argnames, args_size,
            scope->gc
        );
        /* argtypes = gc_add(scope->gc, malloc(sizeof(unsigned))); */
    }

    /* ret->value.fn = create_function( */
    /*     NULL, args[1]->value.tree, 0, */
    /*     (const unsigned []) argtypes, argtypes_size, */
    /*     argnames, argnames_size, */
    /*     scope->gc */
    /* ); */

    check_mem(ret->value.fn);

    return 0;

error:
    return -1;
}
