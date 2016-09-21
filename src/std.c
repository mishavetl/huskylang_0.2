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
#include "list/src/list.h"
#include "performer.h"

/**
 * Collect
 */

#include "std/arithm.c"
#include "std/tuple.c"
#include "std/list.c"
#include "std/var.c"
#include "std/io.c"
#include "std/atom.c"
#include "std/number.c"
#include "std/saved.c"

/**
 * Register
 */

STDFUNCTIONS(19,
    REGSTDFUNCTION("+",
        create_function(
            plus, INFINITY_ARGS,
            (const unsigned []) {tid_num}, 1,
            scope->gc));
    REGSTDFUNCTION("-",
        create_function(
            minus, INFINITY_ARGS,
            (const unsigned []) {tid_num}, 1,
            scope->gc));

    REGSTDFUNCTION("io:puts",
        create_function(
            io__puts, INFINITY_ARGS,
            (const unsigned []) {tid_string}, 1,
            scope->gc));
    REGSTDFUNCTION("io:putln",
        create_function(
            io__puts, 0,
            (const unsigned []) {}, 0,
            scope->gc));

    REGSTDFUNCTION("number:to_string",
        create_function(
            number__to_string, 1,
            (const unsigned []) {tid_num}, 1,
            scope->gc));

    REGSTDFUNCTION("atom:to_string",
        create_function(
            atom__to_string, 1,
            (const unsigned []) {tid_atom}, 1,
            scope->gc));

    REGSTDFUNCTION("var:atom",
        create_function(
            var__set, 2,
            (const unsigned []) {tid_atom}, 1,
            scope->gc));
    REGSTDFUNCTION("var:number",
        create_function(
            var__set, 2,
            (const unsigned []) {tid_atom, tid_num}, 2,
            scope->gc));
    REGSTDFUNCTION("var:tuple",
        create_function(
            var__set, 2,
            (const unsigned []) {tid_atom, tid_tuple}, 2,
            scope->gc));
    REGSTDFUNCTION("var:string",
        create_function(
            var__set, 2,
            (const unsigned []) {tid_atom, tid_string}, 2,
            scope->gc));
    REGSTDFUNCTION("var:list",
        create_function(
            var__set, 2,
            (const unsigned []) {tid_atom, tid_list}, 2,
            scope->gc));
    REGSTDFUNCTION("var:saved",
        create_function(
            var__set, 2,
            (const unsigned []) {tid_atom, tid_saved}, 2,
            scope->gc));

    REGSTDFUNCTION("$",
        create_function(
            var__get, 1,
            (const unsigned []) {tid_atom}, 1,
            scope->gc));

    REGSTDFUNCTION("{}",
        create_function(
            tuple__construct, INFINITY_ARGS,
            (const unsigned []) {}, 0,
            scope->gc));
    REGSTDFUNCTION("#",
        create_function(
            tuple__get, 2,
            (const unsigned []) {tid_tuple, tid_num}, 2,
            scope->gc));

    REGSTDFUNCTION("[]",
        create_function(
            list__construct, INFINITY_ARGS,
            (const unsigned []) {}, 0,
            scope->gc));
    REGSTDFUNCTION("hd",
        create_function(
            list__head, 1,
            (const unsigned []) {tid_list}, 1,
            scope->gc));
    REGSTDFUNCTION("tl",
        create_function(
            list__tail, 1,
            (const unsigned []) {tid_list}, 1,
            scope->gc));

    REGSTDFUNCTION("saved:call",
        create_function(
            saved__call, 1,
            (const unsigned []) {tid_saved}, 1,
            scope->gc));
);
