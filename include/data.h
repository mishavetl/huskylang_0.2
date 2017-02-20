/**
 * @file
 * @brief Data struct and functions to manage it
 */
#ifndef __data_h__
#define __data_h__

#include "tokenizer.h"
#include "function.h"
#include "call_tree.h"
#include "../src/list/src/list.h"

typedef struct variable var_t;

typedef long INTEGRAL_TYPE;
typedef double REAL_TYPE;

/**
 * @brief Value of a data
 */
union data_value {
    fn_t *fn; ///< Function value
    INTEGRAL_TYPE integral; ///< Integral value
    REAL_TYPE real; ///< Real value
    const char *atom; ///< Atom value
    const char *string; ///< String value
    data_t **tuple; ///< Tuple value
    list_t *list; ///< List value
    call_tree_t *tree; ///< Function call tree value (saved function call)
};

/**
 * @brief Single type of a data
 */
enum tid {
    tid_atom, ///< Atom
    tid_string, ///< String
    tid_list, ///< List
    tid_tuple, ///< Tuple (immutable array)
    tid_fn, ///< Function
    tid_saved, ///< Saved
    tid_integral, ///< Integral
    tid_real, ///< Real
    __tid_size ///< Size of tid enum
};

#define TYPE_NAMES_MAP {"number", "atom", "fn", "string", "tuple", "list", "saved"}

/**
 * @brief Type structure
 * @details Type of the data
 */
struct type {
    /** Single type enumerable */
    enum tid single;
    /** Multiple type. Use it for additional type information. */
    struct type **multiple;
};

/**
 * @brief Data structure
 * @details Use it for all data in the language
 */
typedef struct data {
    /** Type of the data entry */
    struct type *type;
    /** Value of the data entry */
    union data_value value;
} data_t;

struct type init_type();

int types_identical(struct type *type1, struct type *type2);

struct type *construct_type(enum tid single, struct type **multiple, gc_t *gc);

struct type **initializer_type_to_array(struct type *types[], int n, gc_t *gc);
#define itta initializer_type_to_array
#define stt (struct type *[])

/**
 * @brief Constructs data from the token
 *
 * @param[in] token Token to construct from
 * @param[out] data Data to write to
 *
 * @return Status: if success then 0 else -1
 */
int data_from_token(token_t *token, data_t *data, gc_t *gc);

/**
 * @brief Converts typename to type id
 *
 * @param[in] typename Typename to convert
 *
 * @return int tid
 */
int typename_to_tid(const char *typename);

#endif
