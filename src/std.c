#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "type.h"
#include "std.h"
#include "variable.h"
#include "memory.h"
#include "dbg.h"
#include "function.h"
#include "huserr.h"

extern gc_t *gc_global;

int plus(type_t **args, argc_t argc, type_t *ret, scope_t *scope)
{
    size_t i;

    ret->type = tid_num;
    ret->value.num = 0;

    for (i = 0; i < argc; i++) {
        ret->value.num += args[i]->value.num;
    }

    return 0;

    error:

    return -1;
}

int number__to_string(type_t **args, argc_t argc, type_t *ret, scope_t *scope)
{
    char *string = gc_add(scope->gc, malloc(sizeof(char) * (log10(args[0]->value.num) + 2)));

    check_mem(string);

    sprintf(string, "%d", args[0]->value.num);

    ret->type = tid_string;
    ret->value.string = string;

    return 0;

    error:

    return -1;
}

int atom__to_string(type_t **args, argc_t argc, type_t *ret, scope_t *scope)
{
    ret->type = tid_string;
    ret->value.string = gc_add(scope->gc, strdup(args[0]->value.atom));
    check_mem(ret->value.string);

    return 0;

    error:

    return -1;
}

// int minus(type_t **args, argc_t argc, type_t *ret, scope_t *scope)
// {
//     size_t i;

//     ret->type = tid_num;

//     check(args[0]->type == tid_num);
//     ret->value.num = args[0]->value.num;

//     for (i = 1; args[i]; i++) {
//         check(args[i]->type == tid_num);
//         ret->value.num -= args[i]->value.num;
//     }

//     error:

//     ret->type = atom;
//     ret->value.atom = "bad";
// }

int io__puts(type_t **args, argc_t argc, type_t *ret, scope_t *scope)
{
    size_t i;

    ret->type = tid_atom;

    for (i = 0; args[i]; i++) {
        printf("%s", args[i]->value.atom);
    }

    puts("");

    ret->value.atom = "good";

    return 0;
}

// int lister(type_t **args, argc_t argc, type_t *ret, scope_t *scope)
// {
//     ret->type = tid_atom;
//     ret->value.atom = "good";
// }

int set(type_t **args, argc_t argc, type_t *ret, scope_t *scope)
{
    ret->type = tid_atom;

    /* Check that variable does not exist. */
    // check(getvar(scope, args[0]->value.atom) < 0, "");

    /* Set variable. */
    check(setvar(scope, args[0]->value.atom, args[1]) >= 0, "");

    ret->value.atom = "good";

    return 0;

    error:

    return -1;
}

int get(type_t **args, argc_t argc, type_t *ret, scope_t *scope)
{
    int i;

    /* Check that variable does not exist. */
    if ((i = getvar(scope, args[0]->value.atom)) <= 0) {
        scope->error = gc_add(scope->gc, malloc(sizeof(huserr_t)));
        scope->error->name = "typeErr";
        scope->error->msg = "undefined variable";
        goto error;
    }

    ret->type = scope->vars[i]->value->type;
    ret->value = scope->vars[i]->value->value;

    return 0;

    error:

    return -1;
}

STDFUNCTIONS(7,
    REGSTDFUNCTION("+",
        create_function(
            plus, INFINITY_ARGS,
            (const int []) {tid_num}, 1,
            scope->gc))
    REGSTDFUNCTION("io:puts",
        create_function(
            io__puts, INFINITY_ARGS,
            (const int []) {tid_string}, 1,
            scope->gc))
    REGSTDFUNCTION("number:to_string",
        create_function(
            number__to_string, 1,
            (const int []) {tid_num}, 1,
            scope->gc))
    REGSTDFUNCTION("atom:to_string",
        create_function(
            atom__to_string, 1,
            (const int []) {tid_atom}, 1,
            scope->gc))
    REGSTDFUNCTION("var:atom",
        create_function(
            set, 2,
            (const int []) {tid_atom}, 1,
            scope->gc))
    REGSTDFUNCTION("var:number",
        create_function(
            set, 2,
            (const int []) {tid_atom, tid_num}, 2,
            scope->gc))
    REGSTDFUNCTION("$",
        create_function(
            get, 1,
            (const int []) {tid_atom}, 1,
            scope->gc))
)
