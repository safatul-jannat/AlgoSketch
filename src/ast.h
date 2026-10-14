#ifndef ALGOSKETCH_AST_H
#define ALGOSKETCH_AST_H

#include <stddef.h>

typedef enum
{
    TYPE_UNKNOWN = 0,
    TYPE_NUMBER,
    TYPE_TEXT,
    TYPE_BOOLEAN,
    TYPE_ERROR
} Type;

const char *type_name(Type t);

typedef enum
{
    AST_PROGRAM = 0,
    AST_FUNCTION_DECL,
    AST_BLOCK,
    AST_VAR_DECL,
    AST_ASSIGNMENT,
    AST_DISPLAY,
    AST_IF,
    AST_WHILE,
    AST_RETURN,
    AST_FUNCTION_CALL,
    AST_BINARY_EXPR,
    AST_UNARY_EXPR,
    AST_IDENTIFIER,
    AST_NUMBER_LITERAL,
    AST_STRING_LITERAL,
    AST_BOOLEAN_LITERAL
} AstKind;

typedef struct AstNode AstNode;

typedef struct AstNodeList
{
    AstNode **items;
    size_t size;
    size_t capacity;
} AstNodeList;

struct AstNode
{
    AstKind kind;
    int line;
    Type inferred_type;

    const char *name;       /* identifier / function name */
    char *op;               /* binary / unary operator string */

    AstNode *left;          /* binary lhs or general first child */
    AstNode *right;         /* binary rhs or general second child */
    AstNode *cond;          /* if / while condition */
    AstNode *then_block;    /* if / while body */
    AstNode *else_block;    /* if otherwise block (may be NULL) */
    AstNode *return_expr;   /* return expression */
    AstNode *value;         /* initializer / assignment value / display expr / call callee */

    AstNodeList params;     /* function parameters (var-decl-like nodes) */
    AstNodeList args;       /* function call arguments */

    double num_value;
    const char *str_value;
    int bool_value;
};

AstNode *ast_new(AstKind kind, int line);

void ast_list_append(AstNodeList *list, AstNode *node);

void ast_free(AstNode *node);
void ast_free_list(AstNodeList *list);

void ast_print(AstNode *node, int indent);

#endif
