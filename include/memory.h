/**
 * @file
 * @brief Collection of functions for memory managment
 */
#ifndef __memory_h__
#define __memory_h__

#include <stdlib.h>
#include "../src/list/src/list.h"

typedef struct type type_t;
typedef struct scope scope_t;

/**
 * @brief Frees ptr if it is not NULL
 *
 * @param ptr Pointer
 */
#define FREE(ptr) if (ptr) { free(ptr); ptr = NULL; }

/**
 * @brief Garbage Collector
 */
typedef struct garbage_collector {
    /** Pointers to the memory blocks to clean */
    void **ptrs;
    /** Pointers to the memory blocks containing lists to clean */
    list_t **lists;
    /** Size of the `ptrs` */
    size_t size;
    /** Size of `lists` */
    size_t lists_size;
} gc_t;

/**
 * @brief` Initiates garbage collector struct
 *
 * @param[in] gc Garbage collector struct ptr
 *
 * @return Garbage Collector
 */
gc_t gc_init();

/**
 * @brief Frees `garbage_collector` memory
 *
 * @param[out] gc Garbage collector
 */
void gc_clean(gc_t *gc);

/**
 * @brief Adds memory to the garbage collector
 *
 * @param[in] gc Garbage collector struct ptr
 * @param[in] ptr Pointer to add to gc
 *
 * @return Added ptr
 */
void *gc_add(gc_t *gc, void *ptr);

/**
 * @brief Adds list memory to the garbage collector
 *
 * @param[out] gc Garbage collector struct ptr
 * @param[in] ptr Pointer to a list to add to gc
 *
 * @return Added ptr
 */
list_t *gc_add_list(gc_t *gc, list_t *ptr);

/**
 * @brief Counts 2d array length
 *
 * @param[in] ar 2d array
 *
 * @return Length of an array
 */
size_t count_2d(void **ar);

/**
 * @brief Duplicates a type
 *
 * @param[in] src Type to duplicate
 * @param[in] scope
 *
 * @return Duplicated type
 */
type_t *copy_type(type_t *src, scope_t *scope);

/**
 * @brief Duplicates string array
 * 
 * @param[in] src String array to duplicate
 * @param[in] scope
 *
 * @return Duplicated array
 */
char **str_array_dup(char **src, int n, gc_t *gc);

#endif
