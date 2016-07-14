#ifndef __argconfig_h__
#define __argconfig_h__

#include <argp.h>

const char *argp_program_version =
    "husky 0.0.1-alpha-01 ; hsk-std 0.0.1-alpha-01";
const char *argp_program_bug_address =
    "<mishavetl@gmail.com>";

/* Program documentation. */
static char doc[] =
    "Husky Programming Language -- purely functional programming language with nelisp syntax style";

/* A description of the arguments we accept. */
static char args_doc[] = "SCRIPT_FILE_PATH";

/* The options we understand. */
static struct argp_option options[] = {
    {"verbose", 'v', 0, 0, "Produce verbose output"},
    {"quiet", 'q', 0, 0, "Don't produce any output"},
    {"interactive", 'i', 0, 0, "Launch interactive shell"},
    {0}
};

/* Used by main to communicate with parse_opt. */
struct arguments {
    char *script_path;
    int silent, verbose, interactive;
};

/* Parse a single option. */
static error_t parse_opt (int key, char *arg, struct argp_state *state)
{
    struct arguments *arguments = state->input;

    switch (key) {
	case 'q':
	    arguments->silent = 1;
	    break;
	case 'v':
	    arguments->verbose = 1;
	    break;
	case 'i':
	    arguments->interactive = 1;
	    break;

	case ARGP_KEY_ARG:
	    if (state->arg_num > 1) argp_usage(state);
	    arguments->script_path = arg;
	    break;

	case ARGP_KEY_END:
	    if (state->arg_num < 1 && !arguments->interactive)
		argp_usage(state);
	    
	    break;

	default:
	    return ARGP_ERR_UNKNOWN;
    }

    return 0;
}

/* Our argp parser. */
static struct argp argp = {options, parse_opt, args_doc, doc};

#endif
