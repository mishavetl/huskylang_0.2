#include <stdlib.h>
#include "memory.h"
#include "dbg.h"

size_t count_2d(void **ar)
{
    size_t len;

    for (len = 0; ar[len]; len++);

    return len;
}

inline gc_t gc_init()
{
    return (gc_t) {
        .ptrs = NULL,
        .size = 0
    };
}

void gc_clean(gc_t *gc)
{
    long long i;

    if (gc->ptrs) {
        for (i = 0; i < gc->size; i++) {
            FREE(gc->ptrs[i]);
        }
        
        FREE(gc->ptrs);
    }
}

void *gc_add(gc_t *gc, void *ptr)
{
    gc->ptrs = realloc(gc->ptrs, sizeof(void *) * ++gc->size);

    return gc->ptrs[gc->size - 1] = ptr;
}
