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
#define FREE(ptr) if (ptr) free(ptr);


#endif
