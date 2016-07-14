#include <criterion/criterion.h>
#include "tokenizer_config.h"


Test(tokenizer, generate_config__generates_all_token_check_functions) {
    token_config_t token_config;
    size_t i;

    tokenizer__generate_config(&token_config);

    for (i = 0; i < __tok_amount; i++) {
      cr_assert(token_config.check_functions[i]);
    }
}
