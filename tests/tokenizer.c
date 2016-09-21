#include <criterion/criterion.h>
#include "tokenizer.h"
#include "tokenizer_config.h"

token_t **tokens;

int line = 1;

void teardown(void)
{
    tokenizer__clean_tokens(tokens);
}

Test(tokenizer, string__tokenizes_numbers, .fini = teardown) {
    token_config_t token_config;
    tokenizer__generate_config(&token_config);

    tokens = tokenizer__string(&token_config, "1 2 3 4 5", &line);

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
    token_config_t token_config;
    tokenizer__generate_config(&token_config);

    tokens = tokenizer__string(&token_config, "1, 2, 3", &line);

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
    token_config_t token_config;
    tokenizer__generate_config(&token_config);

    tokens = tokenizer__string(&token_config, "1, foo, 3, bar, baz", &line);

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

Test(tokenizer, string__atoms_with_special_characters, .fini = teardown) {
    token_config_t token_config;
    tokenizer__generate_config(&token_config);

    tokens = tokenizer__string(&token_config, "`\\tfoo \\`bar\\` baz\\\\\\n`", &line);

    cr_assert_str_eq(tokens[0]->value, "\tfoo `bar` baz\\\n");
    cr_assert_eq(tokens[0]->type, tok_atom);

    cr_assert_null(tokens[1]);
}

Test(tokenizer, string__atoms_with_special_characters_as_an_arg, .fini = teardown) {
    token_config_t token_config;
    tokenizer__generate_config(&token_config);

    tokens = tokenizer__string(&token_config, "`\\tfoo \\`bar\\` baz\\\\\\n` foo bar", &line);

    cr_assert_str_eq(tokens[0]->value, "\tfoo `bar` baz\\\n");
    cr_assert_eq(tokens[0]->type, tok_atom);

    cr_assert_str_eq(tokens[1]->value, "foo");
    cr_assert_eq(tokens[1]->type, tok_atom);

    cr_assert_str_eq(tokens[2]->value, "bar");
    cr_assert_eq(tokens[2]->type, tok_atom);

    cr_assert_null(tokens[3]);
}

Test(tokenizer, string__assigns_last_token_to_null, .fini = teardown) {
    token_config_t token_config;
    tokenizer__generate_config(&token_config);

    tokens = tokenizer__string(&token_config, "1,", &line);

    cr_assert_str_eq(tokens[0]->value, "1");
    cr_assert_eq(tokens[0]->type, tok_num);
    cr_assert_str_eq(tokens[1]->value, ",");
    cr_assert_eq(tokens[1]->type, tok_del);

    cr_assert_eq(tokens[2], NULL);
}

Test(tokenizer, string__works_without_spaces, .fini = teardown) {
    token_config_t token_config;
    tokenizer__generate_config(&token_config);

    tokens = tokenizer__string(&token_config, "1,2,foo,bar", &line);

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
    token_config_t token_config;
    tokenizer__generate_config(&token_config);

    tokens = tokenizer__string(&token_config, "1, (2, 3), 5", &line);

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
    token_config_t token_config;
    tokenizer__generate_config(&token_config);

    tokens = tokenizer__string(&token_config, "-- 1, (2, 3), 5", &line);

    cr_assert_null(tokens[0]);
}

Test(tokenizer, string__comments_at_the_end_of_the_line, .fini = teardown) {
    token_config_t token_config;
    tokenizer__generate_config(&token_config);

    tokens = tokenizer__string(&token_config, "1, (2, 3), 5 -- just a comment", &line);

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

Test(tokenizer, string__basic_string, .fini = teardown) {
    token_config_t token_config;
    tokenizer__generate_config(&token_config);

    tokens = tokenizer__string(&token_config, "'foo bar'", &line);

    cr_assert_str_eq(tokens[0]->value, "foo bar");
    cr_assert_eq(tokens[0]->type, tok_string);

    cr_assert_null(tokens[1]);
}

Test(tokenizer, string__strings_as_arguments, .fini = teardown) {
    token_config_t token_config;
    tokenizer__generate_config(&token_config);

    tokens = tokenizer__string(&token_config, "foo 'foo bar', (bar 'foo bar')", &line);

    cr_assert_str_eq(tokens[0]->value, "foo");
    cr_assert_eq(tokens[0]->type, tok_atom);

    cr_assert_str_eq(tokens[1]->value, "foo bar");
    cr_assert_eq(tokens[1]->type, tok_string);

    cr_assert_str_eq(tokens[2]->value, ",");
    cr_assert_eq(tokens[2]->type, tok_del);

    cr_assert_str_eq(tokens[3]->value, "(");
    cr_assert_eq(tokens[3]->type, tok_prior_start);

    cr_assert_str_eq(tokens[4]->value, "bar");
    cr_assert_eq(tokens[4]->type, tok_atom);

    cr_assert_str_eq(tokens[5]->value, "foo bar");
    cr_assert_eq(tokens[5]->type, tok_string);

    cr_assert_str_eq(tokens[6]->value, ")");
    cr_assert_eq(tokens[6]->type, tok_prior_end);

    cr_assert_null(tokens[7]);
}

Test(tokenizer, string__strings_escape_characters, .fini = teardown) {
    token_config_t token_config;
    tokenizer__generate_config(&token_config);

    tokens = tokenizer__string(&token_config, "'\\tfoo\\\\ \\'bar\\n'", &line);

    cr_assert_str_eq(tokens[0]->value, "\tfoo\\ 'bar\n");
    cr_assert_eq(tokens[0]->type, tok_string);

    cr_assert_null(tokens[1]);
}
