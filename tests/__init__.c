#include <stdio.h>
#include <criterion/criterion.h>
#include <criterion/hooks.h>
#include "tokenizer_config.h"

ReportHook(POST_ALL)(struct criterion_global_stats *stats) {
    int asserts_failed = stats->asserts_failed;
    printf("[\e[0;34m====\e[0m] \e[1mAsserts:\e[0m (passed: \e[0;32m%d\e[0m, failed: %s%d%s)\n",
        (int) stats->asserts_passed,
        asserts_failed == 0 ? "" : "\e[0;31m",
        (int) asserts_failed,
        asserts_failed == 0 ? "" : "\e[0m"
    );
}
