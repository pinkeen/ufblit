#include "ufb_msgs.h"

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

static void __msg(FILE *out, const char *prefix, const char *format, va_list v)
{
    fprintf(out, "[UFBlit] %s: ", prefix);
    vfprintf(out, format, v);
    fprintf(out, "\n");
}

void __warn(const char *fmt, ...)
{
    va_list v;
    va_start(v, fmt);
    __msg(stderr, "WW", fmt, v);
    va_end(v);
}

void __info(const char *fmt, ...)
{
    va_list v;
    va_start(v, fmt);
    __msg(stdout, "II", fmt, v);
    va_end(v);
}

void __fatal(const char *fmt, ...)
{
    va_list v;
    va_start(v, fmt);
    __msg(stderr, "FATAL", fmt, v);
    va_end(v);
}

void __assert(const char *file, int line, const char *function, const char *expr, const char *fmt, ...)
{
    va_list args;
    char buf[512];

    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    fprintf(stderr, "[UFBlit] Assertion '%s' failed in %s:%d:%s() - %s\n", expr, file, line, function, buf);

    abort();
}
