#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "query.h"
#include "dbg.h"
#include "memory.h"

int stack_push(char **stack, char c, size_t *size, size_t *last)
{
    if (*size <= *last + 1) {
        *size += 2;
        *stack = realloc(*stack, sizeof(char) * (*size));
        check_mem(*stack);
    }

    *last += 1;
    (*stack)[*last] = c;

    return 0;

error:
    return -1;
}

int stack_pop(char **stack, size_t *last)
{
    if (*last <= 0) {
        return -1;
    }

    *last -= 1;

    return (*stack)[*last + 1];
}

int get_query(int interactive, int *line, char **buffer, size_t *size, FILE *f)
{
    int ret = 0;
    int c, prev = '\0';
    char *stack = NULL;
    size_t stack_size = 0, stack_last = 0, i;

    for (i = 0; ; i++) {
        if (interactive) {
            goto error;
        } else {
            if ((c = fgetc(f)) == EOF) {\
                ret = EOF;
                break;
            }
        }

        if (c == '\r') continue;

        if (*size <= i + 1) {
            *size += 128;
            *buffer = realloc(*buffer, sizeof(char) * (*size));
        }

        (*buffer)[i] = c;
        (*buffer)[i + 1] = '\0';

        if (stack && stack[stack_last] == '\'') {
            if (c == '\'') {
                stack_pop(&stack, &stack_last);
            }
        } else {
            switch (c) {
                case '\'':
                    stack_push(&stack, c, &stack_size, &stack_last);
                    break;

                case '(':
                    stack_push(&stack, c, &stack_size, &stack_last);
                    break;

                case ')':
                    if (stack[stack_last] != '(') {
                        sentinel("syntaxErr at %ld:%ld", *line, i);
                    }

                    stack_pop(&stack, &stack_last);
                    break;

                case ';':
                    stack_push(&stack, c, &stack_size, &stack_last);
                    break;

                case '.':
                    if (stack[stack_last] != ';') {
                        sentinel("syntaxErr at %ld:%ld", *line, i);
                    }

                    stack_pop(&stack, &stack_last);
                    break;
            }

            if (c == '\n') {
                *line++;

                if (prev == '\\') {
                    (*buffer)[i] = '\0';
                    (*buffer)[--i] = '\n';
                } else if (stack_last == 0) {
                    break;
                }
            }

            prev = c;
        }
    }

    if (stack_last != 0) {
        sentinel("syntaxErr at %ld:%ld", *line, i);
        goto error;
    }

    FREE(stack);

    return ret;

error:
    FREE(stack);

    return -1;
}
