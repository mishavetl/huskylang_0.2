#ifndef __main_utils_h__
#define __main_utils_h__

#include "call_tree.h"
#include "tokenizer.h"

void print_tree(const call_tree_t *tree, mapv_t i);
void clean(call_tree_t *tree, token_t **tokens);

#endif
