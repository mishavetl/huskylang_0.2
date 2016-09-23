/**
 * @file
 * @brief Collection of functions for query managment
 */
#ifndef __query_h__
#define __query_h__

#include <stdio.h>

/**
 * @brief Takes query from the file
 *
 * @param[in] interactive Is interactive interpretation
 * @param[out] line Query
 * @param[out] buffer Buffer for file reading
 * @param[out] size Size of `buffer`
 * @param[in] f File to read from
 *
 * @return Status: -1 if eof else 0
 */
int get_query(int interactive, int *line, char **buffer, size_t *size, FILE *f);

#endif
