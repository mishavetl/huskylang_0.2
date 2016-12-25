#include <string.h>
#include "performer.h"
#include "function.h"
#include "type.h"
#include "memory.h"
#include "variable.h"
#include "call_tree.h"
#include "huserr.h"
#include "dbg.h"

int
performer__funcall(
    call_tree_t *tree, scope_t *scope, type_t *ret, mapv_t i
) {
    mapv_t j;
    int size = 0;
    type_t *fn, *type;
    type_t **args;
    const var_t *var_fn;
    gc_t gc = gc_init();

    if (tree->is_saved[i]) {
        ret->type = tid_saved;
        ret->value.tree = call_tree__duplicate(tree, scope->gc);
        ret->value.tree->start = i;
        ret->value.tree->is_saved[i] = false;
        return 0;
    }

    check_mem(type = gc_add(&gc, malloc(sizeof(type_t))));
    check_mem(args = (type_t **)
        gc_add(&gc,
            malloc(sizeof(type_t *) * (count_mapv(tree->map[i]) + 1))
        )
    );

    if (tree->map[tree->map[i][0]] && tree->map[i][0] != i) {
        performer__funcall(tree, scope, type, tree->map[i][0]);
    } else {
        check(type_from_token(tree->tokens[i], type) >= 0,
            "Failed to construct type."
        );
    }

    if (type->type != tid_atom) {
        scope->error = gc_add(scope->gc, malloc(sizeof(huserr_t)));
        scope->error->name = "typeErr";
        scope->error->msg = "is not atom";
        scope->error->token = tree->tokens[i];

        goto error;
    }

    var_fn = getvar((const scope_t *) scope, type->value.atom);

    if (!var_fn) {
        scope->error = gc_add(scope->gc, malloc(sizeof(huserr_t)));
        scope->error->name = "nameErr";
        scope->error->msg = "function undefined";
        scope->error->token = tree->tokens[i];

        goto error;
    }

    fn = var_fn->value;

    if (fn->type != tid_fn) {
        scope->error = gc_add(scope->gc, malloc(sizeof(huserr_t)));
        scope->error->name = "typeErr";
        scope->error->msg = "is not function";
        scope->error->token = tree->tokens[i];

        goto error;
    }

    scope_t scope_ = scope__init();
    gc_t gc_ = gc_init();
    scope_.gc = &gc_;
    scope_.parent = scope;

    for (j = 1; tree->map[i][j] != TERMINATE_MAPV; j++) {
        if (tree->map[i][j] == EMPTY_MAPV) continue;

        check_mem(type = gc_add(&gc, malloc(sizeof(type_t))));

        if (!tree->map[tree->map[i][j]]) {
            check(
                type_from_token(tree->tokens[tree->map[i][j]], type) >= 0,
                "Failed to construct type."
            );
        } else {
            performer__funcall(tree, scope, type, tree->map[i][j]);

            if (scope->error) {
                goto error;
            }
        }

        args[size] = type;

        if (fn->value.fn->argnames_size > size) {
            setvar(&scope_, fn->value.fn->argnames[size], args[size]);
        }

        if (fn->value.fn->argtypes_size != 0) {
            if (type->type !=
                fn->value.fn->argtypes[size % fn->value.fn->argtypes_size]
            ) {
                scope->error = gc_add(scope->gc, malloc(sizeof(huserr_t)));
                scope->error->name = "typeErr";
                scope->error->msg = "argument type mismatch";
                scope->error->token = tree->tokens[tree->map[i][j]];
                goto error;
            }
        }

        size++;
    }

    args[size] = NULL;

    if (size != fn->value.fn->argc && fn->value.fn->argc != INFINITY_ARGS) {
        scope->error = gc_add(scope->gc, malloc(sizeof(huserr_t)));
        scope->error->name = "argumentErr";
        scope->error->msg = "invalid arguments number";
        scope->error->token = tree->tokens[i];
        goto error;
    }

    if (fn->value.fn->tree) {
        performer__execute(fn->value.fn->tree, &scope_, ret);

        if (scope_.error) {
            scope->error = scope_.error;
            scope->error->msg = gc_add(scope->gc, strdup(scope->error->msg));

            goto error;
        }
        
        type_t *copy = copy_type(ret, scope);

        ret->value = copy->value;

        gc_clean(&gc_);
    } else if (fn->value.fn->callback(args, size, ret, scope) <= 0 && scope->error) {
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
