#include <stdio.h>
#include <criterion/criterion.h>
#include <criterion/hooks.h>
#include "../src/inc/tokenizer_config.h"

token_config_t token_config;

ReportHook(PRE_ALL)(struct criterion_test_set *tests) {
    tokenizer__generate_config(&token_config);
}

ReportHook(POST_ALL)(struct criterion_global_stats *stats) {
    int asserts_failed = stats->asserts_failed;
    printf("[\e[0;34m====\e[0m] \e[1mAsserts:\e[0m (passed: \e[0;32m%d\e[0m, failed: %s%d%s)\n",
        stats->asserts_passed,
        asserts_failed == 0 ? "" : "\e[0;31m",
        asserts_failed,
        asserts_failed == 0 ? "" : "\e[0m"
    );
}
