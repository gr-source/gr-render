#include "gError.h"

#include <stdarg.h>
#include <stdio.h>

static char strerror[1024];

const char *grr::error() {
    return strerror;
}

void grr::error(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    vsnprintf(strerror, sizeof(strerror), fmt, args);

    va_end(args);
}

