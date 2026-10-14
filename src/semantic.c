#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "semantic.h"
#include "error.h"

#define MAX_ITER 20

static int infer_expr(SemanticContext *ctx, AstNode *node);
static int infer_stmt(SemanticContext *ctx, AstNode *node);

static Symbol *lookup_variable(SemanticContext *ctx, const char *name)
{
    Symbol *s;
    if (ctx->in_function && ctx->current_func_name)
    {
        s = symbol_lookup_in_scope(&ctx->table, ctx->current_func_name,
                                   name, SYMBOL_PARAMETER);
        if (s)
            return s;
        return symbol_lookup_in_scope(&ctx->table, ctx->current_func_name,
                                      name, SYMBOL_VARIABLE);
    }
    return symbol_lookup_program(&ctx->table, name);
}

static Type unify_types(Type a, Type b)
{
    if (a == TYPE_UNKNOWN) return b;
    if (b == TYPE_UNKNOWN) return a;
    if (a == b) return a;
    return TYPE_ERROR;
}

/* Returns the effective operand type, constraining identifiers to `needed`.
   Returns TYPE_ERROR on confirmed mismatch. */
static Type type_of_operand(SemanticContext *ctx, AstNode *operand, Type needed)
{
    infer_expr(ctx, operand);

    if (operand->kind == AST_IDENTIFIER)
    {
        Symbol *sym = lookup_variable(ctx, operand->name);
        if (sym)
        {
            if (sym->type == TYPE_UNKNOWN)
            {
                sym->type = needed;
                operand->inferred_type = needed;
                return needed;
            }
            if (sym->type != needed)
                return TYPE_ERROR;
            return sym->type;
        }
    }
    return operand->inferred_type;
}

static int infer_binary(SemanticContext *ctx, AstNode *node)
{
    const char *op = node->op;
    Type a, b;

    if (strcmp(op, "and") == 0 || strcmp(op, "or") == 0)
    {
        a = type_of_operand(ctx, node->left, TYPE_BOOLEAN);
        b = type_of_operand(ctx, node->right, TYPE_BOOLEAN);
        if (a == TYPE_ERROR || b == TYPE_ERROR)
        {
            node->inferred_type = TYPE_ERROR;
            return 1;
        }
        node->inferred_type = (a == TYPE_UNKNOWN || b == TYPE_UNKNOWN)
            ? TYPE_UNKNOWN : TYPE_BOOLEAN;
        return 0;
    }

    if (strcmp(op, "<") == 0 || strcmp(op, ">") == 0 ||
        strcmp(op, "<=") == 0 || strcmp(op, ">=") == 0)
    {
        a = type_of_operand(ctx, node->left, TYPE_NUMBER);
        b = type_of_operand(ctx, node->right, TYPE_NUMBER);
        if (a == TYPE_ERROR || b == TYPE_ERROR ||
            a == TYPE_TEXT || b == TYPE_TEXT ||
            a == TYPE_BOOLEAN || b == TYPE_BOOLEAN)
        {
            node->inferred_type = TYPE_ERROR;
            return 1;
        }
        node->inferred_type = (a == TYPE_UNKNOWN || b == TYPE_UNKNOWN)
            ? TYPE_UNKNOWN : TYPE_BOOLEAN;
        return 0;
    }

    if (strcmp(op, "==") == 0 || strcmp(op, "!=") == 0)
    {
        Type lt, rt;
        infer_expr(ctx, node->left);
        infer_expr(ctx, node->right);
        lt = node->left->inferred_type;
        rt = node->right->inferred_type;
        if (lt != TYPE_UNKNOWN && rt != TYPE_UNKNOWN &&
            unify_types(lt, rt) == TYPE_ERROR)
        {
            node->inferred_type = TYPE_ERROR;
            return 1;
        }
        node->inferred_type = (lt == TYPE_UNKNOWN || rt == TYPE_UNKNOWN)
            ? TYPE_UNKNOWN : TYPE_BOOLEAN;
        return 0;
    }

    if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0 ||
        strcmp(op, "*") == 0 || strcmp(op, "/") == 0)
    {
        a = type_of_operand(ctx, node->left, TYPE_NUMBER);
        b = type_of_operand(ctx, node->right, TYPE_NUMBER);
        if (a == TYPE_ERROR || b == TYPE_ERROR ||
            a == TYPE_TEXT || b == TYPE_TEXT ||
            a == TYPE_BOOLEAN || b == TYPE_BOOLEAN)
        {
            node->inferred_type = TYPE_ERROR;
            return 1;
        }
        node->inferred_type = (a == TYPE_UNKNOWN || b == TYPE_UNKNOWN)
            ? TYPE_UNKNOWN : TYPE_NUMBER;
        return 0;
    }

    infer_expr(ctx, node->left);
    infer_expr(ctx, node->right);
    node->inferred_type = TYPE_ERROR;
    return 1;
}

