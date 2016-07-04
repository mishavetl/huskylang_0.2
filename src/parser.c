#include <string.h>
#include "inc/list.h"
#include "inc/parser_element.h"
#include "inc/parser.h"
#include "inc/dbg.h"

#define DELIMITERS ",:"
#define PRIORITY_START "("
#define PRIORITY_END ")"
#define END_OF_CALL "\n"

#define NEW_PARSER_ELEMENT_NODE(token, mixed) \
    parser_element_node__new(parser_element__new(token, mixed))

parser_element_node_t *parser__tokens_to_list__body(token_t **tokens, size_t *i_);

parser_element_node_t *new_node(token_t **tokens, size_t *i)
{
    parser_element_node_t *node;

    if (strcmp(tokens[*i]->value, PRIORITY_START) == 0) {
        *i += 1;
        node = NEW_PARSER_ELEMENT_NODE(NULL, parser__tokens_to_list__body(tokens, i));
    } else {
        node = NEW_PARSER_ELEMENT_NODE(tokens[*i], NULL);
    }

    return node;
}

/**
 * Body of the tokens_to_list funciton
 *
 */
parser_element_node_t *parser__tokens_to_list__body(token_t **tokens, size_t *i_)
{
    int append_next = 0;
    size_t i = *i_;

    parser_element_node_t *node;

    parser_element_node_t *cur = NULL;
    parser_element_node_t *head = NULL;
    parser_element_node_t *cur_list = head;

    for (; tokens[i]; i++) {
        if (strcmp(PRIORITY_END, tokens[i]->value) == 0) {
            break;
        } else if (strstr(DELIMITERS, tokens[i]->value)) {
            append_next = 1;
        } else if (append_next) {
            node = new_node(tokens, &i);

            if (cur) {
                parser_element_node__append_after(cur, node);
                cur = cur->next;
            } else {
                cur = node;
                head = NEW_PARSER_ELEMENT_NODE(NULL, cur);
                cur_list = head;
            }

            append_next = 0;
        } else {
            cur = new_node(tokens, &i);
            node = NEW_PARSER_ELEMENT_NODE(NULL, cur);

            if (cur_list) {
                parser_element_node__append_after(cur_list, node);
                cur_list = cur_list->next;
            } else {
                cur_list = node;
                head = cur_list;
            }
        }
    }

    *i_ = i;

    return head;
}

/**
 * Converts tokens to a list
 *
 */
parser_element_node_t *parser__tokens_to_list(token_t **tokens)
{
    size_t i = 0;

    return parser__tokens_to_list__body(tokens, &i);
}

/**
 * Parses function call from a parser element list
 *
 */
parser_element_node_t *parser__funcall(parser_element_node_t *head)
{
    size_t len;
    parser_element_node_t *node = head;
    parser_element_node_t *node_;

    for (len = 0; node; len++, node = node->next) {
        if (node->val->type == tid_parser_mixed) {
            for (node_ = node->val->data.mixed; node_; node_ = node_->next) {
                if (node_->val->type == tid_parser_mixed) {
                    node_->val->data.mixed = parser__funcall(
                        node_->val->data.mixed
                    );
                }
            }
        }
    }

    if (len == 1 || len == 2) {
        return head;
    } else if (len == 3) {
        head = head->next;
        head->next->val->data.mixed = parser_element_node__add(head->prev->val->data.mixed, head->next->val->data.mixed);
        node = parser_element_node__remove(head->prev);
        parser_element__clean(node->val);
        free(node);
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
    parser_element_node_t *token_list = parser__tokens_to_list(tokens);

    return parser__funcall(token_list);
}
