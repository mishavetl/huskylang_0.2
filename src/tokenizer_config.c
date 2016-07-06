#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "tokenizer_config.h"

#define DELIMITERS ","
#define SPACES " \t"

TOKENS
(
    TOKEN(tid_num,
        return isdigit(ch);
    );
    TOKEN(tid_del,
        return pos == 0 && index(DELIMITERS, ch);
    );
    TOKEN(tid_prior_start,
        return pos == 0 && ch == '(';
    );
    TOKEN(tid_prior_end,
        return pos == 0 && ch == ')';
    );
    TOKEN(tid_atom,
        return !index(DELIMITERS, ch) && !index(SPACES, ch);
    );

    return 0;
)
