#ifndef ALGOSKETCH_ERROR_H
#define ALGOSKETCH_ERROR_H

typedef enum
{
    ERROR_NONE = 0,
    ERROR_LEXICAL,
    ERROR_SYNTAX,
    ERROR_SEMANTIC,
    ERROR_INTERNAL
} ErrorType;

extern int error_count;
extern int fatal_error;

void error_report(ErrorType type, int line, const char *fmt, ...);
void error_set_quiet(int quiet);

const char *error_type_name(ErrorType type);

#endif
