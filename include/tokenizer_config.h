#ifndef TOKENIZER_CONFIG_H
#define TOKENIZER_CONFIG_H

#include <stdbool.h>

#define TOKENS(...) int tokenizer__generate_config(token_config_t *config) { \
    size_t i; \
    for (i = 0; i <= __tid_amount; i++) { \
        config->check_functions[i] = NULL; \
    } \
    __VA_ARGS__ \
}

#define TOKEN(name, ...) \
    do { \
        bool is_##name(size_t pos, char ch) {__VA_ARGS__} \
        config->check_functions[name] = is_##name; \
    } while (0);

/**
 * Token Types enumerable
 *
 * @author mishavetl
 *
 * ** register here your tokens **
 *
 * @member tid_num (number)
 * @member tid_del (delimiter)
 * @member tid_prior_start (priority start character)
 * @member tid_prior_end (priority end character)
 * @member tid_atom (atom)
 *
 */
enum token_type {
    tid_num,
    tid_del,
    tid_prior_start,
    tid_prior_end,
    tid_atom,
    __tid_amount
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
    token_check_function_t check_functions[__tid_amount + 1];
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
