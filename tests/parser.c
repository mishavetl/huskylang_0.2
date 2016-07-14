#include <criterion/criterion.h>
#include "tokenizer.h"
#include "parser.h"
#include "memory.h"
#include "dbg.h"

Test(parser, funcall__typical_function_call) {
    call_tree_t tree;

    static token_t funname = {.value = "+", .type = tok_atom};
    static token_t arg1 = {.value = "1", .type = tok_num};
    static token_t arg2 = {.value = "2", .type = tok_num};
    static token_t del = {.value = ",", .type = tok_del};

    token_t *tokens[] = {
        &funname,
        &arg1,
        &del,
        &arg2,
        NULL
    };

    cr_assert_geq(parser__funcall(&tree, tokens), 0, "Status.");
    cr_assert_arr_eq(tree.tokens, tokens, sizeof(tokens), "Tree tokens and passed tokens equality.");

    cr_assert_eq(tree.map[0][0], EMPTY_MAPV);
    cr_assert_eq(tree.map[0][1], 1);
    cr_assert_eq(tree.map[0][2], 3);
    cr_assert_eq(tree.map[0][3], TERMINATE_MAPV);
    cr_assert_null(tree.map[1]);
    cr_assert_null(tree.map[2]);
    cr_assert_null(tree.map[3]);

    FREE(tree.map[0]);
    FREE(tree.map);
}

Test(parser, funcall__with_module) {
    call_tree_t tree;

    static token_t fname = {.value = "foo:bar", .type = tok_atom};
    static token_t arg1 = {.value = "1", .type = tok_num};
    static token_t arg2 = {.value = "2", .type = tok_num};
    static token_t del = {.value = ",", .type = tok_del};

    token_t *tokens[] = {
        &fname,
        &arg1,
        &del,
        &arg2,
        NULL
    };

    cr_assert_geq(parser__funcall(&tree, tokens), 0, "Status.");
    cr_assert_arr_eq(tree.tokens, tokens, sizeof(tokens), "Tree tokens and passed tokens equality.");


    cr_assert_eq(tree.map[0][0], EMPTY_MAPV);
    cr_assert_eq(tree.map[0][1], 1);
    cr_assert_eq(tree.map[0][2], 3);
    cr_assert_eq(tree.map[0][3], TERMINATE_MAPV);
    cr_assert_null(tree.map[1]);
    cr_assert_null(tree.map[2]);
    cr_assert_null(tree.map[3]);

    FREE(tree.map[0]);
    FREE(tree.map);
}

Test(parser, funcall__only_one_arg) {
    call_tree_t tree;

    static token_t funname = {.value = "foo", .type = tok_atom};
    static token_t arg1 = {.value = "1", .type = tok_num};

    token_t *tokens[] = {
        &funname,
        &arg1,
        NULL
    };

    cr_assert_geq(parser__funcall(&tree, tokens), 0, "Status.");
    cr_assert_arr_eq(tree.tokens, tokens, sizeof(tokens), "Tree tokens and passed tokens equality.");

    cr_assert_eq(tree.map[0][0], EMPTY_MAPV);
    cr_assert_eq(tree.map[0][1], 1);
    cr_assert_eq(tree.map[0][2], TERMINATE_MAPV);
    cr_assert_null(tree.map[1]);

    FREE(tree.map[0]);
    FREE(tree.map);
}

Test(parser, funcall__with_fname_located_between_two_args) {
    call_tree_t tree;

    static token_t funname = {.value = "-", .type = tok_atom};
    static token_t arg1 = {.value = "1", .type = tok_num};
    static token_t arg2 = {.value = "2", .type = tok_num};

    token_t *tokens[] = {
        &arg1,
        &funname,
        &arg2,
        NULL
    };

    cr_assert_geq(parser__funcall(&tree, tokens), 0, "Status.");
    cr_assert_arr_eq(tree.tokens, tokens, sizeof(tokens), "Tree tokens and passed tokens equality.");

    cr_assert_null(tree.map[0]);
    cr_assert_eq(tree.map[1][0], 0);
    cr_assert_eq(tree.map[1][1], 2);
    cr_assert_eq(tree.map[1][2], TERMINATE_MAPV);
    cr_assert_null(tree.map[2]);

    FREE(tree.map[1]);
    FREE(tree.map);
}

