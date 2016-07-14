#ifndef __performer_h__
#define __performer_h__

#include "call_tree.h"
#include "variable.h"

/**
 * Executes a call with specific variables set (scope)
 *
 * @author mishavetl
 *
 *
 */
int performer__execute(call_tree_t *tree, var_t **vars, type_t *ret);

#endif
