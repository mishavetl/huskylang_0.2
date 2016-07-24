#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "type.h"
#include "std.h"
#include "variable.h"
#include "memory.h"
#include "dbg.h"
#include "function.h"

extern gc_t *gc_global;

void plus(type_t **args, argc_t argc, type_t *ret, scope_t *scope)
{
    size_t i;

    ret->type = tid_num;
    ret->value.num = 0;

    for (i = 0; i < argc; i++) {
        check(args[i]->type == tid_num, "%d", args[i]->type);
        ret->value.num += args[i]->value.num;
    }

    return;

    error:

    ret->type = tid_atom;
    ret->value.atom = "bad";
}

void number__to_string(type_t **args, argc_t argc, type_t *ret, scope_t *scope)
{
    char *string = gc_add(gc_global, malloc(sizeof(char) * (log10(args[0]->value.num) + 2)));

    check_mem(string);

    sprintf(string, "%d", args[0]->value.num);

    ret->type = tid_string;
    ret->value.string = string;


    return;

    error:

    ret->type = tid_atom;
    ret->value.atom = "bad";
}

void atom__to_string(type_t **args, argc_t argc, type_t *ret, scope_t *scope)
{
    ret->type = tid_string;
    ret->value.string = gc_add(gc_global, strdup(args[0]->value.atom));
    check_mem(ret->value.string);

    return;

    error:

    ret->type = tid_atom;
    ret->value.atom = "bad";
}

// void minus(type_t **args, argc_t argc, type_t *ret, scope_t *scope)
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

void io__puts(type_t **args, argc_t argc, type_t *ret, scope_t *scope)
{
    size_t i;

    ret->type = tid_atom;

    for (i = 0; args[i]; i++) {
        printf("%s", args[i]->value.atom);
    }

    puts("");

    ret->value.atom = "good";

    return;
}

// void lister(type_t **args, argc_t argc, type_t *ret, scope_t *scope)
// {
//     ret->type = tid_atom;
//     ret->value.atom = "good";
// }

// void set(type_t **args, argc_t argc, type_t *ret, scope_t *scope)
// {
//     ret->type = tid_atom;

//     check(argc == 2, "Bad argc.");
//     check(args[0]->type == tid_atom, "Invalid variable identifier");

//     setvar(scope, args[0]->value.atom, args[1]);
//     ret->value.atom = "good";

// error:

//     ret->value.atom = "bad";
// }

STDFUNCTIONS(4,
    REGSTDFUNCTION("+",
        create_function(
            plus, INFINITY_ARGS,
            (const int []) {tid_num}, 1,
            gc_global))
    REGSTDFUNCTION("io:puts",
        create_function(
            io__puts, INFINITY_ARGS,
            (const int []) {tid_string}, 1,
            gc_global))
    REGSTDFUNCTION("number:to_string",
        create_function(
            number__to_string, 1,
            (const int []) {tid_num}, 1,
            gc_global))
    REGSTDFUNCTION("atom:to_string",
        create_function(
            atom__to_string, 1,
            (const int []) {tid_atom}, 1,
            gc_global))
)
