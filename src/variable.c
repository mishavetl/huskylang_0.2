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

unsigned getvar(const scope_t *scope, unsigned start, const char *name, const var_t **var)
{
    unsigned i;
    var_t **vars = (var_t **) scope->vars;

    for (i = start; i < scope->vsize; i++) {
        if (strcmp(vars[i]->name, name) == 0) {
            *var = vars[i];
            return i;
        }
    }

    if (scope->parent) {
        return i + getvar(scope->parent, i - scope->vsize, name, var);
    } else {
        *var = NULL;
    }
    
    return i;
}
