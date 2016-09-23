#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "huserr.h"
#include "variable.h"
#include "memory.h"
#include "dbg.h"

#define FORMAT "Traceback %s: '%s' at token '%s' on line %ld-%ld, column %ld"

char *huserr__to_string(scope_t *scope)
{
    char *buffer;

    check_mem(scope);
    check_mem(scope->error);

    buffer = gc_add(scope->gc, malloc(sizeof(char) * (
        strlen(FORMAT) - 15 /* number of special characters */
        + strlen(scope->error->name)
        + strlen(scope->error->msg)
        + strlen(scope->error->token->value)
        + (int) log10(scope->error->token->linefrom) + 1
        + (int) log10(scope->error->token->lineto) + 1
        + (int) log10(scope->error->token->col + 1) + 1
        + 1
    )));

    check_mem(buffer);

    sprintf(
        buffer,
        FORMAT,
        scope->error->name, scope->error->msg,
        scope->error->token->value,
        scope->error->token->linefrom,
        scope->error->token->lineto,
        scope->error->token->col + 1
    );

    return buffer;

    error:
    return NULL;
}
