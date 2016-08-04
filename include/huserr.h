#ifndef __huserr_h__
#define __huserr_h__

#include "type.h"

typedef struct huserr {
    token_t *token;
    const char *name;
    const char *msg;
} huserr_t;

#endif