Test(parser, funcall__with_fname_located_between_multiple_args_after_first) {
    call_tree_t tree;

    static token_t funname = {.value = "-", .type = tok_atom};
    static token_t arg1 = {.value = "1", .type = tok_num};
    static token_t arg2 = {.value = "2", .type = tok_num};
    static token_t arg3 = {.value = "3", .type = tok_num};
    static token_t arg4 = {.value = "4", .type = tok_num};
    static token_t arg5 = {.value = "5", .type = tok_num};
    static token_t del = {.value = ",", .type = tok_del};

    token_t *tokens[] = {
        &arg1,
        &funname,
        &arg2,
        &del,
        &arg3,
        &del,
        &arg4,
        &del,
        &arg5,
        NULL
    };

    cr_assert_geq(parser__funcall(&tree, tokens), 0, "Status.");
    cr_assert_arr_eq(tree.tokens, tokens, sizeof(tokens), "Tree tokens and passed tokens equality.");

    cr_assert_null(tree.map[0]);
    cr_assert_eq(tree.map[1][0], 0);
    cr_assert_eq(tree.map[1][1], 2);
    cr_assert_eq(tree.map[1][2], 4);
    cr_assert_eq(tree.map[1][3], 6);
    cr_assert_eq(tree.map[1][4], 8);
    cr_assert_eq(tree.map[1][5], TERMINATE_MAPV);
    cr_assert_null(tree.map[2]);
    cr_assert_null(tree.map[3]);
    cr_assert_null(tree.map[4]);
    cr_assert_null(tree.map[5]);
    cr_assert_null(tree.map[6]);
    cr_assert_null(tree.map[7]);
    cr_assert_null(tree.map[8]);

    FREE(tree.map[1]);
    FREE(tree.map);
}

Test(parser, funcall__with_fname_located_between_multiple_args_after_second) {
    call_tree_t tree;

    static token_t funname = {.value = "-", .type = tok_atom};
    static token_t arg1 = {.value = "1", .type = tok_num};
    static token_t arg2 = {.value = "2", .type = tok_num};
    static token_t arg3 = {.value = "3", .type = tok_num};
    static token_t arg4 = {.value = "4", .type = tok_num};
    static token_t arg5 = {.value = "5", .type = tok_num};
    static token_t del = {.value = ",", .type = tok_del};

    token_t *tokens[] = {
        &arg1,
        &del,
        &arg2,
        &funname,
        &arg3,
        &del,
        &arg4,
        &del,
        &arg5,
        NULL
    };

    cr_assert_geq(parser__funcall(&tree, tokens), 0, "Status.");
    cr_assert_arr_eq(tree.tokens, tokens, sizeof(tokens), "Tree tokens and passed tokens equality.");

    cr_assert_null(tree.map[0]);
    cr_assert_null(tree.map[1]);
    cr_assert_null(tree.map[2]);
    cr_assert_eq(tree.map[3][0], 0);
    cr_assert_eq(tree.map[3][1], 2);
    cr_assert_eq(tree.map[3][2], 4);
    cr_assert_eq(tree.map[3][3], 6);
    cr_assert_eq(tree.map[3][4], 8);
    cr_assert_eq(tree.map[3][5], TERMINATE_MAPV);
    cr_assert_null(tree.map[4]);
    cr_assert_null(tree.map[5]);
    cr_assert_null(tree.map[6]);
    cr_assert_null(tree.map[7]);
    cr_assert_null(tree.map[8]);

    FREE(tree.map[3]);
    FREE(tree.map);
}

