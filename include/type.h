/**
 * @file
 * @brief Type struct and functions to manage it
 */
#ifndef __type_h__
#define __type_h__

#include "tokenizer.h"
#include "function.h"
#include "call_tree.h"
#include "../src/list/src/list.h"

typedef struct variable var_t;

/**
 * @brief Value of a type
 */
union type_value {
    fn_t *fn; ///< Function value
    int num; ///< Number value
    const char *atom; ///< Atom value
    const char *string; ///< String value
    type_t **tuple; ///< Tuple value
    list_t *list; ///< List value
    call_tree_t *tree; ///< Function call tree value (saved function call)
};

/**
 * @brief Type of a type
 */
enum tid {
    tid_num, ///< Number
    tid_atom, ///< Atom
    tid_fn, ///< Function
    tid_string, ///< String
    tid_tuple, ///< Tuple (immutable array)
    tid_list, ///< List
    tid_saved ///< Saved
};

/**
 * @brief Type structure
 * @details Use it for all data in the language
 */
typedef struct type {
    /** Type enumerable of the type entry */
    enum tid type;
    /** Value of the type entry */
    union type_value value;
} type_t;

/**
 * @brief Constructs type from the token
 *
 * @param[in] token Token to construct from
 * @param[out] type Type to write to
 *
 * @return Status: if success then 0 else -1
 */
int type_from_token(token_t *token, type_t *type);

#endif
