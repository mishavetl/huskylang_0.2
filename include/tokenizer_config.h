#ifndef __tokenizer_config_h__
#define __tokenizer_config_h__

#include <stdlib.h>
#include <stdbool.h>

#define TOKENS(...) int tokenizer__generate_config(token_config_t *config) { \
    size_t i; \
    for (i = 0; i <= __tok_amount; i++) { \
        config->check_functions[i] = NULL; \
    } \
    __VA_ARGS__ \
}

#define TOKEN(name, ...) \
    do { \
        bool is_tok_##name(size_t pos, char ch) {__VA_ARGS__} \
        config->check_functions[tok_##name] = is_tok_##name; \
    } while (0);

/**
 * Token Types enumerable
 *
 * @author mishavetl
 *
 * ** register here your tokens **
 *
 * @member tok_num (number)
 * @member tok_del (delimiter)
 * @member tok_prior_start (priority start character)
 * @member tok_prior_end (priority end character)
 * @member tok_atom (atom)
 *
 */
enum token_type {
    tok_num,
    tok_prior_start,
    tok_prior_end,
    tok_del,
    tok_atom,
    __tok_amount
};

typedef bool (*token_check_function_t)(size_t, char);

/**
 * Token Config structure
 *
 * @author mishavetl
 *
 * @member check_functions (an array of token check functions)
 *
 */
typedef struct token_config {
    token_check_function_t check_functions[__tok_amount + 1];
} token_config_t;

/**
 * Generates token config
 *
 * @author mishavetl
 *
 * @param token_config_t *config (a pointer to a clean instance of token_config_t)
 *
 * @ret (status of the generation -1 for error, 0 for success)
 *
 */
int tokenizer__generate_config(token_config_t *config);

#endif
