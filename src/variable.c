#include "variable.h"
#include "memory.h"

extern gc_t *gc_global;

int setvar(scope_t *scope, const char *name, type_t *value)
{
    scope->vars = gc_add(gc_global, realloc(scope->vars, sizeof(var_t *) * ++scope->vsize));
    check_mem(scope->vars);

    scope->vars[scope->vsize - 1] = gc_add(gc_global, malloc(sizeof(var_t)));
    scope->vars[scope->vsize - 1]->name = name;
    scope->vars[scope->vsize - 1]->value = value;

    return 0;

    error:

    return -1;
}
