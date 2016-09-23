#ifndef __argconfig_h__
#define __argconfig_h__

#include <stdio.h>
#include <config.h>

/* Program title. */
#define PROGRAM_TITLE "Husky Programming Language\n-- purely functional programming language with unpositioned lisp syntax"
/* Program bug sumbit address. */
#define PROGRAM_BUG_ADDRESS "<mishavetl@gmail.com>"
/* Program usage. */
#define PROGRAM_USAGE "Usage: %s [-r `path_to_script` | -i] [-v]"

/**
 * Print program usage.
 * @author mishavetl
 *
 * @param program_name (just an argv[0] string)
 *
 */
extern inline void print_usage(const char *program_name)
{
    printf(PROGRAM_TITLE "\n" PROGRAM_USAGE "\n", program_name);
}

#endif