static int infer_unary(SemanticContext *ctx, AstNode *node)
{
    const char *op = node->op;
    Type t;

    if (strcmp(op, "not") == 0)
    {
        t = type_of_operand(ctx, node->left, TYPE_BOOLEAN);
        if (t == TYPE_ERROR)
        {
            node->inferred_type = TYPE_ERROR;
            return 1;
        }
        node->inferred_type = (t == TYPE_UNKNOWN) ? TYPE_UNKNOWN : TYPE_BOOLEAN;
        return 0;
    }

    if (strcmp(op, "-") == 0)
    {
        t = type_of_operand(ctx, node->left, TYPE_NUMBER);
        if (t == TYPE_ERROR)
        {
            node->inferred_type = TYPE_ERROR;
            return 1;
        }
        node->inferred_type = (t == TYPE_UNKNOWN) ? TYPE_UNKNOWN : TYPE_NUMBER;
        return 0;
    }

    infer_expr(ctx, node->left);
    node->inferred_type = TYPE_ERROR;
    return 1;
}

static int infer_call(SemanticContext *ctx, AstNode *node)
{
    Symbol *fnsym = symbol_lookup_program(&ctx->table, node->name);
    size_t i;
    int changed = 0;

    if (!fnsym || fnsym->kind != SYMBOL_FUNCTION)
    {
        node->inferred_type = TYPE_ERROR;
        return 1;
    }

    for (i = 0; i < node->args.size; i++)
    {
        Type at;
        infer_expr(ctx, node->args.items[i]);
        at = node->args.items[i]->inferred_type;

        if ((int)i < fnsym->arity)
        {
            if (fnsym->param_types[i] == TYPE_UNKNOWN && at != TYPE_UNKNOWN)
            {
                fnsym->param_types[i] = at;
                changed = 1;
            }
            else if (fnsym->param_types[i] != TYPE_UNKNOWN &&
                     at != TYPE_UNKNOWN &&
                     fnsym->param_types[i] != at)
            {
                node->inferred_type = TYPE_ERROR;
            }
        }
    }

    node->inferred_type = fnsym->return_type;
    return changed;
}

static int infer_expr(SemanticContext *ctx, AstNode *node)
{
    if (!node)
        return 0;

    switch (node->kind)
    {
        case AST_NUMBER_LITERAL:
            node->inferred_type = TYPE_NUMBER;
            return 0;
        case AST_STRING_LITERAL:
            node->inferred_type = TYPE_TEXT;
            return 0;
        case AST_BOOLEAN_LITERAL:
            node->inferred_type = TYPE_BOOLEAN;
            return 0;
        case AST_IDENTIFIER:
        {
            Symbol *sym = lookup_variable(ctx, node->name);
            if (!sym)
            {
                node->inferred_type = TYPE_ERROR;
                return 1;
            }
            node->inferred_type = sym->type;
            return 0;
        }
        case AST_BINARY_EXPR:
            return infer_binary(ctx, node);
        case AST_UNARY_EXPR:
            return infer_unary(ctx, node);
        case AST_FUNCTION_CALL:
            return infer_call(ctx, node);
        default:
            node->inferred_type = TYPE_ERROR;
            return 1;
    }
}

