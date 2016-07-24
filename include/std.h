#ifndef __std_h__
#define __std_h__

#include "variable.h"
#include "function.h"
#include "type.h"

#define STDFUNCTIONS(n, ...)                                                    \
    scope_t *get_stdlib_variables()                                             \
    {                                                                           \
        int count = 0;                                                          \
        type_t *fn;                                                             \
        scope_t *scope = (scope_t *) gc_add(                                    \
            gc_global, malloc(sizeof(scope_t)));                                \
        scope->vars = (var_t **) gc_add(                                        \
            gc_global, malloc(sizeof(var_t *) * (n + 1)));                      \
                                                                                \
        scope->vars[0] = NULL;                                                  \
        __VA_ARGS__                                                             \
                                                                                \
        return scope;                                                           \
    }

#define REGSTDFUNCTION(fname, fn_)                                              \
    fn = (type_t *) gc_add(gc_global, malloc(sizeof(type_t)));                  \
    fn->type = tid_fn;                                                          \
    fn->value.fn = fn_;                                                         \
                                                                                \
    scope->vars[count] = (var_t *) gc_add(gc_global, malloc(sizeof(var_t)));    \
    scope->vars[count]->name = fname;                                           \
    scope->vars[count++]->value = fn;                                           \
    scope->vars[count] = NULL;

/**
 * Get standard library functions array
 *
 * @author mishavetl
 *
 * @return (variables array)
 *
 */
scope_t *get_stdlib_variables();

#endif
