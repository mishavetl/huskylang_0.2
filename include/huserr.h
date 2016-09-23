#ifndef __huserr_h__
#define __huserr_h__

#include "type.h"

/**
 * STRUCT husky error
 *
 * @member token (token where this error occured)
 * @member name (name of an error)
 * @member msg (msg of an error)
 */
typedef struct huserr {
    token_t *token;
    const char *name;
    const char *msg;
} huserr_t;

/**
 * FUNCTION convert `huserr` to string
 *
 * @param scope
 *
 * @return char (error string)
 */
char *huserr__to_string(scope_t *scope);

#endif