static int infer_var_decl(SemanticContext *ctx, AstNode *node)
{
    int changed = 0;
    Symbol *sym = lookup_variable(ctx, node->name);

    if (node->value)
        changed |= infer_expr(ctx, node->value);

    if (sym)
    {
        Type vt = node->value ? node->value->inferred_type : TYPE_UNKNOWN;
        if (sym->type == TYPE_UNKNOWN && vt != TYPE_UNKNOWN)
        {
            sym->type = vt;
            node->inferred_type = vt;
            changed = 1;
        }
        else
        {
            node->inferred_type = sym->type;
        }
    }
    return changed;
}

static int infer_assignment(SemanticContext *ctx, AstNode *node)
{
    int changed = 0;
    Symbol *sym = lookup_variable(ctx, node->name);

    if (!sym)
        return 1;

    if (node->value)
        changed |= infer_expr(ctx, node->value);

    if (sym->type == TYPE_UNKNOWN && node->value &&
        node->value->inferred_type != TYPE_UNKNOWN)
    {
        sym->type = node->value->inferred_type;
        node->inferred_type = sym->type;
        changed = 1;
    }
    else
    {
        node->inferred_type = sym->type;
    }
    return changed;
}

static int infer_stmt(SemanticContext *ctx, AstNode *node)
{
    int changed = 0;
    size_t i;

    if (!node)
        return 0;

    switch (node->kind)
    {
        case AST_BLOCK:
            for (i = 0; i < node->params.size; i++)
                changed |= infer_stmt(ctx, node->params.items[i]);
            break;
        case AST_VAR_DECL:
            changed |= infer_var_decl(ctx, node);
            break;
        case AST_ASSIGNMENT:
            changed |= infer_assignment(ctx, node);
            break;
        case AST_DISPLAY:
            if (node->return_expr)
            {
                changed |= infer_expr(ctx, node->return_expr);
                node->inferred_type = node->return_expr->inferred_type;
            }
            break;
        case AST_IF:
            changed |= infer_expr(ctx, node->cond);
            changed |= infer_stmt(ctx, node->then_block);
            if (node->else_block)
                changed |= infer_stmt(ctx, node->else_block);
            break;
        case AST_WHILE:
            changed |= infer_expr(ctx, node->cond);
            changed |= infer_stmt(ctx, node->then_block);
            break;
        case AST_RETURN:
            if (node->return_expr)
            {
                changed |= infer_expr(ctx, node->return_expr);
                if (ctx->current_func)
                {
                    Symbol *fnsym = symbol_lookup_program(&ctx->table, ctx->current_func->name);
                    if (fnsym && fnsym->return_type == TYPE_UNKNOWN)
                    {
                        fnsym->return_type = node->return_expr->inferred_type;
                        changed = 1;
                    }
                }
            }
            break;
        case AST_FUNCTION_CALL:
            changed |= infer_call(ctx, node);
            break;
        default:
            break;
    }
    return changed;
}

static int infer_function_body(SemanticContext *ctx, AstNode *func)
{
    int changed = 0;
    size_t i;
    AstNode *saved_func = ctx->current_func;
    const char *saved_name = ctx->current_func_name;

    ctx->in_function = 1;
    ctx->current_func = func;
    ctx->current_func_name = func->name;

    if (func->then_block)
    {
        for (i = 0; i < func->then_block->params.size; i++)
            changed |= infer_stmt(ctx, func->then_block->params.items[i]);
    }

    ctx->current_func_name = saved_name;
    ctx->current_func = saved_func;
    ctx->in_function = 0;
    return changed;
}

