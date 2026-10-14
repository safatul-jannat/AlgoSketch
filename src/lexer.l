%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "error.h"
#include "ast.h"
#include "lexer.h"

#undef BEGIN

#include "parser.h"

static char *duplicate_string(const char *s)
{
    size_t len = strlen(s);
    char *out = (char *)malloc(len + 1);
    if (out)
    {
        memcpy(out, s, len);
        out[len] = 0;
    }
    return out;
}

static char *unquote_string(const char *s)
{
    size_t len = strlen(s);
    char *out = (char *)malloc(len + 1);
    size_t i, j = 0;

    if (!out)
        return NULL;

    if (len >= 2 && s[0] == '"' && s[len - 1] == '"')
    {
        for (i = 1; i < len - 1; i++)
        {
            if (s[i] == '\\' && i + 1 < len - 1)
            {
                switch (s[i + 1])
                {
                    case 'n': out[j++] = '\n'; i++; continue;
                    case 't': out[j++] = '\t'; i++; continue;
                    case '"': out[j++] = '"'; i++; continue;
                    case '\\': out[j++] = '\\'; i++; continue;
                    default: break;
                }
            }
            out[j++] = s[i];
        }
    }
    out[j] = 0;
    return out;
}
%}

%option noyywrap
%option yylineno
%option nounput
%option noinput
%option nodefault

DIGIT      [0-9]
ID         [A-Za-z_][A-Za-z0-9_]*
NUMBER     [0-9]+(\.[0-9]+)?
STRING     \"([^"\\\n]|\\.)*\"

%%

[ \t\r\n]+          { /* ignore whitespace */ }

"var"               { return VAR; }
"if"                { return IF; }
"then"              { return THEN; }
"otherwise"         { return OTHERWISE; }
"while"             { return WHILE; }
"function"          { return FUNCTION; }
"begin"             { return BEGIN; }
"end"               { return END; }
"return"            { return RETURN; }
"display"           { return DISPLAY; }
"true"              { return TRUE; }
"false"             { return FALSE; }
"and"               { return AND; }
"or"                { return OR; }
"not"               { return NOT; }

"="                 { return ASSIGN; }
"+"                 { return PLUS; }
"-"                 { return MINUS; }
"*"                 { return MULTIPLY; }
"/"                 { return DIVIDE; }
"<"                 { return LT; }
">"                 { return GT; }
"<="                { return LE; }
">="                { return GE; }
"=="                { return EQ; }
"!="                { return NE; }
"("                 { return LPAREN; }
")"                 { return RPAREN; }
","                 { return COMMA; }
";"                 { return SEMICOLON; }

{NUMBER}            {
                        yylval.num = atof(yytext);
                        return NUMBER_LITERAL;
                    }

{STRING}            {
                        yylval.text = unquote_string(yytext);
                        return STRING_LITERAL;
                    }

{ID}                {
                        yylval.text = duplicate_string(yytext);
                        return ID;
                    }

.                   {
                        error_report(ERROR_LEXICAL, yylineno,
                            "invalid character '%.*s' (0x%02x)",
                            (int)(yytext[0] ? 1 : 0), yytext,
                            (unsigned char)yytext[0]);
                    }

%%
