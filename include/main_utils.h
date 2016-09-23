/**
 * @file
 * @brief Collection of functions for the main function
 */
#ifndef __main_utils_h__
#define __main_utils_h__

#include "call_tree.h"
#include "tokenizer.h"

/**
 * @brief Prints function call tree in the lisp style
 *
 * @param[in] tree Function call tree
 * @param[in] i Token index where to start printing
 */
void print_tree(const call_tree_t *tree, mapv_t i);

/**
 * @brief Cleans function call tree and tokens
 *
 * @param[in] tree Function call tree
 * @param[in] tokens Tokens array
 */
void clean(call_tree_t *tree, token_t **tokens);

#endif
