#ifndef ALGOSKETCH_LEXER_H
#define ALGOSKETCH_LEXER_H

extern int yylineno;
extern char *yytext;

void yyrestart(FILE *input_file);

#endif
