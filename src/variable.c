#include <stdlib.h>
#include <string.h>

#include "dbg.h"
#include "variable.h"
#include "memory.h"
#include "list/src/list.h"

scope_t scope__init()
{
    return (scope_t) {
        .parent = NULL,
        .error = NULL,
        .vsize = 0,
        .vars = NULL,
        .gc = NULL
    };
}

int setvar(scope_t *scope, const char *name, data_t *value)
{
    scope->vars = realloc(scope->vars, sizeof(var_t *) * ++scope->vsize);
    check_mem(scope->vars);

    scope->vars[scope->vsize - 1] = gc_add(scope->gc, malloc(sizeof(var_t)));
    check_mem(scope->vars[scope->vsize - 1]);

    scope->vars[scope->vsize - 1]->name = gc_add(scope->gc, strdup(name));
    check_mem(scope->vars[scope->vsize - 1]->name);

    scope->vars[scope->vsize - 1]->value = copy_data(value, scope);
    check_mem(scope->vars[scope->vsize - 1]->value);

    return 0;

    error:
    return -1;
}

unsigned getvar_start(const scope_t *scope, unsigned start, const char *name, const var_t **var)
{
    int i, i_start = scope->vsize - start - 1;
    var_t **vars = (var_t **) scope->vars;

    for (i = i_start; i >= 0; --i) {
        if (strcmp(vars[i]->name, name) == 0) {
            *var = vars[i];
            return scope->vsize - i - 1;
        }
    }

    if (scope->parent) {
        return scope->vsize + getvar_start(scope->parent, -i - 1, name, var);
    }
    
    *var = NULL;
    return scope->vsize;
}

unsigned getvar(const scope_t *scope, const char *name, const var_t **var)
{
    return getvar_start(scope, 0, name, var);
}