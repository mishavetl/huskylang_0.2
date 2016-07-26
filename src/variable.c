#include <stdlib.h>
#include <string.h>

#include "variable.h"
#include "memory.h"

int setvar(scope_t *scope, const char *name, type_t *value)
{
    scope->vars = realloc(scope->vars, sizeof(var_t *) * ++scope->vsize);
    check_mem(scope->vars);

    scope->vars[scope->vsize - 1] = gc_add(scope->gc, malloc(sizeof(var_t)));
    check_mem(scope->vars[scope->vsize - 1]);

    scope->vars[scope->vsize - 1]->name = gc_add(scope->gc, strdup(name));
    scope->vars[scope->vsize - 1]->value = gc_add(scope->gc, malloc(sizeof(type_t)));
    scope->vars[scope->vsize - 1]->value->type = value->type;
    scope->vars[scope->vsize - 1]->value->value = value->value;

    if (value->type == tid_atom) {
        scope->vars[scope->vsize - 1]->value->value.atom =
            gc_add(scope->gc, strdup(value->value.atom));
    } else if (value->type == tid_string) {
        scope->vars[scope->vsize - 1]->value->value.string =
            gc_add(scope->gc, strdup(value->value.string));
    }

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
