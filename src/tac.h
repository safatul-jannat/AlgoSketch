#ifndef ALGOSKETCH_TAC_H
#define ALGOSKETCH_TAC_H

#include <stddef.h>

#include "ast.h"

typedef enum
{
    TAC_COPY = 0,
    TAC_CONSTANT,
    TAC_BINARY,
    TAC_UNARY,
    TAC_LABEL,
    TAC_GOTO,
    TAC_IF_FALSE,
    TAC_IF_TRUE,
    TAC_PARAM,
    TAC_CALL,
    TAC_RETURN,
    TAC_PRINT
} TacOp;

typedef struct TacInst
{
    TacOp op;
    int line;
    Type type;
    char *result;
    char *arg1;
    char *arg2;
    char *binop;
    char *label;
} TacInst;

typedef struct TacList
{
    TacInst **items;
    size_t size;
    size_t capacity;
} TacList;

typedef struct TacVarDecl
{
    char *name;
    Type type;
} TacVarDecl;

typedef struct TacVarList
{
    TacVarDecl **items;
    size_t size;
    size_t capacity;
} TacVarList;

typedef struct TacFunction
{
    char *name;
    int arity;
    char **param_names;
    Type *param_types;
    Type return_type;
    TacVarList locals;
    TacList body;
} TacFunction;

typedef struct TacProgram
{
    TacFunction **functions;
    size_t count;
    size_t capacity;
    TacFunction *main;
} TacProgram;

TacProgram *tac_generate(AstNode *program);

void tac_print(TacProgram *prog);
void tac_free(TacProgram *prog);

#endif
