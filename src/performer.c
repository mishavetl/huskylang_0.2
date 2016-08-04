#include <string.h>
#include "performer.h"
#include "function.h"
#include "type.h"
#include "memory.h"
#include "variable.h"
#include "call_tree.h"
#include "huserr.h"

int find_variable(const var_t **vars, const char *name)
{
    int i;

    for (i = 0; vars[i]; i++)
        if (strcmp(vars[i]->name, name) == 0)
            return i;

    return -1;
}

size_t count_mapv(mapv_t *ar)
{
    size_t len;

    for (len = 0; ar[len] != TERMINATE_MAPV; len++);

    return len;
}

int
performer__funcall(
    call_tree_t *tree, scope_t *scope, type_t *ret, mapv_t i)
{
    mapv_t j;
    size_t size = 0;
    type_t *fn, *type;
    type_t **args;
    gc_t gc = gc_init();

    check_mem(type = gc_add(&gc, malloc(sizeof(type_t))));
    check_mem(args =
        gc_add(&gc,
            malloc(sizeof(type_t *) * (count_mapv(tree->map[i]) + 1))));

    check(type_from_token(tree->tokens[i], type) >= 0,
        "Failed to construct type.");

    if (type->type != tid_atom) {
        scope->error = gc_add(scope->gc, malloc(sizeof(huserr_t)));
        scope->error->name = "typeErr";
        scope->error->msg = "is not atom";
        scope->error->token = tree->tokens[i];

        goto error;
    }

    j = find_variable(scope->vars, type->value.atom);

    if (j < 0) {
        scope->error = gc_add(scope->gc, malloc(sizeof(huserr_t)));
        scope->error->name = "nameErr";
        scope->error->msg = "function undefined";
        scope->error->token = tree->tokens[i];

        goto error;
    }

    fn = scope->vars[j]->value;

    if (fn->type != tid_fn) {
        scope->error = gc_add(scope->gc, malloc(sizeof(huserr_t)));
        scope->error->name = "typeErr";
        scope->error->msg = "is not function";
        scope->error->token = tree->tokens[i];

        goto error;
    }

    for (j = 0; tree->map[i][j] != TERMINATE_MAPV; j++) {
        if (tree->map[i][j] == EMPTY_MAPV) continue;

        check_mem(type = gc_add(&gc, malloc(sizeof(type_t))));

        if (!tree->map[tree->map[i][j]]) {
            check(
                type_from_token(tree->tokens[tree->map[i][j]], type) >= 0,
                "Failed to construct type.");
        } else {
            performer__funcall(tree, scope, type, tree->map[i][j]);

            if (scope->error) {
                goto error;
            }
        }

        args[size] = type;

        if (type->type !=
            fn->value.fn->argtypes[size % fn->value.fn->argtypes_size])
        {
            scope->error = gc_add(scope->gc, malloc(sizeof(huserr_t)));
            scope->error->name = "typeErr";
            scope->error->msg = "argument type mismatch";
            scope->error->token = tree->tokens[tree->map[i][j]];
            goto error;
        }

        size++;
    }

    args[size] = NULL;

    if (size !=
        fn->value.fn->argc && fn->value.fn->argc != INFINITY_ARGS)
    {
        scope->error = gc_add(scope->gc, malloc(sizeof(huserr_t)));
        scope->error->name = "argumentErr";
        scope->error->msg = "invalid arguments number";
        scope->error->token = tree->tokens[i];
        goto error;
    }

    if (fn->value.fn->callback(args, size, ret, scope) <= 0 && scope->error) {
        scope->error->token = tree->tokens[i];
        goto error;
    }

    gc_clean(&gc);

    return 0;

    error:

    gc_clean(&gc);

    return -1;
}

int performer__execute(call_tree_t *tree, scope_t *scope, type_t *ret)
{
    performer__funcall(tree, scope, ret, tree->start);

    return 0;
}
