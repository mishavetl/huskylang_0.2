#ifndef MEMORY_H
#define MEMORY_H

/**
 * MACROS Free ptr if it is not NULL
 *
 * @author mishavetl
 *
 * @param ptr (just a pointer)
 *
 */
#define FREE(ptr) if (ptr) { free(ptr); ptr = NULL; }

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
