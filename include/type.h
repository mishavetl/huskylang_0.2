#ifndef __type_h__
#define __type_h__

#include "list.h"
#include "tokenizer.h"
#include "function.h"
#include "../src/list/src/list.h"

typedef struct variable var_t;

/**
 * STRUCT type
 *
 * @author mishavetl
 *
 * @member type (type enumerable of the type entry)
 * @member value (value of the type entry
 *
 */
typedef struct type {
    enum tid {
        tid_num, // number
        tid_atom, // atom
        tid_fn, // function
        tid_string, // string
        tid_tuple, // tuple (immutable array)
        tid_list // list
    } type;
    union {
        fn_t *fn;
        int num;
        const char *atom;
        const char *string;
        type_t **tuple;
        list_t *list;
    } value;
} type_t;

/**
 * Constructs type from the token
 *
 * @author mishavetl
 *
 * @param token (token to construct from)
 * @param type (type to write to)
 *
 * @return (status: if success then 0 else -1)
 *
 */
int type_from_token(token_t *token, type_t *type);

#endif
