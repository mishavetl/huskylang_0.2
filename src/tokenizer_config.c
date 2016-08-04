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
        return pos == 0 && strchr(DELIMITERS, ch);
    );
    TOKEN(atom,
        return !strchr(DELIMITERS, ch) && !strchr(SPACES, ch);
    );

    return 0;
)
