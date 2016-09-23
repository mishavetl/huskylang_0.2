/**
 * @file
 * @brief Set of tokens types and functions to identificate them
 */
#ifndef __tokenizer_config_h__
#define __tokenizer_config_h__

#include <stdlib.h>
#include <stdbool.h>

#define TOKEN_CHAR_SKIP 2
#define TOKEN_CHAR_SKIP_AND_END 3

/**
 * @brief Registers a token
 * @param name Name of the token
 */
#define REGTOKEN(name)                                                          \
    do {                                                                        \
        config->check_functions[tok_##name] = is_tok_##name;                    \
    } while (0);

/**
 * @brief Creates `tokenizer__generate_config` function
 * @param ... Tokens registrations
 */
#define TOKENS(...) int tokenizer__generate_config(token_config_t *config) {    \
    size_t i;                                                                   \
    for (i = 0; i <= __tok_amount; i++) {                                       \
        config->check_functions[i] = NULL;                                      \
    }                                                                           \
                                                                                \
    __VA_ARGS__                                                                 \
                                                                                \
    return 0;                                                                   \
}

/**
 * @brief Creates token check function
 * @param name Name of the token
 * @param ... Function body
 */
#define TOKEN(name, ...)                                                        \
    int is_tok_##name(size_t pos, char ch, int started) {                       \
        (void) pos;                                                             \
        (void) ch;                                                              \
        (void) started;                                                         \
                                                                                \
        __VA_ARGS__                                                             \
    }

/**
 * @brief Token types enumerable
 * @details Register here your tokens
 */
enum token_type {
    tok_num, ///< Number
    tok_prior_start, ///< Priority start character
    tok_prior_end, ///< Priority end character
    tok_prior_start_saved, ///< Saved priority start character
    tok_del, ///< Delimiter
    tok_string, ///< String
    tok_atom, ///< Atom
    __tok_amount ///< Amount of token types
};

/** Token check function type */
typedef int (*token_check_function_t)(size_t, char, int);

/**
 * @brief Token config structure
 */
typedef struct token_config {
    /** An array of token check functions */
    token_check_function_t check_functions[__tok_amount + 1];
} token_config_t;

/**
 * @brief Generates token config
 *
 * @param[out] config Pointer to a clean instance of token_config_t
 *
 * @return Status of the generation -1 for error, 0 for success
 */
int tokenizer__generate_config(token_config_t *config);

#endif
