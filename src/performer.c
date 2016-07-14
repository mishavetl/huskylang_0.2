#include <string.h>
#include "performer.h"
#include "type.h"
#include "memory.h"
#include "variable.h"
#include "call_tree.h"

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

int performer__funcall(call_tree_t *tree, var_t **vars, type_t *ret, mapv_t i)
{
    mapv_t j;
    size_t size = 0;
    type_t *fn, *type;
    type_t **args;
    gc_t gc = gc__init();

    check_mem(type = gc__add(&gc, malloc(sizeof(type_t))));
    check_mem(args = gc__add(&gc, malloc(sizeof(type_t *) * (count_mapv(tree->map[i]) + 1))));

    check(type_from_token(tree->tokens[i], type) >= 0, "Failed to construct type.");

    check((j = find_variable(vars, type->value.atom)) >= 0, "Function is not defined '%s'.", tree->tokens[i]->value);

    fn = vars[j]->value;

    for (j = 0; tree->map[i][j] != TERMINATE_MAPV; j++) {
        if (tree->map[i][j] == EMPTY_MAPV) continue;

        check_mem(type = gc__add(&gc, malloc(sizeof(type_t))));

        if (!tree->map[tree->map[i][j]]) {
            check(type_from_token(tree->tokens[tree->map[i][j]], type) >= 0, "Failed to construct type.");
        } else {
            check(performer__funcall(tree, vars, type, tree->map[i][j]) >= 0, "Failed to execute function call as argument.");
        }

        args[size++] = type;
    }

    args[size] = NULL;

    fn->value.fn(args, ret);

    gc__clean(&gc);

    return 0;

    error:

    gc__clean(&gc);

    return -1;
}

int performer__execute(call_tree_t *tree, var_t **vars, type_t *ret)
{
    check(performer__funcall(tree, vars, ret, tree->start) >= 0, "Failed to execute function call.");

    return 0;

    error:

    return -1;
}
