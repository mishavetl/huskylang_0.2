#include "dbg.h"
#include "type.h"
#include "tokenizer.h"
#include "tokenizer_config.h"

/** @todo */
int type_from_token(token_t *token, type_t *type)
{
    check_mem(token);
    check_mem(type);

    if (token->type == tok_num) {
        type->value.num = atof(token->value);
        type->type = tid_num;
    } else if (token->type == tok_atom) {
        type->value.atom = token->value;
        type->type = tid_atom;
    } else {
        sentinel("type error: 'error creating type' on col: %ld, token: '%s'.", token->col, token->value);
    }

    return 0;

    error:

    return -1;
}
