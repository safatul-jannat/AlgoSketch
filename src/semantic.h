#ifndef ALGOSKETCH_SEMANTIC_H
#define ALGOSKETCH_SEMANTIC_H

#include "ast.h"
#include "symbol_table.h"

typedef struct SemanticContext
{
    SymbolTable table;
    int in_function;
    AstNode *current_func;
    const char *current_func_name;
} SemanticContext;

int semantic_analyze(AstNode *program);

void semantic_print_symbols(SemanticContext *ctx);

#endif