#include <string.h>
#include "inc/list.h"
#include "inc/parser.h"
#include "inc/dbg.h"

#define DELIMITERS ",:"
#define END_OF_CALL "\n"

INIT_LIST(parser_element, parser_element_t *);

#define NEW_PARSER_ELEMENT_NODE(token, mixed) \
    parser_element_node__new(parser_element__new(token, mixed))


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

void parser_element__clean(parser_element_t *elem)
{
    if (elem) free(elem);
}

void parser_element__cleanall(parser_element_node_t *elem)
{
    parser_element_node_t *prev;

    for (; elem; prev = elem, elem = elem->next, free(prev)) {
        if (elem->val->type == tid_parser_mixed)
            parser_element__cleanall(elem->val->data.mixed);

        parser_element__clean(elem->val);
    }
}

void syntax_error(const token_t *token)
{
    debug("Syntax error: %d-%d:%d before %s", token->linefrom, token->lineto, token->col, token->value);
}

parser_element_node_t *parser__tokens_to_list(token_t **tokens, size_t *len)
{
    size_t i;
    int append_next = 0;
    parser_element_node_t *cur;
    parser_element_node_t *head;
    parser_element_node_t *cur_list;

    cur = parser_element_node__new(parser_element__new(tokens[0], NULL));
    head = parser_element_node__new(parser_element__new(NULL, cur));
    cur_list = head;
    *len = 1;

    for (i = 1; tokens[i]; i++) {
        // if (!tokens[i]) {
        //     syntax_error(tokens[i - 1]);
        //     break;
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

parser_element_node_t *parser__funcall(parser_element_node_t *cur, size_t len)
{
    parser_element_node_t *tmp, *head = cur;

    debug("%d", len);

    if (len == 2) {
        //
    } else if (len == 3) {
        head = head->next;
        head->next->val->data.mixed = parser_element_node__add(head->prev->val->data.mixed, head->next->val->data.mixed);
        tmp = parser_element_node__remove(head->prev);
        parser_element__clean(tmp->val);
        free(tmp);
    } else {
        syntax_error(head->val->data.token);
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
    // parser_element_node_t *token_list = parser_element_node__new(token_list);

    // while (tokens) {
    //     cal = parser__tokens_extract_funcall(tokens, &len);
    //     parser__funcall(list, len);
    //     debug("%s", (*tokens)->value);
    //     tokens += len;
    // }
    return parser__funcall(token_list, len);
    // return parser__tokens_to_list(tokens, &len);
}
