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
    } else if (token->type == tok_string) {
        type->value.string = token->value;
        type->type = tid_string;
    } else {
        sentinel("type error: 'error creating type' on col: %ld, token: '%s'.", token->col, token->value);
    }

    return 0;

    error:

    return -1;
}

int typename_to_tid(const char *typename)
{
    if (strcmp(typename, "atom") == 0) {
        return tid_atom;
    } else if (strcmp(typename, "string") == 0) {
        return tid_string;
    } else if (strcmp(typename, "num") == 0) {
        return tid_num;
    } else if (strcmp(typename, "tuple") == 0) {
        return tid_tuple;
    } else if (strcmp(typename, "list") == 0) {
        return tid_list;
    } else {
        return -1;
    }
}
