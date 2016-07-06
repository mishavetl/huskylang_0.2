#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <stdbool.h>
#include "tokenizer_config.h"

/**
 * Token structure
 *
 * @author mishavetl
 *
 * @member linefrom (line number where token starts)
 * @member lineto (line number where token ends)
 * @member col (col where token starts)
 * @member value (value of the token)
 * @member type (enumerable Token Type)
 *
 */
typedef struct token {
    size_t linefrom, lineto, col;
    char *value;
    enum token_type type;
} token_t;

/**
 * Translates string to the token array
 *
 * @author mishavetl
 *
 * @param config (Token Config structure)
 * @param str (string to translate)
 *
 * @ret (token array if success, NULL if error)
 *
 */
token_t **tokenizer__string(const token_config_t *config, const char *str);

/**
 * Free token array memory
 *
 * @author mishavetl
 *
 * @param token (token array)
 *
 */
void tokenizer__clean_tokens(token_t **tokens);

#endif