Test(parser, funcall__with_fname_located_between_multiple_args_after_third) {
    call_tree_t tree;

    static token_t funname = {.value = "-", .type = tok_atom};
    static token_t arg1 = {.value = "1", .type = tok_num};
    static token_t arg2 = {.value = "2", .type = tok_num};
    static token_t arg3 = {.value = "3", .type = tok_num};
    static token_t arg4 = {.value = "4", .type = tok_num};
    static token_t arg5 = {.value = "5", .type = tok_num};
    static token_t del = {.value = ",", .type = tok_del};

    token_t *tokens[] = {
        &arg1,
        &del,
        &arg2,
        &del,
        &arg3,
        &funname,
        &arg4,
        &del,
        &arg5,
        NULL
    };

    cr_assert_geq(parser__funcall(&tree, tokens), 0, "Status.");
    cr_assert_arr_eq(tree.tokens, tokens, sizeof(tokens), "Tree tokens and passed tokens equality.");

    cr_assert_null(tree.map[0]);
    cr_assert_null(tree.map[1]);
    cr_assert_null(tree.map[2]);
    cr_assert_null(tree.map[3]);
    cr_assert_null(tree.map[4]);
    cr_assert_eq(tree.map[5][0], 0);
    cr_assert_eq(tree.map[5][1], 2);
    cr_assert_eq(tree.map[5][2], 4);
    cr_assert_eq(tree.map[5][3], 6);
    cr_assert_eq(tree.map[5][4], 8);
    cr_assert_eq(tree.map[5][5], TERMINATE_MAPV);
    cr_assert_null(tree.map[6]);
    cr_assert_null(tree.map[7]);
    cr_assert_null(tree.map[8]);

    FREE(tree.map[5]);
    FREE(tree.map);
}

Test(parser, funcall__with_fname_located_between_multiple_args_before_last) {
    call_tree_t tree;

    static token_t funname = {.value = "-", .type = tok_atom};
    static token_t arg1 = {.value = "1", .type = tok_num};
    static token_t arg2 = {.value = "2", .type = tok_num};
    static token_t arg3 = {.value = "3", .type = tok_num};
    static token_t arg4 = {.value = "4", .type = tok_num};
    static token_t del = {.value = ",", .type = tok_del};

    token_t *tokens[] = {
        &arg1,
        &del,
        &arg2,
        &del,
        &arg3,
        &funname,
        &arg4,
        NULL
    };

    cr_assert_geq(parser__funcall(&tree, tokens), 0, "Status.");
    cr_assert_arr_eq(tree.tokens, tokens, sizeof(tokens), "Tree tokens and passed tokens equality.");

    cr_assert_null(tree.map[0]);
    cr_assert_null(tree.map[1]);
    cr_assert_null(tree.map[2]);
    cr_assert_null(tree.map[3]);
    cr_assert_null(tree.map[4]);
    cr_assert_eq(tree.map[5][0], 0);
    cr_assert_eq(tree.map[5][1], 2);
    cr_assert_eq(tree.map[5][2], 4);
    cr_assert_eq(tree.map[5][3], 6);
    cr_assert_eq(tree.map[5][4], TERMINATE_MAPV);
    cr_assert_null(tree.map[6]);

    FREE(tree.map[5]);
    FREE(tree.map);
}

