#ifndef __memory_h__
#define __memory_h__

#include <stdlib.h>
#include "../src/list/src/list.h"

/**
 * MACRO Free ptr if it is not NULL
 *
 * @author mishavetl
 *
 * @param ptr (just a pointer)
 *
 */
#define FREE(ptr) if (ptr) { free(ptr); ptr = NULL; }

/**
 * STRUCT Garbage Collector
 *
 * @author mishavetl
 *
 * @member ptrs (pointers to the memory blocks to clean)
 * @member size (size of an array ptrs)
 *
 */
typedef struct garbage_collector {
    void **ptrs;
    list_t **lists;
    size_t size;
    size_t lists_size;
} gc_t;

/**
 * Initiates garbage collector struct
 *
 * @param gc (garbage collector struct ptr)
 *
 */
extern inline gc_t gc_init();

/**
 * Collects garbage_collector memory
 *
 * @author mishavetl
 *
 * @param gc (garbage collector struct ptr)
 *
 */
void gc_clean(gc_t *gc);

/**
 * Adds memory to the garbage collector
 *
 * @author mishavetl
 *
 * @param gc (garbage collector struct ptr)
 * @param ptr (ptr to add to gc)
 *
 * @return added ptr
 *
 */
void *gc_add(gc_t *gc, void *ptr);

/**
 * Adds list memory to the garbage collector
 *
 * @author mishavetl
 *
 * @param gc (garbage collector struct ptr)
 * @param ptr (ptr to add to gc)
 *
 * @return added ptr
 *
 */
list_t *gc_add_list(gc_t *gc, list_t *ptr);

/**
 * Count 2d array length
 *
 * @author mishavetl
 *
 * @param ar (2d array)
 *
 * @ret (length of 2d array)
 *
 */
size_t count_2d(void **ar);

#endif
