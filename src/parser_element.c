#include "inc/list.h"
#include "inc/parser_element.h"

INIT_LIST(parser_element, parser_element_t *);

/**
 * Create new parser element
 *
 */
parser_element_t *parser_element__new(token_t *token, parser_element_node_t *mixed)
{
    parser_element_t *elem = (parser_element_t *) malloc(sizeof(parser_element_t));

    if (token) {
        elem->type = tid_parser_token;
        elem->data.token = token;
    } else {
        elem->type = tid_parser_mixed;
        elem->data.mixed = mixed;
    }

    return elem;
}

/**
 * Print parser element tree
 *
 */
void parser_element__printall(parser_element_node_t *cur, int n)
{
    if (cur->val->type == tid_parser_mixed) {
        printf("%.*s", n, " ");
        puts("[");

        parser_element__printall(cur->val->data.mixed, n + 1);

        printf("%.*s", n, " ");
        printf("]");
    } else {
        printf("%.*s", n + 1, " ");
        printf("%s", cur->val->data.token->value);
    }

    if (cur->next) {
        puts(",");
        parser_element__printall(cur->next, n);
    } else {
        puts("");
    }
}

/**
 * Clean single parser element
 *
 */
void parser_element__clean(parser_element_t *elem)
{
    if (elem) free(elem);
}

/**
 * Clean parser element tree
 *
 */
void parser_element__cleanall(parser_element_node_t *elem)
{
    parser_element_node_t *prev;

    for (; elem; prev = elem, elem = elem->next, free(prev)) {
        if (elem->val->type == tid_parser_mixed)
            parser_element__cleanall(elem->val->data.mixed);

        parser_element__clean(elem->val);
    }
}
