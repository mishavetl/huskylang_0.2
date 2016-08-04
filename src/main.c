#include <stdio.h>
#include <unistd.h>
#include <getopt.h>

#include "tokenizer.h"
#include "dbg.h"
#include "parser.h"
#include "memory.h"
#include "std.h"
#include "performer.h"
#include "main_utils.h"
#include "argconfig.h"
#include "non_posix.h"

gc_t *gc_global;

int main(int argc, char *argv[])
{
    call_tree_t tree;
    token_t **tokens = NULL;
    token_config_t token_config;
    scope_t scope;
    type_t ret;
    int line;

    FILE *f = NULL;
    char *buffer = NULL;
    size_t size = 0;

    char c;

    /* Parse arguments. */

    char *script_path = NULL;
    int interactive = 0, version = 0;

    while ((c = getopt(argc, argv, "ivr:")) != -1) {
        switch (c) {
            case 'i':
                interactive = 1;
                break;
            case 'v':
                puts(PROGRAM_TITLE "\n" PROGRAM_VERSION);
                version = 1;
                break;
            case 'r':
                script_path = optarg;
                break;
            default:
                print_usage(argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    /* Check for correctness of input data. */

    if (script_path == NULL && !interactive && !version) {
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    } else if (script_path == NULL && !interactive && version) {
        exit(EXIT_SUCCESS);
    }

    /* Init. */

    gc_t gc_global_heap = gc_init();
    gc_t gc_scope = gc_init();

    scope.gc = &gc_scope;
    gc_global = &gc_global_heap;

    scope.vars = NULL;
    tree.map = NULL;

    check(tokenizer__generate_config(&token_config) >= 0, "Token config generation failed.");

    if (interactive) {
        puts("Not implemented.");
        exit(EXIT_FAILURE);
    } else {
        check((f = fopen(script_path, "rb")), "Error opening file.");
    }

    get_stdlib_variables(&scope);

    /* Interpretation process. */

    for (line = 1; ; line++) {
        if (interactive) {

        } else {
            if (getline(&buffer, &size, f) == -1) {
                break;
            }
        }

        if (strlen(buffer) > 1) {
            check((tokens = tokenizer__string(&token_config, buffer, line)), "Tokenization failed.");
            check(parser__funcall(&tree, tokens) >= 0, "Function call parsing failed.");
            performer__execute(&tree, &scope, &ret);

            if (ret.type == tid_atom) {
                if (strcmp(ret.value.atom, "bad") == 0) {
                    puts("Error");
                    goto error;
                }
            }

            clean(&tree, tokens);
        }
        /* printf("ret: %s\n", ret.value.atom); */
    }

    FREE(buffer);

    if (f) fclose(f);
    gc_clean(gc_global);
    gc_clean(scope.gc);
    FREE(scope.vars);

    exit(EXIT_SUCCESS);

    error:

    FREE(buffer);

    if (f) fclose(f);
    gc_clean(gc_global);
    gc_clean(scope.gc);
    FREE(scope.vars);
    clean(&tree, tokens);

    exit(EXIT_FAILURE);
}
