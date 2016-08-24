#include <stdlib.h>
#include <string.h>

#include "variable.h"
#include "memory.h"
#include "list/src/list.h"

int setvar(scope_t *scope, const char *name, type_t *value)
{
    scope->vars = realloc(scope->vars, sizeof(var_t *) * ++scope->vsize);
    check_mem(scope->vars);

    scope->vars[scope->vsize - 1] = gc_add(scope->gc, malloc(sizeof(var_t)));
    check_mem(scope->vars[scope->vsize - 1]);

    scope->vars[scope->vsize - 1]->name = gc_add(scope->gc, strdup(name));
    check_mem(scope->vars[scope->vsize - 1]->name);

    scope->vars[scope->vsize - 1]->value = copy_type(value, scope);
    check_mem(scope->vars[scope->vsize - 1]->value);

    return 0;

    error:

    return -1;
}

int getvar(const scope_t *scope, const char *name)
{
    int i;
    const var_t **vars = scope->vars;

    for (i = 0; i < scope->vsize; i++) {
        if (strcmp(vars[i]->name, name) == 0) {
            return i;
        }
    }

    return -1;
}
