#ifndef ALGOSKETCH_SYMBOL_TABLE_H
#define ALGOSKETCH_SYMBOL_TABLE_H

#include <stddef.h>

#include "ast.h"

typedef enum
{
    SCOPE_PROGRAM = 0,
    SCOPE_FUNCTION
} Scope;

typedef enum
{
    SYMBOL_VARIABLE = 0,
    SYMBOL_PARAMETER,
    SYMBOL_FUNCTION
} SymbolKind;

typedef struct Symbol
{
    char *name;
    SymbolKind kind;
    Scope scope;
    Type type;
    int line;

    int arity;
    Type *param_types;
    Type return_type;
    AstNode *func_node;
} Symbol;

/* One independent scope per user function. */
typedef struct FunctionScope
{
    char *func_name;
    Symbol **symbols;
    size_t size;
    size_t capacity;
} FunctionScope;

typedef struct SymbolTable
{
    Symbol **symbols;          /* program-scope symbols */
    size_t size;
    size_t capacity;

    FunctionScope **functions; /* a dedicated scope for each function */
    size_t func_count;
    size_t func_capacity;
} SymbolTable;

void symbol_table_init(SymbolTable *table);
void symbol_table_free(SymbolTable *table);

Symbol *symbol_add_program(SymbolTable *table, const char *name, SymbolKind kind,
                           int line);

Symbol *symbol_add_function_scope(SymbolTable *table, const char *func_name,
                                  const char *name, SymbolKind kind, int line);

Symbol *symbol_lookup_program(SymbolTable *table, const char *name);

/* Looks up `name` inside the function named `func_name` only. */
Symbol *symbol_lookup_in_scope(const SymbolTable *table, const char *func_name,
                               const char *name, SymbolKind kind);

size_t symbol_count(SymbolTable *table);
Symbol *symbol_at(SymbolTable *table, size_t idx);

size_t symbol_scope_count(const SymbolTable *table, const char *func_name);
Symbol *symbol_scope_at(const SymbolTable *table, const char *func_name, size_t idx);

#endif