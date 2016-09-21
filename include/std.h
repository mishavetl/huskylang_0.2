#ifndef __std_h__
#define __std_h__

#include "variable.h"
#include "function.h"
#include "type.h"

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

#define REGSTDFUNCTION(fname, fn_)                                              \
    fn = (type_t *) gc_add(scope->gc, malloc(sizeof(type_t)));                  \
    fn->type = tid_fn;                                                          \
    fn->value.fn = fn_;                                                         \
                                                                                \
    scope->vars[count] = (var_t *) gc_add(scope->gc, malloc(sizeof(var_t)));    \
    scope->vars[count]->name = fname;                                           \
    scope->vars[count++]->value = fn;                                           \
    scope->vars[count] = NULL;

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
 * Get standard library functions array
 *
 * @author mishavetl
 *
 * @return (variables array)
 *
 */
int get_stdlib_variables(scope_t *scope);

#endif
