#ifndef __type_h__
#define __type_h__

#include "list.h"
#include "tokenizer.h"

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
    enum {
        tid_num, // number
        tid_atom, // atom
        tid_fn // function
    } type;
    union {
        int (*fn)(struct type **args, struct type *);
        int num;
        const char *atom;
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
