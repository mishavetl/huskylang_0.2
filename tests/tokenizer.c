#include <criterion/criterion.h>
#include "tokenizer.h"
#include "tokenizer_config.h"

token_t **tokens;
extern token_config_t token_config;

void teardown(void)
{
    tokenizer__clean_tokens(tokens);
}

Test(tokenizer, string__tokenizes_numbers, .fini = teardown) {
    tokens = tokenizer__string(&token_config, "1 2 3 4 5", 1);

    cr_assert_str_eq(tokens[0]->value, "1");
    cr_assert_eq(tokens[0]->type, tok_num);

    cr_assert_str_eq(tokens[1]->value, "2");
    cr_assert_eq(tokens[1]->type, tok_num);

    cr_assert_str_eq(tokens[2]->value, "3");
    cr_assert_eq(tokens[2]->type, tok_num);

    cr_assert_str_eq(tokens[3]->value, "4");
    cr_assert_eq(tokens[3]->type, tok_num);

    cr_assert_str_eq(tokens[4]->value, "5");
    cr_assert_eq(tokens[4]->type, tok_num);
}

Test(tokenizer, string__tokenizes_numbers_with_commas, .fini = teardown) {
    tokens = tokenizer__string(&token_config, "1, 2, 3", 1);

    cr_assert_str_eq(tokens[0]->value, "1");
    cr_assert_eq(tokens[0]->type, tok_num);
    cr_assert_str_eq(tokens[1]->value, ",");
    cr_assert_eq(tokens[1]->type, tok_del);

    cr_assert_str_eq(tokens[2]->value, "2");
    cr_assert_eq(tokens[2]->type, tok_num);
    cr_assert_str_eq(tokens[3]->value, ",");
    cr_assert_eq(tokens[3]->type, tok_del);

    cr_assert_str_eq(tokens[4]->value, "3");
    cr_assert_eq(tokens[4]->type, tok_num);
}

Test(tokenizer, string__tokenizes_atoms_with_numbers_and_commas, .fini = teardown) {
    tokens = tokenizer__string(&token_config, "1, foo, 3, bar, baz", 1);

    cr_assert_str_eq(tokens[0]->value, "1");
    cr_assert_eq(tokens[0]->type, tok_num);
    cr_assert_str_eq(tokens[1]->value, ",");
    cr_assert_eq(tokens[1]->type, tok_del);

    cr_assert_str_eq(tokens[2]->value, "foo");
    cr_assert_eq(tokens[2]->type, tok_atom);
    cr_assert_str_eq(tokens[3]->value, ",");
    cr_assert_eq(tokens[3]->type, tok_del);

    cr_assert_str_eq(tokens[4]->value, "3");
    cr_assert_eq(tokens[4]->type, tok_num);
    cr_assert_str_eq(tokens[5]->value, ",");
    cr_assert_eq(tokens[5]->type, tok_del);

    cr_assert_str_eq(tokens[6]->value, "bar");
    cr_assert_eq(tokens[6]->type, tok_atom);
    cr_assert_str_eq(tokens[7]->value, ",");
    cr_assert_eq(tokens[7]->type, tok_del);

    cr_assert_str_eq(tokens[8]->value, "baz");
    cr_assert_eq(tokens[8]->type, tok_atom);
}

Test(tokenizer, string__assigns_last_token_to_null, .fini = teardown) {
    tokens = tokenizer__string(&token_config, "1,", 1);

    cr_assert_str_eq(tokens[0]->value, "1");
    cr_assert_eq(tokens[0]->type, tok_num);
    cr_assert_str_eq(tokens[1]->value, ",");
    cr_assert_eq(tokens[1]->type, tok_del);

    cr_assert_eq(tokens[2], NULL);
}

Test(tokenizer, string__works_without_spaces, .fini = teardown) {
    tokens = tokenizer__string(&token_config, "1,2,foo,bar", 1);

    cr_assert_str_eq(tokens[0]->value, "1");
    cr_assert_eq(tokens[0]->type, tok_num);
    cr_assert_str_eq(tokens[1]->value, ",");
    cr_assert_eq(tokens[1]->type, tok_del);

    cr_assert_str_eq(tokens[2]->value, "2");
    cr_assert_eq(tokens[2]->type, tok_num);
    cr_assert_str_eq(tokens[3]->value, ",");
    cr_assert_eq(tokens[3]->type, tok_del);

    cr_assert_str_eq(tokens[4]->value, "foo");
    cr_assert_eq(tokens[4]->type, tok_atom);
    cr_assert_str_eq(tokens[5]->value, ",");
    cr_assert_eq(tokens[5]->type, tok_del);

    cr_assert_str_eq(tokens[6]->value, "bar");
    cr_assert_eq(tokens[6]->type, tok_atom);
}

Test(tokenizer, string__tokenizes_brackets_as_delimeters, .fini = teardown) {
    tokens = tokenizer__string(&token_config, "1, (2, 3), 5", 1);

    cr_assert_str_eq(tokens[0]->value, "1");
    cr_assert_eq(tokens[0]->type, tok_num);
    cr_assert_str_eq(tokens[1]->value, ",");
    cr_assert_eq(tokens[1]->type, tok_del);

    cr_assert_str_eq(tokens[2]->value, "(");
    cr_assert_eq(tokens[2]->type, tok_prior_start);

    cr_assert_str_eq(tokens[3]->value, "2");
    cr_assert_eq(tokens[3]->type, tok_num);
    cr_assert_str_eq(tokens[4]->value, ",");
    cr_assert_eq(tokens[4]->type, tok_del);

    cr_assert_str_eq(tokens[5]->value, "3");
    cr_assert_eq(tokens[5]->type, tok_num);

    cr_assert_str_eq(tokens[6]->value, ")");
    cr_assert_eq(tokens[6]->type, tok_prior_end);

    cr_assert_str_eq(tokens[7]->value, ",");
    cr_assert_eq(tokens[7]->type, tok_del);

    cr_assert_str_eq(tokens[8]->value, "5");
    cr_assert_eq(tokens[8]->type, tok_num);
}

Test(tokenizer, string__comments_at_the_start_of_the_line, .fini = teardown) {
    tokens = tokenizer__string(&token_config, "-- 1, (2, 3), 5", 1);

    cr_assert_null(tokens[0]);
}

Test(tokenizer, string__comments_at_the_end_of_the_line, .fini = teardown) {
    tokens = tokenizer__string(&token_config, "1, (2, 3), 5 -- just a comment", 1);

    cr_assert_str_eq(tokens[0]->value, "1");
    cr_assert_eq(tokens[0]->type, tok_num);
    cr_assert_str_eq(tokens[1]->value, ",");
    cr_assert_eq(tokens[1]->type, tok_del);

    cr_assert_str_eq(tokens[2]->value, "(");
    cr_assert_eq(tokens[2]->type, tok_prior_start);

    cr_assert_str_eq(tokens[3]->value, "2");
    cr_assert_eq(tokens[3]->type, tok_num);
    cr_assert_str_eq(tokens[4]->value, ",");
    cr_assert_eq(tokens[4]->type, tok_del);

    cr_assert_str_eq(tokens[5]->value, "3");
    cr_assert_eq(tokens[5]->type, tok_num);

    cr_assert_str_eq(tokens[6]->value, ")");
    cr_assert_eq(tokens[6]->type, tok_prior_end);

    cr_assert_str_eq(tokens[7]->value, ",");
    cr_assert_eq(tokens[7]->type, tok_del);

    cr_assert_str_eq(tokens[8]->value, "5");
    cr_assert_eq(tokens[8]->type, tok_num);

    cr_assert_null(tokens[9]);
}
