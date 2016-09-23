/**
 * @file
 * @brief Husky error structure and functions to handle it
 */
#ifndef __huserr_h__
#define __huserr_h__

#include "type.h"

/**
 * @brief Husky error
 */
typedef struct huserr {
    /** Token on top of which, error occured */
    token_t *token;
    /** Error name */
    const char *name;
    /** Error message */
    const char *msg;
} huserr_t;

/**
 * @brief Converts `huserr` to string
 *
 * @param[in] scope Scope
 *
 * @return Error string
 */
char *huserr__to_string(scope_t *scope);

#endif
