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

/**
 * Collection
 */

#include "std/arithm.c"
#include "std/tuple.c"
#include "std/var.c"
#include "std/io.c"
#include "std/atom.c"
#include "std/number.c"

/**
 * Registration
 */

STDFUNCTIONS(13,
    REGSTDFUNCTION("+",
        create_function(
            plus, INFINITY_ARGS,
            (const int []) {tid_num}, 1,
            scope->gc));
    REGSTDFUNCTION("-",
        create_function(
            minus, INFINITY_ARGS,
            (const int []) {tid_num}, 1,
            scope->gc));

    REGSTDFUNCTION("io:puts",
        create_function(
            io__puts, INFINITY_ARGS,
            (const int []) {tid_string}, 1,
            scope->gc));
    REGSTDFUNCTION("io:putln",
        create_function(
            io__puts, 0,
            (const int []) {}, 0,
            scope->gc));

    REGSTDFUNCTION("number:to_string",
        create_function(
            number__to_string, 1,
            (const int []) {tid_num}, 1,
            scope->gc));

    REGSTDFUNCTION("atom:to_string",
        create_function(
            atom__to_string, 1,
            (const int []) {tid_atom}, 1,
            scope->gc));

    REGSTDFUNCTION("var:atom",
        create_function(
            var__set, 2,
            (const int []) {tid_atom}, 1,
            scope->gc));
    REGSTDFUNCTION("var:number",
        create_function(
            var__set, 2,
            (const int []) {tid_atom, tid_num}, 2,
            scope->gc));
    REGSTDFUNCTION("var:tuple",
        create_function(
            var__set, 2,
            (const int []) {tid_atom, tid_tuple}, 2,
            scope->gc));
    REGSTDFUNCTION("var:string",
        create_function(
            var__set, 2,
            (const int []) {tid_atom, tid_string}, 2,
            scope->gc));

    REGSTDFUNCTION("$",
        create_function(
            var__get, 1,
            (const int []) {tid_atom}, 1,
            scope->gc));

    REGSTDFUNCTION("{}",
        create_function(
            tuple__construct, INFINITY_ARGS,
            (const int []) {}, 0,
            scope->gc));
    REGSTDFUNCTION("#",
        create_function(
            tuple__get, 2,
            (const int []) {tid_tuple, tid_num}, 2,
            scope->gc));
);
