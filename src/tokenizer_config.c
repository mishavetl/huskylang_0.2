#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "tokenizer_config.h"

#define DELIMITERS ",()."
#define SPACES " \t\n\r"

TOKEN(num,
    return isdigit(ch);
);

TOKEN(prior_start,
    return pos == 0 && ch == '(';
);

TOKEN(prior_end,
    return pos == 0 && (ch == ')' || ch == '.');
);

TOKEN(prior_start_saved,
    return pos == 0 && ch == ';';
);

TOKEN(del,
    return pos == 0 && strchr(DELIMITERS, ch);
);

TOKEN(string,
    if (pos == 0 && ch == '\'') {
        return TOKEN_CHAR_SKIP;
    } else if (pos != 0 && ch == '\'') {
        return TOKEN_CHAR_SKIP_AND_END;
    } else if (started) {
        return 1;
    } else {
        return 0;
    }
);

TOKEN(atom,
    if (pos == 0 && ch == '`') {
        return TOKEN_CHAR_SKIP;
    } else if (pos != 0 && ch == '`') {
        return TOKEN_CHAR_SKIP_AND_END;
    } else if (started) {
        return 1;
    } else {
        return !strchr(DELIMITERS, ch) && !strchr(SPACES, ch);
    }
);

TOKENS(
    REGTOKEN(num);
    REGTOKEN(prior_start);
    REGTOKEN(prior_end);
    REGTOKEN(prior_start_saved);
    REGTOKEN(del);
    REGTOKEN(string);
    REGTOKEN(atom);
)
