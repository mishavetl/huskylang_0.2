#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "tokenizer_config.h"

#define DELIMITERS ",()"
#define SPACES " \t\n"

TOKENS(
    TOKEN(num,
        return isdigit(ch);
    );
    TOKEN(prior_start,
        return pos == 0 && ch == '(';
    );
    TOKEN(prior_end,
        return pos == 0 && ch == ')';
    );
    TOKEN(del,
        return pos == 0 && index(DELIMITERS, ch);
    );
    TOKEN(atom,
        return !index(DELIMITERS, ch) && !index(SPACES, ch);
    );

    return 0;
)
