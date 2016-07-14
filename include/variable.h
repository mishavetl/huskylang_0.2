#ifndef __variable_h__
#define __variable_h__

#include "type.h"
#include "tokenizer.h"

typedef struct variable {
    char *name;
    type_t *value;
} var_t;

#endif
