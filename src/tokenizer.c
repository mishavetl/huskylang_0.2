#include <stdlib.h>
#include <string.h>
#include "tokenizer.h"
#include "tokenizer_config.h"
#include "dbg.h"
#include "memory.h"

token_t **tokenizer__string(const token_config_t *config, const char *str, int *line)
{
    token_t **tokens = NULL;
    token_t *token;
    size_t i, j, pos, real_pos, size = 1, i_line_start = 0;
    int stat, started;

    for (i = 0; i < strlen(str); i++) {
        if (str[i] == '\n') {
            *line += 1;
            i_line_start = i + 1;
            continue;
        }

        if (str[i] == '-' && str[i + 1] == '-') {
            break;
        }

        for (j = 0; config->check_functions[j]; j++) {
            stat = config->check_functions[j](0, str[i], 0);
            if (stat) {
                tokens = realloc(tokens, sizeof(token_t *) * ++size);
                check_mem(tokens);
                tokens[size - 2] = malloc(sizeof(token_t));
                check_mem(tokens[size - 2]);

                token = tokens[size - 2];
                token->col = i - i_line_start;
                token->type = j;
                token->value = NULL;
                token->linefrom = *line;

                bool has_dot = false;

                for (
                    real_pos = 0, pos = 0, started = 0;
                    (stat = config->check_functions[j](pos, str[i], started));
                    pos++, i++
                ) {
                    if (str[i] == '\n') {
                        *line += 1;
                        i_line_start = i + 1;
                    }

                    if (str[i] == '.' && j == tok_num) {
                        if (has_dot) sentinel("syntaxErr at %d:%ld", *line, i - i_line_start + 1);
                        has_dot = true;
                    }

                    if (stat == TOKEN_CHAR_SKIP) {
                        started = 1;
                        continue;
                    } else if (stat == TOKEN_CHAR_SKIP_AND_END) {
                        i++;
                        break;
                    }

                    token->value = realloc(token->value, sizeof(char) * (real_pos + 2));
                    check_mem(token->value);
                    token->value[real_pos] = '\0';

                    if (str[i] == '\\') {
                        if (str[i + 1] == 'n') {
                            token->value[real_pos] = '\n';
                        } else if (str[i + 1] == 't') {
                            token->value[real_pos] = '\t';
                        }

                        i++;
                    }

                    if (token->value[real_pos] == '\0') token->value[real_pos] = str[i];
                    real_pos++;
                }

                if (j == tok_string && !token->value) {
                    token->value = malloc(sizeof(char) * 1);
                }

                token->value[real_pos] = '\0';
                token->lineto = *line;
                i--;

                break;
            }
        }
    }

    if (tokens) {
        tokens[size - 1] = NULL;
    } else {
        tokens = malloc(sizeof(token_t *));
        tokens[0] = NULL;
    }

    return tokens;

    error:
    return NULL;
}

void tokenizer__clean_tokens(token_t **tokens)
{
    size_t i;

    if (tokens) {
        for (i = 0; tokens[i]; i++) {
            if (tokens[i]) {
                FREE(tokens[i]->value);
                FREE(tokens[i]);
            }
        }

        FREE(tokens);
    }
}
