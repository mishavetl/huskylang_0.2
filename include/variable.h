#ifndef __variable_h__
#define __variable_h__

#include "type.h"
#include "huserr.h"
#include "tokenizer.h"
#include "memory.h"

/**
 * STRUCT variable
 *
 * @author mishavetl
 *
 * @member name (name of a variable)
 * @member value (value of a variable)
 */
typedef struct variable {
    const char *name;
    type_t *value;
} var_t;

/**
 * STRUCT scope
 *
 * @author mishavetl
 *
 * @member parent (parent scope)
 * @member error (error of a scope)
 * @member vsize (size of the `vars` array)
 * @member vars (array of variables)
 * @member gc (garbage collector)
 */
typedef struct scope {
    struct scope *parent;
    huserr_t *error;
    size_t vsize;
    var_t **vars;
    gc_t *gc;
} scope_t;

/**
 * FUNCTION sets variable of a scope
 *
 * @author mishavetl
 *
 * @param scope (scope where to set a variable)
 * @param name (name of a variable)
 * @param value (value of a variable)
 *
 * @return int (status)
 */
int setvar(scope_t *scope, const char *name, type_t *value);

/**
 * FUNCTION gets a variable from a scope
 *
 * @author mishavetl
 *
 * @param scope (scope where to set a variable)
 * @param name (name of a variable)
 * @param value (value of a variable)
 *
 * @return int (index of a variable ; -1 if doesn't exist)
 */
int getvar(const scope_t *scope, const char *name);

/**
 * FUNCTION initiates a scope
 *
 * @author mishavetl
 *
 * @return scope
 */
scope_t scope__init();

#endif
