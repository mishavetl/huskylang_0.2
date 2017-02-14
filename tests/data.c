#include <criterion/criterion.h>
#include "data.h"
#include "dbg.h"
#include "memory.h"

TestSuite(data);

Test(data, init_type) {
    struct type type = init_type();
    cr_assert_eq(type.single, __tid_size);
    cr_assert_null(type.multiple);
}

Test(data, types_identical_with_single_types) {
    struct type type1 = init_type(),
                type2 = init_type();
    
    type1.single = tid_integral;
    type2.single = tid_integral;
    cr_assert(types_identical(&type1, &type2));

    type1.single = tid_integral;
    type2.single = tid_real;
    cr_assert_not(types_identical(&type1, &type2));
}

Test(data, types_identical_with_multiple_types) {
    struct type type1 = init_type(),
                type2 = init_type(),
                type3 = init_type(),
                type4 = init_type(),
                type5 = init_type(),
                type6 = init_type(),
                type7 = init_type(),
                type8 = init_type();
    
    type1.single = tid_tuple;
    type6.single = tid_tuple;
    type2.single = tid_integral;
    type3.single = tid_real;
    type4.single = tid_integral;
    type5.single = tid_real;
    struct type *mult1[3] = {&type2, &type3, NULL};
    struct type *mult6[3] = {&type4, &type5, NULL};
    type1.multiple = mult1;
    type6.multiple = mult6;
    cr_assert(types_identical(&type1, &type6));

    type1.single = tid_string;
    type2.single = tid_tuple;
    type3.single = tid_tuple;
    type4.single = tid_list;
    type5.single = tid_list;
    type6.single = tid_string;
    type7.single = tid_real;
    type8.single = tid_real;
    struct type *mult2[3] = {&type1, &type4, NULL};
    struct type *mult3[3] = {&type6, &type5, NULL};
    struct type *mult4[2] = {&type7, NULL};
    struct type *mult5[2] = {&type8, NULL};
    type1.multiple = NULL;
    type2.multiple = mult2;
    type3.multiple = mult3;
    type4.multiple = mult4;
    type5.multiple = mult5;
    type6.multiple = NULL;
    cr_assert(types_identical(&type2, &type3));
}

Test(data, construct_type_basic)
{
    gc_t gc = gc_init();
    struct type *type;
    struct type *multiple[10] = {NULL};
    
    type = construct_type(tid_integral, NULL, &gc);
    cr_assert_eq(type->single, tid_integral);
    cr_assert_null(type->multiple);

    type = construct_type(tid_string, NULL, &gc);
    cr_assert_eq(type->single, tid_string);
    cr_assert_null(type->multiple);

    multiple[0] = construct_type(tid_atom, NULL, &gc);
    type = construct_type(tid_list, multiple, &gc);
    cr_assert_eq(type->single, tid_list);
    cr_assert_eq(type->multiple[0]->single, tid_atom);
    cr_assert_null(type->multiple[0]->multiple);
    cr_assert_null(type->multiple[1]);

    multiple[0] = construct_type(tid_integral, NULL, &gc);
    multiple[1] = construct_type(tid_real, NULL, &gc);
    multiple[2] = construct_type(tid_string, NULL, &gc);
    type = construct_type(tid_tuple, multiple, &gc);
    cr_assert_eq(type->single, tid_tuple);
    cr_assert_eq(type->multiple[0]->single, tid_integral);
    cr_assert_null(type->multiple[0]->multiple);
    cr_assert_eq(type->multiple[1]->single, tid_real);
    cr_assert_null(type->multiple[1]->multiple);
    cr_assert_eq(type->multiple[2]->single, tid_string);
    cr_assert_null(type->multiple[2]->multiple);
    cr_assert_null(type->multiple[3]);

    gc_clean(&gc);
}