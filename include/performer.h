/**
 * @file
 * @brief Collection of functions for execution of a function call tree
 */
#ifndef __performer_h__
#define __performer_h__

#include "call_tree.h"
#include "variable.h"

/**
 * @brief
 * Executes a call with specific variables set (scope) on a specific position
 *
 * @param[in] tree Function call tree to execute
 * @param[in] scope Scope where to execute
 * @param[out] ret Return pointer of a function call
 * @param[in] i Position where to start performing
 *
 * @return Status of the execution: -1 for error, >= 0 for success
 */
int performer__funcall(
    call_tree_t *tree, scope_t *scope, data_t *ret, mapv_t i
);

/**
 * @brief Executes a call with specific variables set (scope)
 *
 * @param[in] tree Function call tree to execute
 * @param[in] scope Scope where to execute
 * @param[out] ret Return pointer of a function call
 *
 * @return Status of the execution: -1 for error, >= 0 for success
 */
int performer__execute(call_tree_t *tree, scope_t *scope, data_t *ret);

#endif
