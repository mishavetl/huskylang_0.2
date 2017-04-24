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
#include "query.h"
#include "call_tree.h"

int main(int argc, char *argv[])
{
    call_tree_t tree = call_tree__init();
    token_t **tokens = NULL;
    token_config_t token_config;
    scope_t scope = scope__init();
    data_t ret;
    int line;
    int line_saved;
    int status;

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

    gc_t gc_scope = gc_init();

    scope.gc = &gc_scope;

    checkf(tokenizer__generate_config(&token_config) >= 0,
        "Token config generation failed."
    );

    if (interactive) {
        puts("Not implemented.");
        exit(EXIT_FAILURE);
    } else {
        checkf((f = fopen(script_path, "rb")), "Error opening file.");
    }

    get_stdlib_variables(&scope);

    /* Interpretation process. */

    for (line = 1, line_saved = 1; ;) {
        status = get_query(interactive, &line, &buffer, &size, f);
        if (status == -1) {
            break;
        }

        // debug("'%s'", buffer);

        if (strlen(buffer) > 1) {
            tokens = tokenizer__string(&token_config, buffer, &line_saved);
            checkf(tokens, "Tokenization failed.");

            if (tokens[0]) {
                checkf(parser__funcall(&tree, tokens) >= 0,
                    "Function call parsing failed."
                );
                performer__execute(&tree, &scope, &ret);

                if (scope.error) {
                    fprintf(stderr,
                        "Traceback %s: '%s' at token '%s' on line %ld-%ld, column %ld\n",
                        scope.error->name, scope.error->msg,
                        scope.error->token->value,
                        scope.error->token->linefrom,
                        scope.error->token->lineto,
                        scope.error->token->col + 1
                    );
                    scope.error = NULL;
                }
            } else {
                line_saved++;
            }

            clean(&tree, tokens);
        } else {
            line_saved++;
        }

        if (status == EOF) {
            break;
        }
    }

    FREE(buffer);

    if (f) fclose(f);
    gc_clean(scope.gc);
    FREE(scope.vars);

    exit(EXIT_SUCCESS);

    error:

    FREE(buffer);

    if (f) fclose(f);
    gc_clean(scope.gc);
    FREE(scope.vars);
    clean(&tree, tokens);

    exit(EXIT_FAILURE);
}
