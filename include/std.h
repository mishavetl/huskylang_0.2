/**
 * @file
 * @brief Standard husky programming language library
 */
#ifndef __std_h__
#define __std_h__

#include "variable.h"
#include "function.h"
#include "type.h"

/**
 * @brief Macro for creation of `get_stdlib_variables` function
 *
 * @param n Number of std functions
 * @param ... Std functions
 */
#define STDFUNCTIONS(n, ...)                                                    \
    int get_stdlib_variables(scope_t *scope)                                    \
    {                                                                           \
        int count = 0;                                                          \
        type_t *fn;                                                             \
        scope->vars = (var_t **) malloc(sizeof(var_t *) * (n + 1));             \
                                                                                \
        scope->vsize = n;                                                       \
        scope->vars[0] = NULL;                                                  \
        __VA_ARGS__                                                             \
        return 0;                                                               \
    }

/**
 * @brief Macro for registration of std functions
 *
 * @param fname Name of the function
 * @param fn_ Function pointer
 */
#define REGSTDFUNCTION(fname, fn_)                                              \
    fn = (type_t *) gc_add(scope->gc, malloc(sizeof(type_t)));                  \
    fn->type = tid_fn;                                                          \
    fn->value.fn = fn_;                                                         \
                                                                                \
    scope->vars[count] = (var_t *) gc_add(scope->gc, malloc(sizeof(var_t)));    \
    scope->vars[count]->name = fname;                                           \
    scope->vars[count++]->value = fn;                                           \
    scope->vars[count] = NULL;

/**
 * @brief Macro for creation of std functions
 *
 * @param fname Name of the function
 * @param ... Function body
 */
#define STDFUNCTION(fname, ...)                                                 \
    int fname(type_t **args, argc_t argc, type_t *ret, scope_t *scope)          \
    {                                                                           \
        (void) argc;                                                            \
        (void) args;                                                            \
        (void) scope;                                                           \
                                                                                \
        __VA_ARGS__                                                             \
    }

/**
 * @brief Populates scope with standard library functions
 *
 * @param[out] scope Scope to populate
 *
 * @return Status: -1 if error else 0
 */
int get_stdlib_variables(scope_t *scope);

#endif
