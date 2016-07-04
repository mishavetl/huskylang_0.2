#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <stdbool.h>
#include "tokenizer_config.h"

typedef struct token {
    size_t lineto, linefrom, col;
    char *value;
    enum token_type type;
} token_t;

token_t **tokenizer__string(const token_config_t *config, const char *str);
void tokenizer__clean_tokens(token_t **tokens);

#endif
