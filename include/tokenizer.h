/**
 * @file
 * @brief Token struct and functions to get tokens from the string
 */
#ifndef __tokenizer_h__
#define __tokenizer_h__

#include "tokenizer_config.h"

/**
 * @brief Token
 */
typedef struct token {
    /** Line number where token starts */
    size_t linefrom;
    /** Line number where token ends */
    size_t lineto;
    /** Col where token starts */
    size_t col;
    /** Token value */
    char *value;
    /** Token type */
    enum token_type type;
} token_t;

/**
 * @brief Translates a string to a token array
 *
 * @param[in] config Tokens config
 * @param[in] str String to translate
 * @param[in] line Line number
 *
 * @return Token array for success, NULL for error
 */
token_t **tokenizer__string(const token_config_t *config, const char *str, int *line);

/**
 * @brief Free token array memory
 *
 * @param[in] token Tokens array
 */
void tokenizer__clean_tokens(token_t **tokens);

#endif
