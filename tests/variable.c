#include <criterion/criterion.h>
#include <math.h>
#include "variable.h"
#include "data.h"
#include "dbg.h"
#include "memory.h"

TestSuite(variable);

Test(variable, setvar) {
    gc_t gc = gc_init();
    scope_t scope = scope__init();
    scope.gc = &gc;

    data_t data;
    data.type = construct_type(tid_atom, NULL, &gc);
    data.value.atom = "foo";

    setvar(&scope, "foo", &data);

    cr_assert(types_identical(scope.vars[0]->value->type, data.type));
    cr_assert_str_eq(scope.vars[0]->value->value.atom, data.value.atom);
    cr_assert_eq(scope.vsize, 1);

    data_t data1;
    data1.type = construct_type(tid_real, NULL, &gc);
    data1.value.real = .0;

    setvar(&scope, "bar", &data1);

    cr_assert_str_eq(scope.vars[0]->name, "foo");
    cr_assert(types_identical(scope.vars[0]->value->type, data.type));
    cr_assert_str_eq(scope.vars[0]->value->value.atom, data.value.atom);
    cr_assert_str_eq(scope.vars[1]->name, "bar");
    cr_assert(types_identical(scope.vars[1]->value->type, data1.type));
    cr_assert_eq(scope.vars[1]->value->value.real, data1.value.real);
    cr_assert_eq(scope.vsize, 2);    

    gc_clean(&gc);
}

Test(variable, getvar) {
    gc_t gc = gc_init();
    scope_t scope = scope__init();
    scope.gc = &gc;

    data_t data;
    data.type = construct_type(tid_atom, NULL, &gc);
    data.value.atom = "foo";

    data_t data1;
    data1.type = construct_type(tid_real, NULL, &gc);
    data1.value.real = .0;

    setvar(&scope, "foo", &data);
    setvar(&scope, "bar", &data1);

    const var_t *var;

    cr_assert_eq(getvar(&scope, "foo", &var), 1);
    cr_assert_str_eq(var->value->value.atom, data.value.atom);
    cr_assert_eq(getvar(&scope, "bar", &var), 0);
    cr_assert_eq(var->value->value.real, data1.value.real);

    cr_assert_eq(getvar_start(&scope, 1, "bar", &var), 2);
    cr_assert_null(var);

    gc_clean(&gc);
}

Test(variable, getvar_with_scope_parent) {
    gc_t gc = gc_init();
    scope_t scope = scope__init();
    scope_t scope_p = scope__init();
    scope.gc = &gc;
    scope_p.gc = &gc;
    scope.parent = &scope_p;

    data_t data;
    data.type = construct_type(tid_atom, NULL, &gc);
    data.value.atom = "foo";

    data_t data1;
    data1.type = construct_type(tid_real, NULL, &gc);
    data1.value.real = .1;

    data_t data2;
    data2.type = construct_type(tid_integral, NULL, &gc);
    data2.value.integral = 1;

    data_t data3;
    data3.type = construct_type(tid_string, NULL, &gc);
    data3.value.string = "str";

    setvar(&scope, "foo", &data);
    setvar(&scope, "bar", &data1);

    setvar(&scope_p, "baz", &data2);
    setvar(&scope_p, "buzz", &data3);

    const var_t *var;

    cr_assert_eq(getvar(&scope, "foo", &var), 1);
    cr_assert_str_eq(var->value->value.atom, data.value.atom);
    cr_assert_eq(getvar(&scope, "bar", &var), 0);
    cr_assert_eq(var->value->value.real, data1.value.real);

    cr_assert_eq(getvar_start(&scope, 1, "bar", &var), 4);
    cr_assert_null(var);

    cr_assert_eq(getvar(&scope, "baz", &var), 3);
    cr_assert_eq(var->value->value.integral, data2.value.integral);
    cr_assert_eq(getvar_start(&scope, 2, "foo", &var), 4);
    cr_assert_null(var);

    cr_assert_eq(getvar_start(&scope, 2, "baz", &var), 3);
    cr_assert_eq(var->value->value.integral, data2.value.integral);
    
    gc_clean(&gc);
}