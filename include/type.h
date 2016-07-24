#ifndef __type_h__
#define __type_h__

#include "list.h"
#include "tokenizer.h"
#include "function.h"

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
        tid_string // string
    } type;
    union {
        fn_t *fn;
        int num;
        const char *atom;
        const char *string;
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
