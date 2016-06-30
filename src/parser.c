#include <string.h>
#include "inc/list.h"
#include "inc/parser_element.h"
#include "inc/parser.h"
#include "inc/dbg.h"

#define DELIMITERS ",:"
#define END_OF_CALL "\n"

#define NEW_PARSER_ELEMENT_NODE(token, mixed) \
    parser_element_node__new(parser_element__new(token, mixed))

/**
 * Converts tokens to a list
 *
 */
parser_element_node_t *parser__tokens_to_list(token_t **tokens, size_t *len)
{
    size_t i;
    int append_next = 0;
    parser_element_node_t *cur;
    parser_element_node_t *head;
    parser_element_node_t *cur_list;

    cur = NEW_PARSER_ELEMENT_NODE(tokens[0], NULL);
    head = NEW_PARSER_ELEMENT_NODE(NULL, cur);
    cur_list = head;
    *len = 1;

    for (i = 1; tokens[i]; i++) {
        if (strstr(DELIMITERS, tokens[i]->value)) {
            append_next = 1;
        } else if (append_next) {
            parser_element_node__append_after(cur, NEW_PARSER_ELEMENT_NODE(tokens[i], NULL));
            cur = cur->next;
            append_next = 0;
        } else {
            cur = NEW_PARSER_ELEMENT_NODE(tokens[i], NULL);
            parser_element_node__append_after(cur_list, NEW_PARSER_ELEMENT_NODE(NULL, cur));
            cur_list = cur_list->next;
            *len += 1;
        }
    }

    return head;
}

/**
 * Parses function call from a parser element list
 *
 */
parser_element_node_t *parser__funcall(parser_element_node_t *head, size_t len)
{
    parser_element_node_t *tmp;

    if (len == 1) {
        return head;
    } else if (len == 2) {
        //
    } else if (len == 3) {
        head = head->next;
        head->next->val->data.mixed = parser_element_node__add(head->prev->val->data.mixed, head->next->val->data.mixed);
        tmp = parser_element_node__remove(head->prev);
        parser_element__clean(tmp->val);
        free(tmp);
    } else {
        debug("syntax_error");
        return NULL;
    }

    return head;
}

/**
 * TODO make newline support
 */
parser_element_node_t *parser__parse(token_t **tokens)
{
    size_t len;
    parser_element_node_t *token_list = parser__tokens_to_list(tokens, &len);

    return parser__funcall(token_list, len);
}
