#include <math.h>
#include "dbg.h"
#include "data.h"
#include "memory.h"
#include "tokenizer.h"
#include "tokenizer_config.h"

struct type *construct_type(enum tid single, struct type **multiple, gc_t *gc)
{
    return construct_type_sized(single, multiple, count_2d((void **) multiple), gc);
}

struct type *construct_type_sized(enum tid single, struct type **multiple, int multsize, gc_t *gc)
{
    struct type *type = (struct type *) gc_add(gc, malloc(sizeof(struct type)));
    type->single = single;
    type->multiple = multiple;
    type->multsize = multsize;
    return type;
}

/** @todo */
int data_from_token(token_t *token, data_t *data, gc_t *gc)
{
    check_mem(token);
    check_mem(data);

    if (token->type == tok_num) {
        REAL_TYPE n = atof(token->value);
        if ((INTEGRAL_TYPE) n == n) {
            data->type = construct_type(tid_integral, NULL, gc);
            data->value.integral = n;
        } else {
            data->type = construct_type(tid_real, NULL, gc);
            data->value.real = n;
        }
    } else if (token->type == tok_atom) {
        data->value.atom = token->value;
        if (strcmp(token->value, "_alpha") == 0) 
            data->type = construct_type(tid_alpha, NULL, gc);
        else
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
    return (struct type) {__tid_size, NULL, 0};
}

int types_identical(struct type *type1, struct type *type2)
{
    if (!type1 && !type2) return true;
    if (!type1 || !type2) return false;
    if (type1->single == tid_alpha || type2->single == tid_alpha)
        return true;

    int i, j;
    struct type **mult1 = type1->multiple;
    struct type **mult2 = type2->multiple;    
    
    if (type1->single != type2->single) return false;
    if (!mult1 != !mult2) return false;
    else if (!mult1 || !mult2) return true;
    for (i = 0, j = 0; mult1[i] && mult2[j]; ++i, ++j) {
        if (!types_identical(mult1[i], mult2[j])) return false;
        if (type1->multsize != INFINITY || type2->multsize != INFINITY) {
            if (type1->multsize == INFINITY && !mult1[i + 1]) i = -1;
            if (type2->multsize == INFINITY && !mult2[j + 1]) j = -1;
        }
    }
    // if (mult1[i] != mult2[j]) return false;
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

struct type *typedup(struct type *type, gc_t *gc)
{
    return construct_type(
        type->single,
        ((type->multiple)
            ? initializer_type_to_array(type->multiple, type->multsize, gc)
            : NULL
        ),
        gc
    );
}