Test(parser, funcall__with_nested_function_calls) {
    call_tree_t tree;

    static token_t funname = {.value = "-", .type = tok_atom};
    static token_t funname1 = {.value = "+", .type = tok_atom};
    static token_t funname2 = {.value = "*", .type = tok_atom};
    static token_t prior_start = {.value = "(", .type = tok_prior_start};
    static token_t prior_end = {.value = ")", .type = tok_prior_end};
    static token_t arg1 = {.value = "1", .type = tok_num};
    static token_t arg2 = {.value = "2", .type = tok_num};
    static token_t arg3 = {.value = "3", .type = tok_num};
    static token_t arg4 = {.value = "4", .type = tok_num};
    static token_t del = {.value = ",", .type = tok_del};

    token_t *tokens[] = {
        &funname,
        &arg1,
        &del,
        &prior_start,
        &prior_start,
        &arg2,
        &funname2,
        &arg3,
        &prior_end,
        &funname1,
        &arg4,
        &prior_end,
        NULL
    };

    cr_assert_geq(parser__funcall(&tree, tokens), 0, "Status.");
    cr_assert_arr_eq(tree.tokens, tokens, sizeof(tokens), "Tree tokens and passed tokens equality.");

    cr_assert_eq(tree.map[0][0], EMPTY_MAPV);
    cr_assert_eq(tree.map[0][1], 1);
    cr_assert_eq(tree.map[0][2], 9);
    cr_assert_eq(tree.map[0][3], TERMINATE_MAPV);
    cr_assert_null(tree.map[1]);
    cr_assert_null(tree.map[2]);
    cr_assert_null(tree.map[3]);
    cr_assert_null(tree.map[4]);
    cr_assert_null(tree.map[5]);
    cr_assert_eq(tree.map[6][0], 5);
    cr_assert_eq(tree.map[6][1], 7);
    cr_assert_eq(tree.map[6][2], TERMINATE_MAPV);
    cr_assert_null(tree.map[7]);
    cr_assert_null(tree.map[8]);
    cr_assert_eq(tree.map[9][0], 6);
    cr_assert_eq(tree.map[9][1], EMPTY_MAPV);
    cr_assert_eq(tree.map[9][2], 10);
    cr_assert_eq(tree.map[9][3], TERMINATE_MAPV);
    cr_assert_null(tree.map[10]);
    cr_assert_null(tree.map[11]);

    FREE(tree.map[0]);
    FREE(tree.map[6]);
    FREE(tree.map[9]);
    FREE(tree.map);
}

Test(parser, funcall__with_multiple_nested_function_calls_in_one) {
    call_tree_t tree;

    static token_t funname = {.value = "-", .type = tok_atom};
    static token_t funname1 = {.value = "+", .type = tok_atom};
    static token_t funname2 = {.value = "*", .type = tok_atom};
    static token_t prior_start = {.value = "(", .type = tok_prior_start};
    static token_t prior_end = {.value = ")", .type = tok_prior_end};
    static token_t arg1 = {.value = "1", .type = tok_num};
    static token_t arg2 = {.value = "2", .type = tok_num};
    static token_t arg3 = {.value = "3", .type = tok_num};
    static token_t arg4 = {.value = "4", .type = tok_num};
    static token_t del = {.value = ",", .type = tok_del};

    token_t *tokens[] = {
        &arg1,
        &funname,
        &prior_start,
        &arg2,
        &funname2,
        &arg3,
        &prior_end,
        &del,
        &prior_start,
        &funname1,
        &arg4,
        &prior_end,
        NULL
    };

    cr_assert_geq(parser__funcall(&tree, tokens), 0, "Status.");
    cr_assert_arr_eq(tree.tokens, tokens, sizeof(tokens), "Tree tokens and passed tokens equality.");

    cr_assert_null(tree.map[0]);
    cr_assert_eq(tree.map[1][0], 0);
    cr_assert_eq(tree.map[1][1], 4);
    cr_assert_eq(tree.map[1][2], 9);
    cr_assert_eq(tree.map[1][3], TERMINATE_MAPV);
    cr_assert_null(tree.map[2]);
    cr_assert_null(tree.map[3]);
    cr_assert_eq(tree.map[4][0], 3);
    cr_assert_eq(tree.map[4][1], 5);
    cr_assert_eq(tree.map[4][2], TERMINATE_MAPV);
    cr_assert_null(tree.map[5]);
    cr_assert_null(tree.map[6]);
    cr_assert_null(tree.map[7]);
    cr_assert_null(tree.map[8]);
    cr_assert_eq(tree.map[9][0], EMPTY_MAPV);
    cr_assert_eq(tree.map[9][1], 10);
    cr_assert_eq(tree.map[9][2], TERMINATE_MAPV);
    cr_assert_null(tree.map[10]);
    cr_assert_null(tree.map[11]);

    FREE(tree.map[1]);
    FREE(tree.map[4]);
    FREE(tree.map[9]);
    FREE(tree.map);
}
