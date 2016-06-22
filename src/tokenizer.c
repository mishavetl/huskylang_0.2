#include <stdlib.h>
#include <string.h>

#include "inc/tokenizer.h"
#include "inc/tokenizer_config.h"

token_t **tokenizer__string(const token_config_t *config, const char *str)
{
    token_t **tokens = NULL;
    token_t *token;
    size_t i, j, pos, line = 1, size = 1;

    for (i = 0; i < strlen(str); i++) {
        for (j = 0; config->check_functions[j]; j++) {
            if (config->check_functions[j](0, str[i])) {
                tokens = realloc(tokens, sizeof(token_t) * ++size);
                tokens[size - 2] = malloc(sizeof(token_t));

                token = tokens[size - 2];
                token->col = i;
                token->type = j;
                token->value = NULL;
                token->linefrom = line;

                for (pos = 0; config->check_functions[j](pos, str[i]); pos++, i++) {
                    token->value = realloc(token->value, sizeof(char) * (pos + 2));
                    token->value[pos] = '\0';
                    if (str[i] == '\\') {
                        if (str[i + 1] == 'n') {
                            line++;
                            token->value[pos] = '\n';
                            i++;
                        }

                        i++;
                    }

                    if (token->value[pos] == '\0') token->value[pos] = str[i];
                }

                token->value[pos] = '\0';
                token->lineto = line;
                i--;

                break;
            }
        }
    }

    tokens[size - 1] = NULL;

    return tokens;
}

void tokenizer__clean_tokens(token_t **tokens)
{
    size_t i;

    if (tokens) {
        for (i = 0; tokens[i]; i++) {
            if (tokens[i]) {
                if (tokens[i]->value) free(tokens[i]->value);
                free(tokens[i]);
            }
        }

        free(tokens);
    }
}