static int infer_program(SemanticContext *ctx, AstNode *program)
{
    int changed = 0;
    size_t i;
    for (i = 0; i < program->params.size; i++)
    {
        AstNode *n = program->params.items[i];
        if (n->kind == AST_FUNCTION_DECL)
            changed |= infer_function_body(ctx, n);
        else
            changed |= infer_stmt(ctx, n);
    }
    return changed;
}

/* ---- validation ------------------------------------------------- */

static void validate_expr(SemanticContext *ctx, AstNode *node)
{
    size_t i;
    if (!node)
        return;

    switch (node->kind)
    {
        case AST_BINARY_EXPR:
        {
            const char *op = node->op;
            validate_expr(ctx, node->left);
            validate_expr(ctx, node->right);

            if (strcmp(op, "and") == 0 || strcmp(op, "or") == 0)
            {
                if (node->left->inferred_type != TYPE_BOOLEAN)
                    error_report(ERROR_SEMANTIC, node->left->line,
                        "operator '%s' requires boolean operands, got %s",
                        op, type_name(node->left->inferred_type));
                if (node->right->inferred_type != TYPE_BOOLEAN)
                    error_report(ERROR_SEMANTIC, node->right->line,
                        "operator '%s' requires boolean operands, got %s",
                        op, type_name(node->right->inferred_type));
            }
            else if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0 ||
                     strcmp(op, "*") == 0 || strcmp(op, "/") == 0 ||
                     strcmp(op, "<") == 0 || strcmp(op, ">") == 0 ||
                     strcmp(op, "<=") == 0 || strcmp(op, ">=") == 0)
            {
                if (node->left->inferred_type != TYPE_NUMBER)
                    error_report(ERROR_SEMANTIC, node->left->line,
                        "operator '%s' requires number operands, got %s",
                        op, type_name(node->left->inferred_type));
                if (node->right->inferred_type != TYPE_NUMBER)
                    error_report(ERROR_SEMANTIC, node->right->line,
                        "operator '%s' requires number operands, got %s",
                        op, type_name(node->right->inferred_type));
            }
            else if (strcmp(op, "==") == 0 || strcmp(op, "!=") == 0)
            {
                if (node->left->inferred_type != node->right->inferred_type &&
                    node->left->inferred_type != TYPE_UNKNOWN &&
                    node->right->inferred_type != TYPE_UNKNOWN)
                    error_report(ERROR_SEMANTIC, node->line,
                        "cannot compare %s with %s using '%s'",
                        type_name(node->left->inferred_type),
                        type_name(node->right->inferred_type), op);
            }
            break;
        }
        case AST_UNARY_EXPR:
        {
            const char *op = node->op;
            validate_expr(ctx, node->left);
            if (strcmp(op, "not") == 0 && node->left->inferred_type != TYPE_BOOLEAN)
                error_report(ERROR_SEMANTIC, node->line,
                    "operator 'not' requires boolean operand, got %s",
                    type_name(node->left->inferred_type));
            else if (strcmp(op, "-") == 0 && node->left->inferred_type != TYPE_NUMBER)
                error_report(ERROR_SEMANTIC, node->line,
                    "operator '-' requires number operand, got %s",
                    type_name(node->left->inferred_type));
            break;
        }
        case AST_IDENTIFIER:
            if (node->inferred_type == TYPE_ERROR)
                error_report(ERROR_SEMANTIC, node->line,
                    "use of undefined variable '%s'", node->name);
            break;
        case AST_FUNCTION_CALL:
        {
            Symbol *fnsym = symbol_lookup_program(&ctx->table, node->name);
            if (!fnsym || fnsym->kind != SYMBOL_FUNCTION)
            {
                error_report(ERROR_SEMANTIC, node->line,
                    "call to undefined function '%s'", node->name);
                break;
            }
            if ((int)node->args.size != fnsym->arity)
            {
                error_report(ERROR_SEMANTIC, node->line,
                    "function '%s' expects %d argument(s), got %u",
                    node->name, fnsym->arity, (unsigned)node->args.size);
            }
            for (i = 0; i < node->args.size && (int)i < fnsym->arity; i++)
            {
                validate_expr(ctx, node->args.items[i]);
                if (fnsym->param_types[i] != TYPE_UNKNOWN &&
                    node->args.items[i]->inferred_type != TYPE_UNKNOWN &&
                    fnsym->param_types[i] != node->args.items[i]->inferred_type)
                    error_report(ERROR_SEMANTIC, node->args.items[i]->line,
                        "argument %u of '%s' expects %s, got %s",
                        (unsigned)(i + 1), node->name,
                        type_name(fnsym->param_types[i]),
                        type_name(node->args.items[i]->inferred_type));
            }
            break;
        }
        default:
            break;
    }
}

