#include <criterion/criterion.h>
#include "../src/inc/parser.h"
#include "../src/inc/tokenizer.h"

Test(parser_element, new__correctly_sets_enum_field) {
    token_t token;
    parser_element_node_t parser_elem;
    parser_element_t *elem;

    elem = parser_element__new(NULL, &parser_elem);
    cr_assert_eq(elem->type, tid_parser_mixed);
    parser_element__clean(elem);

    elem = parser_element__new(&token, NULL);
    cr_assert_eq(elem->type, tid_parser_token);
    parser_element__clean(elem);
}

Test(parser_element, new__correctly_sets_union_field) {
    token_t token;
    parser_element_node_t parser_elem;
    parser_element_t *elem;

    elem = parser_element__new(NULL, &parser_elem);
    cr_assert_eq(elem->data.mixed, &parser_elem);
    parser_element__clean(elem);

    elem = parser_element__new(&token, NULL);
    cr_assert_eq(elem->data.token, &token);
    parser_element__clean(elem);
}
