#ifndef __parser_h__
#define __parser_h__

#include "tokenizer.h"
#include "call_tree.h"

/**
 * Constructs a call tree
 *
 * @author mishavetl
 *
 * @param call_tree (pointer to an instance of the call_tree)
 * @param tokens (an array of tokens)
 *
 * @ret (status of the parse -1 for error, >= 0 for success)
 *
 */
mapv_t parser__funcall(call_tree_t *call_tree, token_t **tokens);
// call_tree_t *parser__parse(token_t **tokens);

#endif
