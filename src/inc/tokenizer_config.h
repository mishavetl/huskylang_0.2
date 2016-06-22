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

enum token_type {
    tid_num,
    tid_del,
    tid_atom,
    __tid_amount
};

typedef bool (*token_check_function_t)(size_t, char);

typedef struct token_config {
    token_check_function_t check_functions[__tid_amount + 1];
} token_config_t;

int tokenizer__generate_config(token_config_t *config);

#endif
