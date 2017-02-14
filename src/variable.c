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

const var_t *getvar(const scope_t *scope, const char *name)
{
    unsigned i;
    const var_t **vars = (const var_t **) scope->vars;

    for (i = 0; i < scope->vsize; i++) {
        if (strcmp(vars[i]->name, name) == 0) {
            return vars[i];
        }
    }

    if (scope->parent) {
        return getvar(scope->parent, name);
    }
    
    return NULL;
}
