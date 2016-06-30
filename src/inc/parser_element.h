#ifndef _PARSER_ELEMENT_H
#define _PARSER_ELEMENT_H

#include "list.h"
#include "tokenizer.h"

typedef struct parser_element_node parser_element_node_t;

typedef struct parser_element {
    enum {
        tid_parser_token,
        tid_parser_mixed
    } type;
    union ParserElementData {
        token_t *token;
        parser_element_node_t *mixed;
    } data;
} parser_element_t;

INIT_LIST_STRUCT(parser_element, parser_element_t *);
REG_LIST(parser_element, parser_element_t *);

parser_element_t *parser_element__new(token_t *token, parser_element_node_t *mixed);
void parser_element__printall(parser_element_node_t *cur, int n);
void parser_element__clean(parser_element_t *elem);
void parser_element__cleanall(parser_element_node_t *elem);

#endif
