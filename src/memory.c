#include <stdlib.h>
#include "memory.h"

size_t count_2d(void **ar)
{
    size_t len;

    for (len = 0; ar[len]; len++);

    return len;
}
