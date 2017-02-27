#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "performer.h"
#include "function.h"
#include "data.h"
#include "memory.h"
#include "variable.h"
#include "call_tree.h"
#include "huserr.h"
#include "dbg.h"

int
performer__funcall(
    call_tree_t *tree, scope_t *scope, data_t *ret, mapv_t i
) {
    mapv_t j;
    int size = 0, x, var_fn_index = -1;
    data_t *fn, *data;
    data_t **args;
    const var_t *var_fn;
    gc_t gc = gc_init();
    bool fn_found = false;

    scope_t scope_ = scope__init();
    gc_t gc_ = gc_init();
    scope_.gc = &gc_;
    scope_.parent = scope;

    if (tree->is_saved[i]) {
        ret->type = construct_type(tid_saved, NULL, scope->gc);
        ret->value.tree = call_tree__duplicate(tree, scope->gc);
        ret->value.tree->start = i;
        ret->value.tree->is_saved[i] = false;
        return 0;
    }

    check_mem(data = gc_add(&gc, malloc(sizeof(data_t))));
    check_mem(args = (data_t **)
        gc_add(&gc,
            malloc(sizeof(data_t *) * (count_mapv(tree->map[i]) + 1))
        )
    );

    if (tree->map[tree->map[i][0]] && tree->map[i][0] != i) {
        performer__funcall(tree, scope, data, tree->map[i][0]);
    } else {
        checkf(data_from_token(tree->tokens[i], data, &gc) >= 0,
            "Failed to construct data."
        );
    }

    if (data->type->single != tid_atom) {
        scope->error = gc_add(scope->gc, malloc(sizeof(huserr_t)));
        scope->error->name = "typeErr";
        scope->error->msg = "is not atom";
        scope->error->token = tree->tokens[i];
        goto error;
    }
    const char *fnname = data->value.atom;

    for (j = 1; tree->map[i][j] != TERMINATE_MAPV; j++) {
        if (tree->map[i][j] == EMPTY_MAPV) continue;
        check_mem(data = gc_add(&gc, malloc(sizeof(data_t))));

        if (!tree->map[tree->map[i][j]]) {
            checkf(
                data_from_token(tree->tokens[tree->map[i][j]], data, &gc) >= 0,
                "Failed to construct data."
            );
        } else {
            performer__funcall(tree, scope, data, tree->map[i][j]);
            if (scope->error) {
                goto error;
            }
        }
        
        args[size] = data;
        size++;
    }
    args[size] = NULL;

    while (!fn_found) {
        var_fn_index = getvar_start((const scope_t *) scope, var_fn_index + 1, fnname, &var_fn);
        if (!var_fn) break;
        fn = var_fn->value;
        if (fn->type->single != tid_fn) continue;

        if (fn) {
            if (size == 0 && fn->type->multsize == 1) {
                fn_found = true;
                break;
            }
        }

        for (x = 0; x < size; ++x) {
            fn_found = true;
            struct type **types = fn->type->multiple;
            int index = (fn->value.fn->argc == INFINITY_ARGS)
                    ? x % (count_2d((void **) types) - 1)
                    : (unsigned) x;

            if (!types_identical(args[x]->type, types[index + 1])) {
                fn_found = false;
                break;
            }
        }
    }

    if (!fn_found) {
        scope->error = gc_add(scope->gc, malloc(sizeof(huserr_t)));
        scope->error->name = "argumentErr";
        scope->error->msg = "no function candidate";
        scope->error->token = tree->tokens[i];
        goto error;
    }

    for (x = 0; x < fn->value.fn->argnames_size; ++x) {
        setvar(&scope_, fn->value.fn->argnames[x], args[x]);
    }

    if (fn->value.fn->tree) {
        performer__execute(fn->value.fn->tree, &scope_, ret);

        if (scope_.error) {
            scope->error = gc_add(scope->gc, malloc(sizeof(huserr_t)));
            scope->error->name = gc_add(scope->gc, strdup(scope_.error->name));

            scope->error->token = gc_add(scope->gc, malloc(sizeof(token_t)));
            check_mem(scope->error->token);
            memcpy(scope->error->token, scope_.error->token, sizeof(token_t));

            scope->error->token->value = (char *) gc_add(scope->gc, strdup(scope_.error->token->value));

            scope->error->msg = gc_add(scope->gc, strdup(scope_.error->msg));
            check_mem(scope->error->msg);

            goto error;
        }
        
        data_t *copy = copy_data(ret, scope);

        ret->type = copy->type;
        ret->value = copy->value;
    } else if (fn->value.fn->callback(args, size, ret, scope) <= 0 && scope->error) {
        if (!scope->error->token) {
            scope->error->token = tree->tokens[i];
        }
        goto error;
    }

    gc_clean(&gc);
    gc_clean(scope_.gc);
    FREE(scope_.vars);
    return 0;

error:
    gc_clean(&gc);
    gc_clean(scope_.gc);
    FREE(scope_.vars);
    return -1;
}

int performer__execute(call_tree_t *tree, scope_t *scope, data_t *ret)
{
    performer__funcall(tree, scope, ret, tree->start);
    return 0;
}
