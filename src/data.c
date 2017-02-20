#include "dbg.h"
#include "data.h"
#include "memory.h"
#include "tokenizer.h"
#include "tokenizer_config.h"

struct type *construct_type(enum tid single, struct type **multiple, gc_t *gc)
{
    struct type *type = (struct type *) gc_add(gc, malloc(sizeof(struct type)));
    type->single = single;
    type->multiple = multiple;
    return type;
}

/** @todo */
int data_from_token(token_t *token, data_t *data, gc_t *gc)
{
    check_mem(token);
    check_mem(data);

    if (token->type == tok_num) {
        data->value.integral = atof(token->value);
        data->type = construct_type(tid_integral, NULL, gc);
    } else if (token->type == tok_atom) {
        data->value.atom = token->value;
        data->type = construct_type(tid_atom, NULL, gc);
    } else if (token->type == tok_string) {
        data->value.string = token->value;
        data->type = construct_type(tid_string, NULL, gc);
    } else {
        sentinel("type error: 'error creating type' on col: %ld, token: '%s'.", token->col, token->value);
    }

    return 0;
error:
    return -1;
}

struct type init_type()
{
    return (struct type) {__tid_size, NULL};
}

int types_identical(struct type *type1, struct type *type2)
{
    if (!type1 && !type2) return true;
    if (!type1 || !type2) return false;
    int i;
    struct type **mult1 = type1->multiple;
    struct type **mult2 = type2->multiple;    
    if (type1->single != type2->single) return false;
    if (!type1->multiple != !type2->multiple) return false;
    else if (!type1->multiple || !type2->multiple) return true;
    for (i = 0; mult1[i] && mult2[i]; ++i) {
        if (!types_identical(mult1[i], mult2[i])) return false;
    }
    if (mult1[i] != mult2[i]) return false;
    return true;
}

int typename_to_tid(const char *typename)
{
    if (strcmp(typename, "atom") == 0) {
        return tid_atom;
    } else if (strcmp(typename, "string") == 0) {
        return tid_string;
    } else if (strcmp(typename, "num") == 0) {
        return tid_integral;
    } else if (strcmp(typename, "tuple") == 0) {
        return tid_tuple;
    } else if (strcmp(typename, "list") == 0) {
        return tid_list;
    } else {
        return -1;
    }
}

struct type **initializer_type_to_array(struct type *types[], int n, gc_t *gc)
{
    struct type **types_ = gc_add(gc, malloc(sizeof(struct type *) * (n + 1)));
    for (int i = 0; i < n; ++i) {
        types_[i] = gc_add(gc, malloc(sizeof(struct type)));
        memcpy(types_[i], types[i], sizeof(struct type));
    }
    types_[n] = NULL;
    return types_;
}