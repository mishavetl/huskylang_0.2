#include <stdio.h>
#include <stdlib.h>
#include "tokenizer.h"
#include "dbg.h"
#include "parser.h"
#include "memory.h"

void print_tree(const call_tree_t *tree, mapv_t i)
{
    mapv_t j, m;

    for (; !tree->map[i]; i++);

    printf("(%s%s%s ",
        tree->tokens[i]->value,
        tree->map[i][0] == EMPTY_MAPV ? "" : ":",
        tree->map[i][0] == EMPTY_MAPV ? "" : tree->tokens[tree->map[i][0]]->value);

    for (j = 1; tree->map[i][j] != TERMINATE_MAPV; j++) {
        m = tree->map[i][j];

        if (m == EMPTY_MAPV) continue;
        if (tree->map[m]) print_tree(tree, j);
        else printf("%s", tree->tokens[m]->value);

        if (tree->map[i][j + 1] != TERMINATE_MAPV) printf(" ");
    }

    printf(")\n");
}

void clean(call_tree_t *tree, token_t **tokens)
{
    int i;

    if (tree) {
        for (i = 0; i < tree->size; i++) {
            FREE(tree->map[i]);
        }
        FREE(tree->map);
    }

    tokenizer__clean_tokens(tokens);
}

int main(void)
{
    call_tree_t tree;
    token_t **tokens;
    token_config_t token_config;

    check(tokenizer__generate_config(&token_config) == 0, "Token config generation failed.");
    check((tokens = tokenizer__string(&token_config, "1 + 1")), "Tokenization failed.");
    check(parser__funcall(&tree, tokens, 0) == 0, "Function call parsing failed.");

    // tokens = tokenizer__string(&token_config, "1 * (1 * 2), (1 + (1 * 2)), foo");

    print_tree(&tree, 0);

    clean(&tree, tokens);
    exit(EXIT_SUCCESS);

    error:

    clean(&tree, tokens);
    exit(EXIT_FAILURE);
}
