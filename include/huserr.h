/**
 * @file
 * @brief Husky error structure and functions to handle it
 */
#ifndef __huserr_h__
#define __huserr_h__

#include "data.h"

#define ERROR_MSG_BUFFER_MAX 1024

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

void huserr__raise(const char *name, const char *msg, token_t *token, scope_t *scope);

#endif
