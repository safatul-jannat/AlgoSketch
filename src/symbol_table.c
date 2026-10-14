#include <stdlib.h>
#include <string.h>

#include "symbol_table.h"

static Symbol *symbol_new(const char *name, SymbolKind kind, Scope scope, int line)
{
    Symbol *sym = (Symbol *)calloc(1, sizeof(Symbol));
    if (!sym)
        return NULL;
    sym->name = strdup(name);
    sym->kind = kind;
    sym->scope = scope;
    sym->type = TYPE_UNKNOWN;
    sym->line = line;
    sym->arity = 0;
    sym->param_types = NULL;
    sym->return_type = TYPE_UNKNOWN;
    sym->func_node = NULL;
    return sym;
}

static void free_symbol(Symbol *sym)
{
    if (sym->name)
        free(sym->name);
    if (sym->param_types)
        free(sym->param_types);
    free(sym);
}

void symbol_table_init(SymbolTable *table)
{
    table->symbols = NULL;
    table->size = 0;
    table->capacity = 0;

    table->functions = NULL;
    table->func_count = 0;
    table->func_capacity = 0;
}

void symbol_table_free(SymbolTable *table)
{
    size_t i;
    for (i = 0; i < table->size; i++)
        free_symbol(table->symbols[i]);
    free(table->symbols);
    table->symbols = NULL;
    table->size = 0;
    table->capacity = 0;

    for (i = 0; i < table->func_count; i++)
    {
        size_t j;
        FunctionScope *fs = table->functions[i];
        for (j = 0; j < fs->size; j++)
            free_symbol(fs->symbols[j]);
        free(fs->symbols);
        free(fs->func_name);
        free(fs);
    }
    free(table->functions);
    table->functions = NULL;
    table->func_count = 0;
    table->func_capacity = 0;
}

Symbol *symbol_add_program(SymbolTable *table, const char *name, SymbolKind kind,
                           int line)
{
    Symbol *sym;

    if (table->size >= table->capacity)
    {
        size_t newcap = table->capacity ? table->capacity * 2 : 16;
        Symbol **newarr = (Symbol **)realloc(table->symbols, newcap * sizeof(Symbol *));
        if (!newarr)
            return NULL;
        table->symbols = newarr;
        table->capacity = newcap;
    }

    sym = symbol_new(name, kind, SCOPE_PROGRAM, line);
    if (!sym)
        return NULL;

    table->symbols[table->size++] = sym;
    return sym;
}

static FunctionScope *find_function_scope(const SymbolTable *table,
                                          const char *func_name)
{
    size_t i;
    for (i = 0; i < table->func_count; i++)
        if (strcmp(table->functions[i]->func_name, func_name) == 0)
            return table->functions[i];
    return NULL;
}

Symbol *symbol_add_function_scope(SymbolTable *table, const char *func_name,
                                  const char *name, SymbolKind kind, int line)
{
    FunctionScope *fs = find_function_scope(table, func_name);
    Symbol *sym;

    if (!fs)
    {
        if (table->func_count >= table->func_capacity)
        {
            size_t newcap = table->func_capacity ? table->func_capacity * 2 : 8;
            FunctionScope **newarr = (FunctionScope **)realloc(table->functions,
                newcap * sizeof(FunctionScope *));
            if (!newarr)
                return NULL;
            table->functions = newarr;
            table->func_capacity = newcap;
        }
        fs = (FunctionScope *)calloc(1, sizeof(FunctionScope));
        if (!fs)
            return NULL;
        fs->func_name = strdup(func_name);
        fs->symbols = NULL;
        fs->size = 0;
        fs->capacity = 0;
        table->functions[table->func_count++] = fs;
    }

    if (fs->size >= fs->capacity)
    {
        size_t newcap = fs->capacity ? fs->capacity * 2 : 8;
        Symbol **newarr = (Symbol **)realloc(fs->symbols, newcap * sizeof(Symbol *));
        if (!newarr)
            return NULL;
        fs->symbols = newarr;
        fs->capacity = newcap;
    }

    sym = symbol_new(name, kind, SCOPE_FUNCTION, line);
    if (!sym)
        return NULL;

    fs->symbols[fs->size++] = sym;
    return sym;
}

Symbol *symbol_lookup_program(SymbolTable *table, const char *name)
{
    size_t i;
    for (i = 0; i < table->size; i++)
        if (strcmp(table->symbols[i]->name, name) == 0)
            return table->symbols[i];
    return NULL;
}

Symbol *symbol_lookup_in_scope(const SymbolTable *table, const char *func_name,
                               const char *name, SymbolKind kind)
{
    size_t i;
    const FunctionScope *fs = NULL;

    for (i = 0; i < table->func_count; i++)
        if (strcmp(table->functions[i]->func_name, func_name) == 0)
        {
            fs = table->functions[i];
            break;
        }

    if (!fs)
        return NULL;

    for (i = 0; i < fs->size; i++)
        if (strcmp(fs->symbols[i]->name, name) == 0 &&
            fs->symbols[i]->kind == kind)
            return fs->symbols[i];
    return NULL;
}

size_t symbol_count(SymbolTable *table)
{
    return table->size;
}

Symbol *symbol_at(SymbolTable *table, size_t idx)
{
    if (idx >= table->size)
        return NULL;
    return table->symbols[idx];
}

size_t symbol_scope_count(const SymbolTable *table, const char *func_name)
{
    size_t i;
    for (i = 0; i < table->func_count; i++)
        if (strcmp(table->functions[i]->func_name, func_name) == 0)
            return table->functions[i]->size;
    return 0;
}

Symbol *symbol_scope_at(const SymbolTable *table, const char *func_name, size_t idx)
{
    size_t i;
    for (i = 0; i < table->func_count; i++)
        if (strcmp(table->functions[i]->func_name, func_name) == 0)
        {
            if (idx >= table->functions[i]->size)
                return NULL;
            return table->functions[i]->symbols[idx];
        }
    return NULL;
}