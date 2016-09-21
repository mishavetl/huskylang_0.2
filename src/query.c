#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "query.h"
#include "dbg.h"
#include "memory.h"

int stack_push(char **stack, char c, size_t *size, ssize_t *last)
{
    if ((ssize_t) *size <= *last + 1) {
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

int stack_pop(char **stack, ssize_t *last)
{
    if (*last < 0) {
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
    size_t stack_size = 0, i;
    ssize_t stack_last = -1;

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

        if (stack && stack_last < (ssize_t) stack_size && stack_last >= 0) {
            if (stack[stack_last] == '\'') {
                if (c == '\'') {
                    stack_pop(&stack, &stack_last);
                }
            } else {
                goto not_stack;
            }
        } else {
            not_stack:

            switch (c) {
                case '\'':
                    stack_push(&stack, c, &stack_size, &stack_last);
                    break;

                case '(':
                    stack_push(&stack, c, &stack_size, &stack_last);
                    break;

                case ')':
                    if (stack[stack_last] != '(') {
                        sentinel("syntaxErr at %d:%ld", *line, i);
                    }

                    stack_pop(&stack, &stack_last);
                    break;

                case ';':
                    stack_push(&stack, c, &stack_size, &stack_last);
                    break;

                case '.':
                    if (stack[stack_last] != ';') {
                        sentinel("syntaxErr at %d:%ld", *line, i);
                    }

                    stack_pop(&stack, &stack_last);
                    break;
            }

            if (c == '\n') {
                *line += 1;

                if (prev == '\\') {
                    (*buffer)[i] = '\0';
                    (*buffer)[--i] = '\n';
                } else if (stack_last == -1) {
                    break;
                }
            }

            prev = c;
        }
    }

    if (stack_last != -1) {
        sentinel("syntaxErr at %d:%ld", *line, i);
        goto error;
    }

    FREE(stack);

    return ret;

error:
    FREE(stack);

    return -1;
}