static void validate_stmt(SemanticContext *ctx, AstNode *node)
{
    size_t i;
    if (!node)
        return;

    switch (node->kind)
    {
        case AST_BLOCK:
            for (i = 0; i < node->params.size; i++)
                validate_stmt(ctx, node->params.items[i]);
            break;

        case AST_ASSIGNMENT:
        {
            Symbol *sym = lookup_variable(ctx, node->name);
            if (!sym)
            {
                error_report(ERROR_SEMANTIC, node->line,
                    "assignment to undefined variable '%s'", node->name);
                break;
            }
            validate_expr(ctx, node->value);
            if (sym->type != TYPE_UNKNOWN &&
                node->value->inferred_type != TYPE_UNKNOWN &&
                sym->type != node->value->inferred_type)
                error_report(ERROR_SEMANTIC, node->line,
                    "cannot assign %s to %s variable '%s'",
                    type_name(node->value->inferred_type),
                    type_name(sym->type), node->name);
            break;
        }

        case AST_VAR_DECL:
            if (node->value)
            {
                validate_expr(ctx, node->value);
                if (node->value->inferred_type == TYPE_ERROR)
                    error_report(ERROR_SEMANTIC, node->line,
                        "invalid initializer for variable '%s'", node->name);
                node->inferred_type = node->value->inferred_type;
            }
            break;

        case AST_DISPLAY:
            validate_expr(ctx, node->return_expr);
            if (node->return_expr &&
                node->return_expr->inferred_type == TYPE_ERROR)
                error_report(ERROR_SEMANTIC, node->line,
                    "cannot display value of undetermined type");
            break;

        case AST_IF:
            validate_expr(ctx, node->cond);
            if (node->cond->inferred_type != TYPE_BOOLEAN)
                error_report(ERROR_SEMANTIC, node->cond->line,
                    "if condition must be boolean, got %s",
                    type_name(node->cond->inferred_type));
            validate_stmt(ctx, node->then_block);
            if (node->else_block)
                validate_stmt(ctx, node->else_block);
            break;

        case AST_WHILE:
            validate_expr(ctx, node->cond);
            if (node->cond->inferred_type != TYPE_BOOLEAN)
                error_report(ERROR_SEMANTIC, node->cond->line,
                    "while condition must be boolean, got %s",
                    type_name(node->cond->inferred_type));
            validate_stmt(ctx, node->then_block);
            break;

        case AST_RETURN:
        {
            validate_expr(ctx, node->return_expr);
            if (ctx->current_func)
            {
                Symbol *fnsym = symbol_lookup_program(&ctx->table, ctx->current_func->name);
                if (fnsym && fnsym->return_type != TYPE_UNKNOWN &&
                    node->return_expr->inferred_type != TYPE_UNKNOWN &&
                    fnsym->return_type != node->return_expr->inferred_type)
                    error_report(ERROR_SEMANTIC, node->line,
                        "function '%s' returns %s but statement returns %s",
                        ctx->current_func->name, type_name(fnsym->return_type),
                        type_name(node->return_expr->inferred_type));
            }
            break;
        }

        case AST_FUNCTION_CALL:
            validate_expr(ctx, node);
            break;

        default:
            break;
    }
}

