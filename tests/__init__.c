#include <criterion/criterion.h>
#include "../src/inc/tokenizer_config.h"

token_config_t token_config;

ReportHook(PRE_ALL)(struct criterion_test_set *tests) {
    tokenizer__generate_config(&token_config);
}
