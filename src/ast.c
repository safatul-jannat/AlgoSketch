#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ast.h"

const char *type_name(Type t)
{
    switch (t)
    {
        case TYPE_UNKNOWN:   return "unknown";
        case TYPE_NUMBER:    return "number";
        case TYPE_TEXT:      return "text";
        case TYPE_BOOLEAN:   return "boolean";
        case TYPE_ERROR:     return "error";
    }
    return "?";
}

static const char *ast_kind_str(AstKind k)
{
    switch (k)
    {
        case AST_PROGRAM:         return "PROGRAM";
        case AST_FUNCTION_DECL:   return "FUNCTION_DECL";
        case AST_BLOCK:           return "BLOCK";
        case AST_VAR_DECL:        return "VAR_DECL";
        case AST_ASSIGNMENT:      return "ASSIGNMENT";
        case AST_DISPLAY:         return "DISPLAY";
        case AST_IF:              return "IF";
        case AST_WHILE:           return "WHILE";
        case AST_RETURN:          return "RETURN";
        case AST_FUNCTION_CALL:   return "FUNCTION_CALL";
        case AST_BINARY_EXPR:     return "BINARY_EXPR";
        case AST_UNARY_EXPR:      return "UNARY_EXPR";
        case AST_IDENTIFIER:      return "IDENTIFIER";
        case AST_NUMBER_LITERAL:  return "NUMBER_LITERAL";
        case AST_STRING_LITERAL:  return "STRING_LITERAL";
        case AST_BOOLEAN_LITERAL: return "BOOLEAN_LITERAL";
    }
    return "?";
}

AstNode *ast_new(AstKind kind, int line)
{
    AstNode *n = (AstNode *)calloc(1, sizeof(AstNode));
    if (!n)
        return NULL;
    n->kind = kind;
    n->line = line;
    n->inferred_type = TYPE_UNKNOWN;
    return n;
}

void ast_list_append(AstNodeList *list, AstNode *node)
{
    if (list->size >= list->capacity)
    {
        size_t newcap = list->capacity ? list->capacity * 2 : 4;
        AstNode **newitems = (AstNode **)realloc(list->items, newcap * sizeof(AstNode *));
        if (!newitems)
            return;
        list->items = newitems;
        list->capacity = newcap;
    }
    list->items[list->size++] = node;
}

void ast_free_list(AstNodeList *list)
{
    size_t i;
    for (i = 0; i < list->size; i++)
        ast_free(list->items[i]);
    free(list->items);
    list->items = NULL;
    list->size = 0;
    list->capacity = 0;
}

void ast_free(AstNode *node)
{
    if (!node)
        return;

    ast_free(node->left);
    ast_free(node->right);
    ast_free(node->cond);
    ast_free(node->then_block);
    ast_free(node->else_block);
    ast_free(node->return_expr);
    ast_free(node->value);
    ast_free_list(&node->params);
    ast_free_list(&node->args);

    free(node);
}

static void print_indent(int indent)
{
    int i;
    for (i = 0; i < indent; i++)
        printf("  ");
}

static void print_type(Type t)
{
    if (t != TYPE_UNKNOWN)
        printf("  ::%s", type_name(t));
}

void ast_print(AstNode *node, int indent)
{
    size_t i;
    if (!node)
        return;

    print_indent(indent);
    printf("[%s]", ast_kind_str(node->kind));

    switch (node->kind)
    {
        case AST_IDENTIFIER:
            printf(" '%s'", node->name ? node->name : "");
            print_type(node->inferred_type);
            break;
        case AST_NUMBER_LITERAL:
            printf(" %g", node->num_value);
            print_type(node->inferred_type);
            break;
        case AST_STRING_LITERAL:
            printf(" \"%s\"", node->str_value ? node->str_value : "");
            print_type(node->inferred_type);
            break;
        case AST_BOOLEAN_LITERAL:
            printf(" %s", node->bool_value ? "true" : "false");
            print_type(node->inferred_type);
            break;
        case AST_BINARY_EXPR:
            printf(" op=%s", node->op ? node->op : "?");
            print_type(node->inferred_type);
            break;
        case AST_UNARY_EXPR:
            printf(" op=%s", node->op ? node->op : "?");
            print_type(node->inferred_type);
            break;
        case AST_VAR_DECL:
            printf(" '%s'", node->name ? node->name : "");
            print_type(node->inferred_type);
            break;
        case AST_ASSIGNMENT:
            printf(" '%s'", node->name ? node->name : "");
            print_type(node->inferred_type);
            break;
        case AST_DISPLAY:
            print_type(node->inferred_type);
            break;
        case AST_IF:
        case AST_WHILE:
        case AST_RETURN:
            print_type(node->inferred_type);
            break;
        case AST_FUNCTION_CALL:
            printf(" '%s'", node->name ? node->name : "");
            print_type(node->inferred_type);
            break;
        case AST_FUNCTION_DECL:
            printf(" '%s'", node->name ? node->name : "");
            print_type(node->inferred_type);
            break;
        default:
            break;
    }

    printf("\n");

    if (node->cond)
        ast_print(node->cond, indent + 1);
    if (node->then_block)
        ast_print(node->then_block, indent + 1);
    if (node->else_block)
        ast_print(node->else_block, indent + 1);
    if (node->value && node->kind != AST_BINARY_EXPR && node->kind != AST_UNARY_EXPR)
        ast_print(node->value, indent + 1);
    if (node->left)
        ast_print(node->left, indent + 1);
    if (node->right)
        ast_print(node->right, indent + 1);
    if (node->return_expr)
        ast_print(node->return_expr, indent + 1);

    for (i = 0; i < node->params.size; i++)
        ast_print(node->params.items[i], indent + 1);
    for (i = 0; i < node->args.size; i++)
        ast_print(node->args.items[i], indent + 1);
}
