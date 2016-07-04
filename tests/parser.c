#include <criterion/criterion.h>
#include "parser.h"
#include "tokenizer.h"
#include "tokenizer_config.h"

#define INIT(query) \
    token_config_t config; \
    tokenizer__generate_config(&config); \
    token_t **tokens = tokenizer__string(&config, query); \
    parser_element_node_t *head = parser__tokens_to_list(tokens);

#define FINI() \
    parser_element__cleanall(head); \
    tokenizer__clean_tokens(tokens);

Test(parser, tokens_to_list__converts_simple_sequence_of_integers_separated_by_a_comma_to_a_list) {
    INIT("1, 2, 3");

    parser_element_node_t *mixed = head->val->data.mixed;

    cr_assert_str_eq(mixed->val->data.token->value, "1");
    cr_assert_str_eq(mixed->next->val->data.token->value, "2");
    cr_assert_str_eq(mixed->next->next->val->data.token->value, "3");
    cr_assert_eq(mixed->next->next->next, NULL);

    FINI();
}

Test(parser, tokens_to_list__converts_a_sequence_of_integers_separated_by_a_comma_with_brackets_to_a_list) {
    INIT("1, (2, 3), 5");

    parser_element_node_t *mixed = head->val->data.mixed;

    cr_assert_str_eq(mixed->val->data.token->value, "1");

    cr_assert_str_eq(mixed->next->val->data.mixed->val->data.mixed->val->data.token->value, "2");
    cr_assert_str_eq(mixed->next->val->data.mixed->val->data.mixed->next->val->data.token->value, "3");
    cr_assert_eq(mixed->next->val->data.mixed->val->data.mixed->next->next, NULL);

    cr_assert_str_eq(mixed->next->next->val->data.token->value, "5");
    cr_assert_eq(mixed->next->next->next, NULL);

    FINI();
}

Test(parser, tokens_to_list__converts_a_function_call_with_brackets_to_a_list) {
    INIT("(2 * 3) + 3");

    cr_assert_str_eq(head->val->data.mixed->val->data.mixed->val->data.mixed->val->data.token->value, "2");
    cr_assert_str_eq(head->val->data.mixed->val->data.mixed->next->val->data.mixed->val->data.token->value, "*");
    cr_assert_str_eq(head->val->data.mixed->val->data.mixed->next->next->val->data.mixed->val->data.token->value, "3");
    cr_assert_eq(head->val->data.mixed->val->data.mixed->next->next->next, NULL);

    cr_assert_str_eq(head->next->val->data.mixed->val->data.token->value, "+");
    cr_assert_str_eq(head->next->next->val->data.mixed->val->data.token->value, "3");
    cr_assert_eq(head->next->next->next, NULL);

    FINI();
}

Test(parser, tokens_to_list__converts_a_list_of_integers_with_brackets) {
    INIT("(1, 2, 3)");

    cr_assert_str_eq(head->val->data.mixed->val->data.mixed->val->data.mixed->val->data.token->value, "1");
    cr_assert_str_eq(head->val->data.mixed->val->data.mixed->val->data.mixed->next->val->data.token->value, "2");
    cr_assert_str_eq(head->val->data.mixed->val->data.mixed->val->data.mixed->next->next->val->data.token->value, "3");
    cr_assert_eq(head->val->data.mixed->val->data.mixed->val->data.mixed->next->next->next, NULL);

    FINI();
}

Test(parser, tokens_to_list__converts_function_call_without_mixed_arguments_1) {
    INIT("1, 2 + 3, 4, 5");

    cr_assert_str_eq(head->val->data.mixed->val->data.token->value, "1");
    cr_assert_str_eq(head->val->data.mixed->next->val->data.token->value, "2");
    cr_assert_str_eq(head->next->val->data.mixed->val->data.token->value, "+");
    cr_assert_str_eq(head->next->next->val->data.mixed->val->data.token->value, "3");
    cr_assert_str_eq(head->next->next->val->data.mixed->next->val->data.token->value, "4");
    cr_assert_str_eq(head->next->next->val->data.mixed->next->next->val->data.token->value, "5");
    cr_assert_eq(head->next->next->val->data.mixed->next->next->next, NULL);
    cr_assert_eq(head->next->next->next, NULL);

    FINI();
}

Test(parser, tokens_to_list__converts_function_call_without_mixed_arguments_2) {
    INIT("+ 1, 2");

    cr_assert_str_eq(head->val->data.mixed->val->data.token->value, "+");
    cr_assert_str_eq(head->next->val->data.mixed->val->data.token->value, "1");
    cr_assert_str_eq(head->next->val->data.mixed->next->val->data.token->value, "2");
    cr_assert_eq(head->next->val->data.mixed->next->next, NULL);
    cr_assert_eq(head->next->next, NULL);

    FINI();
}

