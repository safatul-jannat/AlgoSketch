#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "error.h"

int error_count = 0;
int fatal_error = 0;

static int quiet = 0;

void error_set_quiet(int q)
{
    quiet = q;
}

const char *error_type_name(ErrorType type)
{
    switch (type)
    {
        case ERROR_NONE:      return "none";
        case ERROR_LEXICAL:   return "lexical";
        case ERROR_SYNTAX:    return "syntax";
        case ERROR_SEMANTIC:  return "semantic";
        case ERROR_INTERNAL:  return "internal";
    }
    return "unknown";
}

void error_report(ErrorType type, int line, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    error_count++;

    if (!quiet)
    {
        if (line > 0)
            fprintf(stderr, "AlgoSketch %s error (line %d): ", error_type_name(type), line);
        else
            fprintf(stderr, "AlgoSketch %s error: ", error_type_name(type));

        vfprintf(stderr, fmt, args);
        fprintf(stderr, "\n");
    }

    va_end(args);

    if (error_count >= 20)
    {
        fatal_error = 1;
    }
}
