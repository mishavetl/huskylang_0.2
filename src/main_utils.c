#include <stdio.h>
#include "main_utils.h"
#include "call_tree.h"
#include "memory.h"
#include "tokenizer.h"

void print_tree(const call_tree_t *tree, mapv_t i)
{
    mapv_t j, m;

    for (; !tree->map[i]; i++);

    printf("(%s ", tree->tokens[i]->value);

    for (j = 0; tree->map[i][j] != TERMINATE_MAPV; j++) {
        m = tree->map[i][j];

        if (m == EMPTY_MAPV) continue;
        if (tree->map[m]) print_tree(tree, m);
        else printf("%s", tree->tokens[m]->value);

        if (tree->map[i][j + 1] != TERMINATE_MAPV) printf(" ");
    }

    printf(")");
}

void clean(call_tree_t *tree, token_t **tokens)
{
    int i;

    if (tree) {
        if (tree->map) {
            for (i = 0; i < tree->size; i++) {
                FREE(tree->map[i]);
            }
            FREE(tree->map);
        }
    }

    tokenizer__clean_tokens(tokens);
}
