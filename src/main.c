#include <stdio.h>
#include "inc/tokenizer.h"
#include "inc/parser.h"

inline void print_tokens(token_t **tokens)
{
    size_t i;
    for (i = 0; tokens[i]; i++) {
        printf("type: %d %d-%d:%d %s\n",
            (int) tokens[i]->type,
            (int) tokens[i]->linefrom,
            (int) tokens[i]->lineto,
            (int) tokens[i]->col,
            tokens[i]->value
        );
    }
}

int main(void)
{
    token_t **tokens;
    token_config_t token_config;

    tokenizer__generate_config(&token_config);
    tokens = tokenizer__string(&token_config, "foo, 1 baz:buz bar, 2");

    // print_tokens(tokens);

    parser_element_node_t *head = parser__parse(tokens);
    parser_element_node_t *cur;
    parser_element_node_t *cur_val;
    for (cur = head; cur; cur = cur->next) {
        puts("[");
        for (cur_val = cur->val->data.mixed; cur_val; cur_val = cur_val->next) {
            printf("  %s\n", cur_val->val->data.token->value);
        }
        puts("]");
    }

    parser_element__cleanall(head);
    tokenizer__clean_tokens(tokens);
}
