#ifndef __performer_h__
#define __performer_h__

#include "call_tree.h"
#include "variable.h"

/**
 * FUNCTION
 * Executes a call with specific variables set (scope) on a specific position
 *
 * @author mishavetl
 *
 * @param tree (function call tree to execute)
 * @param scope (scope where to execute)
 * @param ret (return pointer of a function call)
 * @param i (position where to start performing)
 *
 * @return int (status)
 */

int
performer__funcall(
    call_tree_t *tree, scope_t *scope, type_t *ret, mapv_t i
);

/**
 * FUNCTION Executes a call with specific variables set (scope)
 *
 * @author mishavetl
 *
 * @param tree (function call tree to execute)
 * @param scope (scope where to execute)
 * @param ret (return pointer of a function call)
 *
 * @return int (status)
 */
int performer__execute(call_tree_t *tree, scope_t *scope, type_t *ret);

#endif
