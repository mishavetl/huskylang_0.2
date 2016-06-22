#include <stdio.h>
#include "inc/tokenizer.h"

inline void print_tokens(token_t **tokens)
{
    size_t i;
    for (i = 0; tokens[i]; i++) {
        printf("type: %d %d-%d:%d %s\n",
            tokens[i]->type,
            tokens[i]->linefrom,
            tokens[i]->lineto,
            tokens[i]->col,
            tokens[i]->value
        );
    }
}

int main(void)
{
    token_t **tokens;
    token_config_t token_config;

    tokenizer__generate_config(&token_config);
    tokens = tokenizer__string(&token_config, "1,2,3,foo:bar");

    print_tokens(tokens);

    tokenizer__clean_tokens(tokens);
}
