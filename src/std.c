#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#include "data.h"
#include "std.h"
#include "variable.h"
#include "memory.h"
#include "dbg.h"
#include "function.h"
#include "huserr.h"
#include "list/src/list.h"
#include "performer.h"
#include "huserr.h"

/**
 * Collect
 */

// #include "std/arithm.c"
// #include "std/tuple.c"
// #include "std/list.c"
// #include "std/var.c"
#include "std/io.c"
// #include "std/atom.c"
// #include "std/number.c"
// #include "std/saved.c"
// #include "std/function.c"
// #include "std/error.c"
// #include "std/type.c"
// #include "std/boolean.c"
#include "std/string.c"
#include "std/integral.c"

/**
 * Register
 */

STDFUNCTIONS(5,
    REGSTDFUNCTION("+",
        itta(stt{
            construct_type(tid_integral, NULL, gc),
            construct_type(tid_integral, NULL, gc),
            construct_type(tid_integral, NULL, gc)
        }, 3, gc),
        create_function(integral__plus, NULL, 2, NULL, 0, gc));
    REGSTDFUNCTION("+",
        itta(stt{
            construct_type(tid_string, NULL, gc),
            construct_type(tid_string, NULL, gc),
            construct_type(tid_string, NULL, gc)
        }, 3, gc),
        create_function(string__add, NULL, 2, NULL, 0, gc));
    // REGSTDFUNCTION("-",
    //     create_function(
    //         minus, NULL, INFINITY_ARGS,
    //         (const unsigned []) {tid_integral}, 1,
    //         NULL, 0,
    //         scope->gc));
    // REGSTDFUNCTION("*",
    //     create_function(
    //         multiply, NULL, INFINITY_ARGS,
    //         (const unsigned []) {tid_integral}, 1,
    //         NULL, 0,
    //         scope->gc));

    // REGSTDFUNCTION("io:gets",
    //     create_function(
    //         io__gets, NULL, 0,
    //         (const unsigned []) {}, 0,
    //         NULL, 0,
    //         scope->gc));
    REGSTDFUNCTION("io:puts",
        itta(stt{
            construct_type(tid_atom, NULL, gc),
            construct_type(tid_string, NULL, gc)
        }, 2, gc),
        create_function(io__puts, NULL, 1, NULL, 0, gc));
    // REGSTDFUNCTION("io:putln",
    //     create_function(
    //         io__puts, NULL, 0,
    //         (const unsigned []) {}, 0,
    //         NULL, 0,
    //         scope->gc));

    REGSTDFUNCTION("to-string",
        itta(stt{
            construct_type(tid_string, NULL, gc),
            construct_type(tid_integral, NULL, gc)
        }, 2, gc),
        create_function(integral__to_string, NULL, 1, NULL, 0, gc));
    REGSTDFUNCTION("to-integral",
        itta(stt{
            construct_type(tid_integral, NULL, gc),
            construct_type(tid_string, NULL, gc)
        }, 2, gc),
        create_function(string__to_integral, NULL, 1, NULL, 0, gc));

    // REGSTDFUNCTION("number:to_string",
    //     create_function(
    //         number__to_string, NULL, 1,
    //         (const unsigned []) {tid_integral}, 1,
    //         NULL, 0,
    //         scope->gc));

    // REGSTDFUNCTION("atom:to_string",
    //     create_function(
    //         atom__to_string, NULL, 1,
    //         (const unsigned []) {tid_atom}, 1,
    //         NULL, 0,
    //         scope->gc));

    // /* REGSTDFUNCTION("var:atom", */
    // /*     create_function( */
    // /*         var__set, NULL, 2, */
    // /*         (const unsigned []) {tid_atom}, 1, */
    // /*         NULL, 0, */
    // /*         scope->gc)); */
    // /* REGSTDFUNCTION("var:number", */
    // /*     create_function( */
    // /*         var__set, NULL, 2, */
    // /*         (const unsigned []) {tid_atom, tid_integral}, 2, */
    // /*         NULL, 0, */
    // /*         scope->gc)); */
    // /* REGSTDFUNCTION("var:tuple", */
    // /*     create_function( */
    // /*         var__set, NULL, 2, */
    // /*         (const unsigned []) {tid_atom, tid_tuple}, 2, */
    // /*         NULL, 0, */
    // /*         scope->gc)); */
    // /* REGSTDFUNCTION("var:string", */
    // /*     create_function( */
    // /*         var__set, NULL, 2, */
    // /*         (const unsigned []) {tid_atom, tid_string}, 2, */
    // /*         NULL, 0, */
    // /*         scope->gc)); */
    // /* REGSTDFUNCTION("var:list", */
    // /*     create_function( */
    // /*         var__set, NULL, 2, */
    // /*         (const unsigned []) {tid_atom, tid_list}, 2, */
    // /*         NULL, 0, */
    // /*         scope->gc)); */
    // /* REGSTDFUNCTION("var:saved", */
    // /*     create_function( */
    // /*         var__set, NULL, 2, */
    // /*         (const unsigned []) {tid_atom, tid_saved}, 2, */
    // /*         NULL, 0, */
    // /*         scope->gc)); */
    // REGSTDFUNCTION("def",
    //     create_function(
    //         var__set, NULL, 2,
    //         (const unsigned []) {tid_atom}, 1,
    //         NULL, 0,
    //         scope->gc));

    // REGSTDFUNCTION("fn",
    //     create_function(
    //         function__define, NULL, 3,
    //         (const unsigned []) {tid_atom, tid_list, tid_saved}, 3,
    //         NULL, 0,
    //         scope->gc));

    // REGSTDFUNCTION("$",
    //     create_function(
    //         var__get, NULL, 1,
    //         (const unsigned []) {tid_atom}, 1,
    //         NULL, 0,
    //         scope->gc));

    // REGSTDFUNCTION("{}",
    //     create_function(
    //         tuple__construct, NULL, INFINITY_ARGS,
    //         (const unsigned []) {}, 0,
    //         NULL, 0,
    //         scope->gc));
    // REGSTDFUNCTION("#",
    //     create_function(
    //         tuple__get, NULL, 2,
    //         (const unsigned []) {tid_tuple, tid_integral}, 2,
    //         NULL, 0,
    //         scope->gc));

    // REGSTDFUNCTION("[]",
    //     create_function(
    //         list__construct, NULL, INFINITY_ARGS,
    //         (const unsigned []) {}, 0,
    //         NULL, 0,
    //         scope->gc));
    // REGSTDFUNCTION("hd",
    //     create_function(
    //         list__head, NULL, 1,
    //         (const unsigned []) {tid_list}, 1,
    //         NULL, 0,
    //         scope->gc));
    // REGSTDFUNCTION("tl",
    //     create_function(
    //         list__tail, NULL, 1,
    //         (const unsigned []) {tid_list}, 1,
    //         NULL, 0,
    //         scope->gc));
    // REGSTDFUNCTION("list:unzip",
    //     create_function(
    //         list__unzip, NULL, 1,
    //         (const unsigned []) {tid_list}, 1,
    //         NULL, 0,
    //         scope->gc));

    // REGSTDFUNCTION("saved:call",
    //     create_function(
    //         saved__call, NULL, 1,
    //         (const unsigned []) {tid_saved}, 1,
    //         NULL, 0,
    //         scope->gc));

    // REGSTDFUNCTION("error:except_string",
    //     create_function(
    //         error__except_string, NULL, 1,
    //         (const unsigned []) {tid_saved}, 1,
    //         NULL, 0,
    //         scope->gc));

    // REGSTDFUNCTION("&",
    //     create_function(
    //         function__create, NULL, 2,
    //         (const unsigned []) {tid_list, tid_saved}, 2,
    //         NULL, 0,
    //         scope->gc));
    // REGSTDFUNCTION("r",
    //     create_function(
    //         function__return, NULL, 1,
    //         (const unsigned []) {}, 0,
    //         NULL, 0,
    //         scope->gc));
    
    // REGSTDFUNCTION("type",
    //     create_function(
    //         typename, NULL, 1,
    //         (const unsigned []) {}, 0,
    //         NULL, 0,
    //         scope->gc));

    // REGSTDFUNCTION("->",
    //     create_function(
    //         boolean__if, NULL, 2,
    //         (const unsigned []) {tid_atom, tid_saved}, 2,
    //         NULL, 0,
    //         scope->gc));
    // REGSTDFUNCTION("|->",
    //     create_function(
    //         boolean__continuous_if, NULL, 3,
    //         (const unsigned []) {tid_atom, tid_saved, tid_saved}, 3,
    //         NULL, 0,
    //         scope->gc));

    // REGSTDFUNCTION("=",
    //     create_function(
    //         number__equal, NULL, 2,
    //         (const unsigned []) {tid_integral, tid_integral}, 2,
    //         NULL, 0,
    //         scope->gc));
)
