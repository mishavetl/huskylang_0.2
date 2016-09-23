/**
 * @file
 * @brief Collection of functions for tokens parsing
 */
#ifndef __parser_h__
#define __parser_h__

#include "tokenizer.h"
#include "call_tree.h"

/**
 * @brief Constructs a call tree
 *
 * @param[out] call_tree Pointer to an instance of the call_tree
 * @param[in] tokens Array of tokens
 *
 * @return Status of the parse: -1 for error, >= 0 for success
 */
mapv_t parser__funcall(call_tree_t *call_tree, token_t **tokens);

#endif