static void validate_function(SemanticContext *ctx, AstNode *func)
{
    size_t i;
    AstNode *saved = ctx->current_func;
    const char *saved_name = ctx->current_func_name;

    ctx->current_func = func;
    ctx->current_func_name = func->name;
    ctx->in_function = 1;

    if (func->then_block)
        for (i = 0; i < func->then_block->params.size; i++)
            validate_stmt(ctx, func->then_block->params.items[i]);

    ctx->current_func_name = saved_name;
    ctx->current_func = saved;
    ctx->in_function = 0;
}

static void validate_program(SemanticContext *ctx, AstNode *program)
{
    size_t i;
    ctx->in_function = 0;
    for (i = 0; i < program->params.size; i++)
    {
        AstNode *n = program->params.items[i];
        if (n->kind == AST_FUNCTION_DECL)
            validate_function(ctx, n);
        else
            validate_stmt(ctx, n);
    }
}

/* ---- recursion detection ---------------------------------------- */

typedef struct CallGraphEdge { const char *from; const char *to; } CallGraphEdge;

static void cg_add(CallGraphEdge **edges, size_t *size, size_t *cap,
                   const char *from, const char *to)
{
    if (*size >= *cap)
    {
        size_t newcap = *cap ? *cap * 2 : 16;
        CallGraphEdge *ne = (CallGraphEdge *)realloc(*edges, newcap * sizeof(CallGraphEdge));
        if (!ne)
            return;
        *edges = ne;
        *cap = newcap;
    }
    (*edges)[*size].from = from;
    (*edges)[*size].to = to;
    (*size)++;
}

static void collect_calls(AstNode *node, const char *from, CallGraphEdge **edges,
                          size_t *size, size_t *cap)
{
    size_t i;
    if (!node)
        return;

    switch (node->kind)
    {
        case AST_FUNCTION_CALL:
            cg_add(edges, size, cap, from, node->name);
            for (i = 0; i < node->args.size; i++)
                collect_calls(node->args.items[i], from, edges, size, cap);
            break;
        case AST_BLOCK:
            for (i = 0; i < node->params.size; i++)
                collect_calls(node->params.items[i], from, edges, size, cap);
            break;
        case AST_IF:
            collect_calls(node->cond, from, edges, size, cap);
            collect_calls(node->then_block, from, edges, size, cap);
            if (node->else_block)
                collect_calls(node->else_block, from, edges, size, cap);
            break;
        case AST_WHILE:
            collect_calls(node->cond, from, edges, size, cap);
            collect_calls(node->then_block, from, edges, size, cap);
            break;
        case AST_RETURN:
            collect_calls(node->return_expr, from, edges, size, cap);
            break;
        case AST_VAR_DECL:
            collect_calls(node->value, from, edges, size, cap);
            break;
        case AST_ASSIGNMENT:
            collect_calls(node->value, from, edges, size, cap);
            break;
        case AST_DISPLAY:
            collect_calls(node->return_expr, from, edges, size, cap);
            break;
        case AST_BINARY_EXPR:
            collect_calls(node->left, from, edges, size, cap);
            collect_calls(node->right, from, edges, size, cap);
            break;
        case AST_UNARY_EXPR:
            collect_calls(node->left, from, edges, size, cap);
            break;
        default:
            break;
    }
}

/* DFS: state 0=unvisited, 1=on stack, 2=done */
static int dfs_cycle(const char *name, int name_count, const char **names,
                     CallGraphEdge *edges, size_t edge_count,
                     int *state)
{
    size_t e;
    int i;

    for (e = 0; e < edge_count; e++)
    {
        if (strcmp(edges[e].from, name) == 0)
        {
            int to_idx = -1;
            for (i = 0; i < name_count; i++)
                if (strcmp(names[i], edges[e].to) == 0)
                { to_idx = i; break; }

            if (to_idx >= 0)
            {
                if (state[to_idx] == 1)
                    return 1; /* cycle found */
                if (state[to_idx] == 0)
                {
                    state[to_idx] = 1;
                    if (dfs_cycle(names[to_idx], name_count, names, edges,
                                  edge_count, state))
                        return 1;
                    state[to_idx] = 2;
                }
            }
        }
    }
    return 0;
}

