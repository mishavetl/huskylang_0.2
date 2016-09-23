/**
 * @file
 * @brief Variable and scope structures and functions to manage them
 */
#ifndef __variable_h__
#define __variable_h__

#include "type.h"
#include "huserr.h"
#include "tokenizer.h"
#include "memory.h"

/**
 * @brief Variable
 */
typedef struct variable {
    /** Name of a variable */
    const char *name;
    /** Value of a variable */
    type_t *value;
} var_t;

/**
 * @brief Scope
 */
typedef struct scope {
    /** Parent scope */
    struct scope *parent;
    /** Error of a scope */
    huserr_t *error;
    /** Size of the `vars` array */
    size_t vsize;
    /** Array of variables */
    var_t **vars;
    /** Garbage collector */
    gc_t *gc;
} scope_t;

/**
 * @brief Sets variable of a scope
 *
 * @param[out] scope Scope where to set a variable
 * @param[in] name Name of a variable
 * @param[in] value Value of a variable
 *
 * @return Status: if success then 0 else -1
 */
int setvar(scope_t *scope, const char *name, type_t *value);

/**
 * @brief Gets a variable from a scope
 *
 * @param[in] scope Scope where to set a variable
 * @param[in] name Name of a variable
 *
 * @return Index of a variable ; -1 if doesn't exist
 */
int getvar(const scope_t *scope, const char *name);

/**
 * @brief Initiates a scope
 *
 * @return Initiated scope
 */
scope_t scope__init();

#endif
