#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "inc/tokenizer_config.h"

#define DELIMITERS ":,"
#define SPACE " \t"

TOKENS
(
    TOKEN(tid_num,
        return isdigit(ch);
    )
    TOKEN(tid_del,
        return pos == 0 && index(DELIMITERS, ch);
    )
    TOKEN(tid_atom,
        return !index(DELIMITERS, ch) && !index(SPACE, ch);
    )
)