Test(parser, tokens_to_list__converts_function_call_with_namespaced_function_name) {
    INIT("foo:bar 1, 2");

    cr_assert_str_eq(head->val->data.mixed->val->data.token->value, "foo");
    cr_assert_str_eq(head->val->data.mixed->next->val->data.token->value, "bar");
    cr_assert_str_eq(head->next->val->data.mixed->val->data.token->value, "1");
    cr_assert_str_eq(head->next->val->data.mixed->next->val->data.token->value, "2");
    cr_assert_eq(head->next->val->data.mixed->next->next, NULL);
    cr_assert_eq(head->next->next, NULL);

    FINI();
}

Test(parser, tokens_to_list__converts_function_call_without_mixed_arguments_3) {
    INIT("1 + 2");

    cr_assert_str_eq(head->val->data.mixed->val->data.token->value, "1");
    cr_assert_str_eq(head->next->val->data.mixed->val->data.token->value, "+");
    cr_assert_str_eq(head->next->next->val->data.mixed->val->data.token->value, "2");
    cr_assert_eq(head->next->next->next, NULL);

    FINI();
}

Test(parser, funcall__parses_function_call_without_mixed_arguments_1) {
    INIT("1, 2 + 3, 4, 5");

    head = parser__funcall(head);

    cr_assert_str_eq(head->val->data.mixed->val->data.token->value, "+");
    cr_assert_eq(head->val->data.mixed->next, NULL);
    cr_assert_str_eq(head->next->val->data.mixed->val->data.token->value, "1");
    cr_assert_str_eq(head->next->val->data.mixed->next->val->data.token->value, "2");
    cr_assert_str_eq(head->next->val->data.mixed->next->next->val->data.token->value, "3");
    cr_assert_str_eq(head->next->val->data.mixed->next->next->next->val->data.token->value, "4");
    cr_assert_str_eq(head->next->val->data.mixed->next->next->next->next->val->data.token->value, "5");
    cr_assert_eq(head->next->val->data.mixed->next->next->next->next->next, NULL);
    cr_assert_eq(head->next->next, NULL);

    FINI();
}

Test(parser, funcall__parses_function_call_without_mixed_arguments_2) {
    INIT("1 + 2");

    head = parser__funcall(head);

    cr_assert_str_eq(head->val->data.mixed->val->data.token->value, "+");
    cr_assert_eq(head->val->data.mixed->next, NULL);
    cr_assert_str_eq(head->next->val->data.mixed->val->data.token->value, "1");
    cr_assert_str_eq(head->next->val->data.mixed->next->val->data.token->value, "2");
    cr_assert_eq(head->next->val->data.mixed->next->next, NULL);
    cr_assert_eq(head->next->next, NULL);

    FINI();
}

Test(parser, funcall__parses_function_call_with_namespaced_function_name) {
    INIT("1 foo:bar 2");

    head = parser__funcall(head);

    cr_assert_str_eq(head->val->data.mixed->val->data.token->value, "foo");
    cr_assert_str_eq(head->val->data.mixed->next->val->data.token->value, "bar");
    cr_assert_str_eq(head->next->val->data.mixed->val->data.token->value, "1");
    cr_assert_str_eq(head->next->val->data.mixed->next->val->data.token->value, "2");
    cr_assert_eq(head->next->val->data.mixed->next->next, NULL);
    cr_assert_eq(head->next->next, NULL);

    FINI();
}

Test(parser, funcall__parses_function_call_with_nested_function_calls_and_brackets) {
    INIT("* 1, (1 + 2), 2, (3 + 3)");

    head = parser__funcall(head);

    cr_assert_str_eq(head->val->data.mixed->val->data.token->value, "*");

    cr_assert_str_eq(head->next->val->data.mixed->val->data.token->value, "1");

    cr_assert_str_eq(head->next->val->data.mixed->next->val->data.mixed->val->data.mixed->val->data.token->value, "+");
    cr_assert_str_eq(
        head->next->val->data.mixed->next->val->data.mixed->next->val->data.mixed->val->data.token->value, "1"
    );
    cr_assert_str_eq(
        head->next->val->data.mixed->next->val->data.mixed->next->val->data.mixed->next->val->data.token->value, "2"
    );

    cr_assert_str_eq(head->next->val->data.mixed->next->next->val->data.token->value, "2");

    cr_assert_str_eq(head->next->val->data.mixed->next->next->next->val->data.mixed->val->data.mixed->val->data.token->value, "+");
    cr_assert_str_eq(
        head->next->val->data.mixed->next->next->next->val->data.mixed->next->val->data.mixed->val->data.token->value, "3"
    );
    cr_assert_str_eq(
        head->next->val->data.mixed->next->next->next->val->data.mixed->next->val->data.mixed->next->val->data.token->value, "3"
    );

    FINI();
}