static void detect_recursion(SemanticContext *ctx, AstNode *program)
{
    size_t i, edge_count = 0, cap = 0;
    CallGraphEdge *edges = NULL;
    const char **names = NULL;
    size_t name_count = 0;
    int *state;
    int k;

    for (i = 0; i < program->params.size; i++)
    {
        AstNode *n = program->params.items[i];
        if (n->kind == AST_FUNCTION_DECL)
        {
            names = (const char **)realloc(names, (name_count + 1) * sizeof(char *));
            names[name_count++] = n->name;
            collect_calls(n->then_block, n->name, &edges, &edge_count, &cap);
        }
    }

    if (name_count == 0 || edge_count == 0)
    {
        free(names);
        free(edges);
        return;
    }

    state = (int *)calloc(name_count, sizeof(int));

    for (k = 0; k < (int)name_count; k++)
    {
        int j;
        for (j = 0; j < (int)name_count; j++)
            state[j] = 0;

        state[k] = 1;
        if (dfs_cycle(names[k], (int)name_count, names, edges, edge_count, state))
        {
            error_report(ERROR_SEMANTIC, 0,
                "recursion is not supported: function call cycle detected");
            break;
        }
    }

    free(state);
    free(names);
    free(edges);
}

void semantic_print_symbols(SemanticContext *ctx)
{
    size_t i;
    for (i = 0; i < symbol_count(&ctx->table); i++)
    {
        Symbol *s = symbol_at(&ctx->table, i);
        printf("%s kind=%d scope=%d type=%s line=%d\n",
            s->name, (int)s->kind, (int)s->scope,
            type_name(s->type), s->line);
    }
    for (i = 0; i < ctx->table.func_count; i++)
    {
        size_t j;
        for (j = 0; j < ctx->table.functions[i]->size; j++)
        {
            Symbol *s = ctx->table.functions[i]->symbols[j];
            printf("%s kind=%d scope=%d func=%s type=%s line=%d\n",
                s->name, (int)s->kind, (int)s->scope,
                ctx->table.functions[i]->func_name,
                type_name(s->type), s->line);
        }
    }
}

/* ---- main entry ------------------------------------------------- */

