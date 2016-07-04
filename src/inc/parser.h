#ifndef _PARSER_H
#define _PARSER_H

#include "list.h"
#include "tokenizer.h"
#include "parser_element.h"

parser_element_node_t *parser__funcall(parser_element_node_t *cur);
parser_element_node_t *parser__parse(token_t **tokens);
parser_element_node_t *parser__tokens_to_list(token_t **tokens);

#endif
