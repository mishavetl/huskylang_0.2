#include <stdlib.h>
#include "std.h"
#include "variable.h"
#include "memory.h"
#include "dbg.h"

extern gc_t *gc_global;

int plus(type_t **args, type_t *ret)
{
    size_t i;

    check_mem(args);

    ret->type = tid_num;
    ret->value.num = 0;

    for (i = 0; args[i]; i++) {
        check(args[i]->type == tid_num, "Invalid argument type.");
        ret->value.num += args[i]->value.num;
    }

    return 0;

    error:

    return -1;
}

int minus(type_t **args, type_t *ret)
{
    size_t i;

    check_mem(args);

    ret->type = tid_num;

    check(args[0]->type == tid_num, "Invalid argument type.");
    ret->value.num = args[0]->value.num;

    for (i = 1; args[i]; i++) {
        check(args[i]->type == tid_num, "Invalid argument type.");
        ret->value.num -= args[i]->value.num;
    }

    return 0;

    error:

    return -1;
}

int io__puts(type_t **args, type_t *ret)
{
    size_t i;

    check_mem(args);

    for (i = 0; args[i]; i++) {
        switch (args[i]->type) {
            case tid_num:
                printf("%d", args[i]->value.num);
                break;

            case tid_atom:
                printf("%s", args[i]->value.atom);
                break;

            default:
                sentinel("system error unsupported argument type '%d'", args[i]->type);
        }
    }

    puts("");

    ret->type = tid_atom;
    ret->value.atom = "good";

    return 0;

    error:

    return -1;
}

int lister(type_t **args, type_t *ret)
{
    check_mem(args);

    ret->type = tid_atom;
    ret->value.atom = "good";

    return 0;

    error:

    return -1;
}

var_t **get_stdlib_variables()
{
    var_t **vars = gc__add(gc_global, malloc(sizeof(var_t *) * 5));
    type_t *fn_plus = gc__add(gc_global, malloc(sizeof(type_t)));
    type_t *fn_minus = gc__add(gc_global, malloc(sizeof(type_t)));
    type_t *fn_io__puts = gc__add(gc_global, malloc(sizeof(type_t)));
    type_t *fn_lister = gc__add(gc_global, malloc(sizeof(type_t)));

    fn_plus->type = tid_fn;
    fn_plus->value.fn = plus;

    vars[0] = gc__add(gc_global, malloc(sizeof(var_t)));
    vars[0]->name = "+";
    vars[0]->value = fn_plus;

    fn_minus->type = tid_fn;
    fn_minus->value.fn = minus;

    vars[1] = gc__add(gc_global, malloc(sizeof(var_t)));
    vars[1]->name = "-";
    vars[1]->value = fn_minus;

    fn_io__puts->type = tid_fn;
    fn_io__puts->value.fn = io__puts;

    vars[2] = gc__add(gc_global, malloc(sizeof(var_t)));
    vars[2]->name = "io:puts";
    vars[2]->value = fn_io__puts;

    fn_lister->type = tid_fn;
    fn_lister->value.fn = lister;

    vars[3] = gc__add(gc_global, malloc(sizeof(var_t)));
    vars[3]->name = "'";
    vars[3]->value = fn_lister;

    vars[4] = NULL;

    return vars;
}