int semantic_analyze(AstNode *program)
{
    SemanticContext ctx;
    size_t i;
    int pass, changed = 1;

    symbol_table_init(&ctx.table);
    ctx.in_function = 0;
    ctx.current_func = NULL;
    ctx.current_func_name = NULL;

    /* Phase 1: collect functions (program scope) */
    for (i = 0; i < program->params.size; i++)
    {
        AstNode *n = program->params.items[i];
        if (n->kind == AST_FUNCTION_DECL)
        {
            Symbol *dup = symbol_lookup_program(&ctx.table, n->name);
            if (dup)
            {
                error_report(ERROR_SEMANTIC, n->line,
                    "duplicate definition of function '%s'", n->name);
                continue;
            }
            Symbol *sym = symbol_add_program(&ctx.table, n->name, SYMBOL_FUNCTION,
                                             n->line);
            if (sym)
            {
                sym->arity = (int)n->params.size;
                sym->param_types = (Type *)calloc(sym->arity > 0 ? (size_t)sym->arity : 1,
                                                  sizeof(Type));
                sym->return_type = TYPE_UNKNOWN;
                sym->func_node = n;
            }
        }
    }

    /* Phase 2 & 3: create per-function scopes; register parameters and
       collect function-local declarations inside the owning function scope;
       register program variables in program scope. */
    for (i = 0; i < program->params.size; i++)
    {
        AstNode *n = program->params.items[i];

        if (n->kind == AST_FUNCTION_DECL)
        {
            int k, j;
            for (k = 0; k < (int)n->params.size; k++)
            {
                AstNode *p = n->params.items[k];
                int dup = 0;
                for (j = 0; j < k; j++)
                {
                    if (strcmp(n->params.items[j]->name, p->name) == 0)
                    {
                        dup = 1;
                        break;
                    }
                }
                if (dup)
                {
                    error_report(ERROR_SEMANTIC, p->line,
                        "duplicate declaration of '%s'", p->name);
                    continue;
                }
                Symbol *psym = symbol_add_function_scope(&ctx.table, n->name,
                                                         p->name, SYMBOL_PARAMETER,
                                                         p->line);
                if (psym)
                {
                    psym->type = TYPE_UNKNOWN;
                    p->inferred_type = TYPE_UNKNOWN;
                }
            }

            if (n->then_block)
            {
                for (k = 0; k < (int)n->then_block->params.size; k++)
                {
                    Symbol *existing;
                    AstNode *lc = n->then_block->params.items[k];
                    if (lc->kind != AST_VAR_DECL)
                        continue;
                    existing = symbol_lookup_in_scope(&ctx.table, n->name,
                                                      lc->name, SYMBOL_PARAMETER);
                    if (existing)
                    {
                        error_report(ERROR_SEMANTIC, lc->line,
                            "duplicate declaration of '%s'", lc->name);
                        continue;
                    }
                    existing = symbol_lookup_in_scope(&ctx.table, n->name,
                                                      lc->name, SYMBOL_VARIABLE);
                    if (existing)
                    {
                        error_report(ERROR_SEMANTIC, lc->line,
                            "duplicate declaration of '%s'", lc->name);
                        continue;
                    }
                    Symbol *lsym = symbol_add_function_scope(&ctx.table, n->name,
                                                             lc->name, SYMBOL_VARIABLE,
                                                             lc->line);
                    if (lsym)
                    {
                        lsym->type = TYPE_UNKNOWN;
                        lc->inferred_type = TYPE_UNKNOWN;
                    }
                }
            }
        }
        else if (n->kind == AST_VAR_DECL)
        {
            Symbol *dup = symbol_lookup_program(&ctx.table, n->name);
            if (dup && (dup->kind == SYMBOL_VARIABLE || dup->kind == SYMBOL_FUNCTION))
            {
                error_report(ERROR_SEMANTIC, n->line,
                    "duplicate declaration of '%s'", n->name);
                continue;
            }
            Symbol *sym = symbol_add_program(&ctx.table, n->name, SYMBOL_VARIABLE,
                                             n->line);
            if (sym)
            {
                sym->type = TYPE_UNKNOWN;
                n->inferred_type = TYPE_UNKNOWN;
            }
        }
    }

    /* Phase 5-6: infer expression types and unify constraints (fixed point) */
    for (pass = 0; pass < MAX_ITER && changed; pass++)
    {
        changed = infer_program(&ctx, program);
    }

    /* annotate AST with resolved function signature and variable types */
    for (i = 0; i < program->params.size; i++)
    {
        AstNode *n = program->params.items[i];
        if (n->kind == AST_FUNCTION_DECL)
        {
            Symbol *fnsym = symbol_lookup_program(&ctx.table, n->name);
            if (fnsym)
            {
                n->inferred_type = fnsym->return_type;
                for (int k = 0; k < fnsym->arity; k++)
                    n->params.items[k]->inferred_type = fnsym->param_types[k];
            }
        }
        else if (n->kind == AST_VAR_DECL)
        {
            Symbol *sym = symbol_lookup_program(&ctx.table, n->name);
            if (sym)
                n->inferred_type = sym->type;
        }
    }

    /* Phase 7-8: validate and detect recursion */
    validate_program(&ctx, program);
    detect_recursion(&ctx, program);

    symbol_table_free(&ctx.table);

    if (error_count > 0)
        return 1;
    return 0;
}
