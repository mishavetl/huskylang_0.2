#ifndef __variable_h__
#define __variable_h__

#include "type.h"
#include "tokenizer.h"
#include "memory.h"

typedef struct variable {
    const char *name;
    type_t *value;
} var_t;

typedef struct scope {
    size_t vsize;
    var_t **vars;
    gc_t *gc;
} scope_t;

int setvar(scope_t *scope, const char *name, type_t *value);
int getvar(const scope_t *scope, const char *name);

#endif
