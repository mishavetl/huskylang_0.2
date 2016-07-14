#include <stdio.h>
#include <argp.h>

#include "tokenizer.h"
#include "dbg.h"
#include "parser.h"
#include "memory.h"
#include "std.h"
#include "performer.h"
#include "main_utils.h"
#include "argconfig.h"

extern FILE *stdin;

gc_t *gc_global;

int main(int argc, char *argv[])
{
    struct arguments arguments;

    call_tree_t tree;
    token_t **tokens = NULL;
    token_config_t token_config;
    var_t **vars;
    type_t ret;
    int line;

    FILE *f = NULL;
    char *buffer;
    size_t size = 0;

    /* Parse arguments. */

    arguments.silent = 0;
    arguments.verbose = 0;
    arguments.interactive = 0;
    arguments.script_path = NULL;

    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    /* Init. */
    
    gc_t gc_global_heap = gc__init();
    gc_global = &gc_global_heap;

    tree.map = NULL;

    check(tokenizer__generate_config(&token_config) >= 0, "Token config generation failed.");

    if (arguments.interactive) {
        puts("Not implemented.");
	goto error;
    } else {
	check((f = fopen(arguments.script_path, "rb")), "Error opening file.");
    }

    /* Interpretation process. */

    for (line = 1; ; line++) {
	if (arguments.interactive) {
	    goto error;
	} else {
	    if (getline(&buffer, &size, f) == -1) {
		break;
	    }
	}

	if (strlen(buffer) > 1) {
	    check((tokens = tokenizer__string(&token_config, buffer, line)), "Tokenization failed.");
	    check(parser__funcall(&tree, tokens) >= 0, "Function call parsing failed.");

	    check((vars = get_stdlib_variables()), "Failed to get stdlib variables.");
	    check(performer__execute(&tree, vars, &ret) >= 0, "Failed to perform execution of the tree.");

	    clean(&tree, tokens);
	}
	/* printf("ret: %s\n", ret.value.atom); */
    }

    FREE(buffer);

    if (f) fclose(f);
    gc__clean(gc_global);

    exit(EXIT_SUCCESS);

    error:

    if (f) fclose(f);

    gc__clean(gc_global);
    clean(&tree, tokens);
    exit(EXIT_FAILURE);
}
