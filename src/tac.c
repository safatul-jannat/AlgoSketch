#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tac.h"
#include "ast.h"

typedef struct GenCtx
{
    TacFunction *current_fn;
    int temp_count;
    int label_count;
    int in_function;
} GenCtx;

/* ---- list helpers ----------------------------------------------- */

static void tac_list_add(TacList *list, TacInst *inst)
{
    if (list->size >= list->capacity)
    {
        size_t newcap = list->capacity ? list->capacity * 2 : 16;
        TacInst **newitems = (TacInst **)realloc(list->items, newcap * sizeof(TacInst *));
        if (!newitems)
            return;
        list->items = newitems;
        list->capacity = newcap;
    }
    list->items[list->size++] = inst;
}

static void tac_var_add(TacVarList *list, const char *name, Type type)
{
    TacVarDecl *vd;

    if (list->size >= list->capacity)
    {
        size_t newcap = list->capacity ? list->capacity * 2 : 8;
        TacVarDecl **newitems = (TacVarDecl **)realloc(list->items, newcap * sizeof(TacVarDecl *));
        if (!newitems)
            return;
        list->items = newitems;
        list->capacity = newcap;
    }
    vd = (TacVarDecl *)calloc(1, sizeof(TacVarDecl));
    if (!vd)
        return;
    vd->name = strdup(name);
    vd->type = type;
    list->items[list->size++] = vd;
}

static int tac_var_has(TacVarList *list, const char *name)
{
    size_t i;
    for (i = 0; i < list->size; i++)
        if (strcmp(list->items[i]->name, name) == 0)
            return 1;
    return 0;
}

/* ---- instruction helpers ----------------------------------------- */

static TacInst *inst_new(TacOp op, int line)
{
    TacInst *inst = (TacInst *)calloc(1, sizeof(TacInst));
    if (inst)
        inst->op = op;
    inst->line = line;
    return inst;
}

static char *new_temp(GenCtx *ctx)
{
    char buf[32];
    snprintf(buf, sizeof(buf), "t%d", ctx->temp_count++);
    return strdup(buf);
}

static char *new_label(GenCtx *ctx)
{
    char buf[32];
    snprintf(buf, sizeof(buf), "L%d", ctx->label_count++);
    return strdup(buf);
}

static char *format_number(double v)
{
    char buf[64];
    snprintf(buf, sizeof(buf), "%g", v);
    return strdup(buf);
}

static char *format_string(const char *s)
{
    size_t len = strlen(s);
    char *out = (char *)malloc(len * 4 + 3);
    size_t i, j = 0;

    out[j++] = '"';
    for (i = 0; i < len; i++)
    {
        if (s[i] == '\\' || s[i] == '"')
        {
            out[j++] = '\\';
            out[j++] = s[i];
        }
        else if (s[i] == '\n')
        {
            out[j++] = '\\';
            out[j++] = 'n';
        }
        else
        {
            out[j++] = s[i];
        }
    }
    out[j++] = '"';
    out[j] = 0;
    return out;
}

/* ---- expression generation --------------------------------------- */

static char *gen_expr(GenCtx *ctx, AstNode *node);

static char *gen_expr(GenCtx *ctx, AstNode *node)
{
    TacList *body = &ctx->current_fn->body;
    size_t i;

    if (!node)
        return strdup("");

    switch (node->kind)
    {
        case AST_NUMBER_LITERAL:
        {
            char *r = new_temp(ctx);
            TacInst *inst = inst_new(TAC_CONSTANT, node->line);
            inst->result = strdup(r);
            inst->arg1 = format_number(node->num_value);
            inst->type = TYPE_NUMBER;
            tac_list_add(body, inst);
            return r;
        }
        case AST_STRING_LITERAL:
        {
            char *r = new_temp(ctx);
            TacInst *inst = inst_new(TAC_CONSTANT, node->line);
            inst->result = strdup(r);
            inst->arg1 = format_string(node->str_value);
            inst->type = TYPE_TEXT;
            tac_list_add(body, inst);
            return r;
        }
        case AST_BOOLEAN_LITERAL:
        {
            char *r = new_temp(ctx);
            TacInst *inst = inst_new(TAC_CONSTANT, node->line);
            inst->result = strdup(r);
            inst->arg1 = strdup(node->bool_value ? "true" : "false");
            inst->type = TYPE_BOOLEAN;
            tac_list_add(body, inst);
            return r;
        }
        case AST_IDENTIFIER:
        {
            char *r = new_temp(ctx);
            TacInst *inst = inst_new(TAC_COPY, node->line);
            inst->result = strdup(r);
            inst->arg1 = strdup(node->name);
            inst->type = node->inferred_type;
            tac_list_add(body, inst);
            return r;
        }
        case AST_BINARY_EXPR:
        {
            const char *op = node->op;

            if (strcmp(op, "and") == 0 || strcmp(op, "or") == 0)
            {
                char *r = new_temp(ctx);
                char *l_false, *l_true, *l_done;
                char *a, *b;
                int is_or = (strcmp(op, "or") == 0);

                l_false = new_label(ctx);
                l_true = new_label(ctx);
                l_done = new_label(ctx);

                a = gen_expr(ctx, node->left);

                if (is_or)
                {
                    TacInst *it = inst_new(TAC_IF_TRUE, node->line);
                    it->arg1 = strdup(a);
                    it->label = strdup(l_true);
                    tac_list_add(body, it);
                }
                else
                {
                    TacInst *it = inst_new(TAC_IF_FALSE, node->line);
                    it->arg1 = strdup(a);
                    it->label = strdup(l_false);
                    tac_list_add(body, it);
                }

                b = gen_expr(ctx, node->right);

                if (is_or)
                {
                    TacInst *it = inst_new(TAC_IF_TRUE, node->line);
                    it->arg1 = strdup(b);
                    it->label = strdup(l_true);
                    tac_list_add(body, it);
                }
                else
                {
                    TacInst *it = inst_new(TAC_IF_FALSE, node->line);
                    it->arg1 = strdup(b);
                    it->label = strdup(l_false);
                    tac_list_add(body, it);
                }

                {
                    TacInst *inst = inst_new(TAC_CONSTANT, node->line);
                    inst->result = strdup(r);
                    inst->arg1 = strdup(is_or ? "false" : "true");
                    inst->type = TYPE_BOOLEAN;
                    tac_list_add(body, inst);
                }

                {
                    TacInst *inst = inst_new(TAC_GOTO, node->line);
                    inst->label = strdup(l_done);
                    tac_list_add(body, inst);
                }

                {
                    TacInst *inst = inst_new(TAC_LABEL, node->line);
                    inst->label = strdup(l_false);
                    tac_list_add(body, inst);
                }
                if (is_or)
                {
                    TacInst *inst = inst_new(TAC_LABEL, node->line);
                    inst->label = strdup(l_true);
                    tac_list_add(body, inst);
                }

                {
                    TacInst *inst = inst_new(TAC_CONSTANT, node->line);
                    inst->result = strdup(r);
                    inst->arg1 = strdup(is_or ? "true" : "false");
                    inst->type = TYPE_BOOLEAN;
                    tac_list_add(body, inst);
                }

                if (!is_or)
                {
                    TacInst *inst = inst_new(TAC_LABEL, node->line);
                    inst->label = strdup(l_true);
                    tac_list_add(body, inst);
                }

                {
                    TacInst *inst = inst_new(TAC_LABEL, node->line);
                    inst->label = strdup(l_done);
                    tac_list_add(body, inst);
                }

                free(a);
                free(b);
                free(l_false);
                free(l_true);
                free(l_done);
                return r;
            }
            else
            {
                char *r = new_temp(ctx);
                char *a = gen_expr(ctx, node->left);
                char *b = gen_expr(ctx, node->right);
                TacInst *inst = inst_new(TAC_BINARY, node->line);
                inst->result = strdup(r);
                inst->arg1 = strdup(a);
                inst->arg2 = strdup(b);
                inst->binop = strdup(op);
                inst->type = node->inferred_type;
                tac_list_add(body, inst);
                free(a);
                free(b);
                return r;
            }
        }
        case AST_UNARY_EXPR:
        {
            const char *op = node->op;
            char *r = new_temp(ctx);
            char *a = gen_expr(ctx, node->left);
            TacInst *inst = inst_new(TAC_UNARY, node->line);
            inst->result = strdup(r);
            inst->arg1 = strdup(a);
            inst->arg2 = strdup(op);
            inst->type = node->inferred_type;
            tac_list_add(body, inst);
            free(a);
            return r;
        }
        case AST_FUNCTION_CALL:
        {
            char *r = new_temp(ctx);
            for (i = 0; i < node->args.size; i++)
            {
                char *a = gen_expr(ctx, node->args.items[i]);
                TacInst *inst = inst_new(TAC_PARAM, node->args.items[i]->line);
                inst->arg1 = strdup(a);
                tac_list_add(body, inst);
                free(a);
            }
            {
                TacInst *inst = inst_new(TAC_CALL, node->line);
                inst->result = strdup(r);
                inst->arg1 = strdup(node->name);
                inst->type = node->inferred_type;
                tac_list_add(body, inst);
            }
            return r;
        }
        default:
            return strdup("");
    }
}

/* ---- statement generation ---------------------------------------- */

static void gen_stmt(GenCtx *ctx, AstNode *node);

static void gen_block(GenCtx *ctx, AstNode *block)
{
    size_t i;
    if (!block)
        return;
    for (i = 0; i < block->params.size; i++)
        gen_stmt(ctx, block->params.items[i]);
}

static void gen_stmt(GenCtx *ctx, AstNode *node)
{
    TacList *body = &ctx->current_fn->body;

    if (!node)
        return;

    switch (node->kind)
    {
        case AST_BLOCK:
            gen_block(ctx, node);
            break;

        case AST_VAR_DECL:
        {
            Type vt = node->inferred_type;
            if (!tac_var_has(&ctx->current_fn->locals, node->name))
            {
                if (vt == TYPE_UNKNOWN && node->value)
                    vt = node->value->inferred_type;
                if (vt == TYPE_UNKNOWN)
                    vt = TYPE_NUMBER;
                tac_var_add(&ctx->current_fn->locals, node->name, vt);
            }

            if (node->value)
            {
                char *a = gen_expr(ctx, node->value);
                TacInst *inst = inst_new(TAC_COPY, node->line);
                inst->result = strdup(node->name);
                inst->arg1 = strdup(a);
                tac_list_add(body, inst);
                free(a);
            }
            break;
        }

        case AST_ASSIGNMENT:
        {
            char *a = gen_expr(ctx, node->value);
            TacInst *inst = inst_new(TAC_COPY, node->line);
            inst->result = strdup(node->name);
            inst->arg1 = strdup(a);
            tac_list_add(body, inst);
            free(a);
            break;
        }

        case AST_DISPLAY:
        {
            char *a = gen_expr(ctx, node->return_expr);
            TacInst *inst = inst_new(TAC_PRINT, node->line);
            inst->arg1 = strdup(a);
            tac_list_add(body, inst);
            free(a);
            break;
        }

        case AST_IF:
        {
            char *cond = gen_expr(ctx, node->cond);
            char *l_else = new_label(ctx);
            char *l_end = new_label(ctx);
            TacInst *it;

            it = inst_new(TAC_IF_FALSE, node->line);
            it->arg1 = strdup(cond);
            it->label = (node->else_block) ? strdup(l_else) : strdup(l_end);
            tac_list_add(body, it);

            gen_block(ctx, node->then_block);

            if (node->else_block)
            {
                it = inst_new(TAC_GOTO, node->line);
                it->label = strdup(l_end);
                tac_list_add(body, it);

                it = inst_new(TAC_LABEL, node->line);
                it->label = l_else;
                tac_list_add(body, it);

                gen_block(ctx, node->else_block);
            }

            it = inst_new(TAC_LABEL, node->line);
            it->label = l_end;
            tac_list_add(body, it);

            free(cond);
            break;
        }

        case AST_WHILE:
        {
            char *l_start = new_label(ctx);
            char *l_end = new_label(ctx);
            TacInst *it;

            it = inst_new(TAC_LABEL, node->line);
            it->label = l_start;
            tac_list_add(body, it);

            char *cond = gen_expr(ctx, node->cond);

            it = inst_new(TAC_IF_FALSE, node->line);
            it->arg1 = strdup(cond);
            it->label = strdup(l_end);
            tac_list_add(body, it);

            gen_block(ctx, node->then_block);

            it = inst_new(TAC_GOTO, node->line);
            it->label = strdup(l_start);
            tac_list_add(body, it);

            it = inst_new(TAC_LABEL, node->line);
            it->label = l_end;
            tac_list_add(body, it);

            free(cond);
            break;
        }

        case AST_RETURN:
        {
            char *a = gen_expr(ctx, node->return_expr);
            TacInst *inst = inst_new(TAC_RETURN, node->line);
            inst->arg1 = strdup(a);
            tac_list_add(body, inst);
            free(a);
            break;
        }

        case AST_FUNCTION_CALL:
        {
            char *r = gen_expr(ctx, node);
            free(r);
            break;
        }

        default:
            break;
    }
}

/* ---- function / program generation -------------------------------- */

static TacFunction *tac_fn_new(const char *name)
{
    TacFunction *fn = (TacFunction *)calloc(1, sizeof(TacFunction));
    fn->name = strdup(name);
    fn->arity = 0;
    fn->param_names = NULL;
    fn->param_types = NULL;
    fn->return_type = TYPE_UNKNOWN;
    fn->body.items = NULL;
    fn->body.size = 0;
    fn->body.capacity = 0;
    fn->locals.items = NULL;
    fn->locals.size = 0;
    fn->locals.capacity = 0;
    return fn;
}

TacProgram *tac_generate(AstNode *program)
{
    GenCtx ctx;
    TacProgram *prog;
    size_t i;
    int k;

    memset(&ctx, 0, sizeof(ctx));
    ctx.current_fn = NULL;
    ctx.temp_count = 1;
    ctx.label_count = 1;
    ctx.in_function = 0;

    prog = (TacProgram *)calloc(1, sizeof(TacProgram));
    prog->main = NULL;

    /* main body */
    ctx.current_fn = tac_fn_new("main");
    ctx.temp_count = 1;
    ctx.label_count = 1;
    ctx.in_function = 0;

    for (i = 0; i < program->params.size; i++)
    {
        AstNode *n = program->params.items[i];
        if (n->kind == AST_FUNCTION_DECL)
            continue;
        gen_stmt(&ctx, n);
    }

    prog->main = ctx.current_fn;

    /* function bodies */
    for (i = 0; i < program->params.size; i++)
    {
        AstNode *n = program->params.items[i];
        if (n->kind != AST_FUNCTION_DECL)
            continue;

        ctx.current_fn = tac_fn_new(n->name);
        ctx.temp_count = 1;
        ctx.label_count = 1;
        ctx.in_function = 1;

        ctx.current_fn->arity = (int)n->params.size;
        ctx.current_fn->param_names = (char **)calloc(n->params.size ? n->params.size : 1, sizeof(char *));
        ctx.current_fn->param_types = (Type *)calloc(n->params.size ? n->params.size : 1, sizeof(Type));
        ctx.current_fn->return_type = n->inferred_type;

        for (k = 0; k < (int)n->params.size; k++)
        {
            AstNode *p = n->params.items[k];
            ctx.current_fn->param_names[k] = strdup(p->name);
            ctx.current_fn->param_types[k] = p->inferred_type;
        }

        gen_block(&ctx, n->then_block);

        if (prog->count >= prog->capacity)
        {
            size_t newcap = prog->capacity ? prog->capacity * 2 : 8;
            TacFunction **newarr = (TacFunction **)realloc(prog->functions, newcap * sizeof(TacFunction *));
            if (!newarr)
                break;
            prog->functions = newarr;
            prog->capacity = newcap;
        }
        prog->functions[prog->count++] = ctx.current_fn;
    }

    return prog;
}

/* ---- printing ----------------------------------------------------- */

static const char *op_str(TacOp op)
{
    switch (op)
    {
        case TAC_COPY:     return "COPY";
        case TAC_CONSTANT: return "CONSTANT";
        case TAC_BINARY:   return "BINARY";
        case TAC_UNARY:    return "UNARY";
        case TAC_LABEL:    return "LABEL";
        case TAC_GOTO:     return "GOTO";
        case TAC_IF_FALSE: return "IF_FALSE";
        case TAC_IF_TRUE:  return "IF_TRUE";
        case TAC_PARAM:    return "PARAM";
        case TAC_CALL:     return "CALL";
        case TAC_RETURN:   return "RETURN";
        case TAC_PRINT:    return "PRINT";
    }
    return "?";
}

static void tac_print_fn(FILE *out, TacFunction *fn)
{
    size_t i;
    fprintf(out, "// function %s (arity %d)\n", fn->name, fn->arity);
    for (i = 0; i < fn->body.size; i++)
    {
        TacInst *it = fn->body.items[i];
        switch (it->op)
        {
            case TAC_COPY:
                fprintf(out, "  %s %s = %s\n", op_str(it->op), it->result, it->arg1);
                break;
            case TAC_CONSTANT:
                fprintf(out, "  %s %s = %s\n", op_str(it->op), it->result, it->arg1);
                break;
            case TAC_BINARY:
                fprintf(out, "  %s %s = %s %s %s\n", op_str(it->op), it->result, it->arg1, it->binop, it->arg2);
                break;
            case TAC_UNARY:
                fprintf(out, "  %s %s = %s %s\n", op_str(it->op), it->result, it->arg2, it->arg1);
                break;
            case TAC_LABEL:
                fprintf(out, "  %s %s:\n", op_str(it->op), it->label);
                break;
            case TAC_GOTO:
                fprintf(out, "  %s %s\n", op_str(it->op), it->label);
                break;
            case TAC_IF_FALSE:
                fprintf(out, "  %s %s GOTO %s\n", op_str(it->op), it->arg1, it->label);
                break;
            case TAC_IF_TRUE:
                fprintf(out, "  %s %s GOTO %s\n", op_str(it->op), it->arg1, it->label);
                break;
            case TAC_PARAM:
                fprintf(out, "  %s %s\n", op_str(it->op), it->arg1);
                break;
            case TAC_CALL:
                fprintf(out, "  %s %s = %s(...)\n", op_str(it->op), it->result, it->arg1);
                break;
            case TAC_RETURN:
                fprintf(out, "  %s %s\n", op_str(it->op), it->arg1);
                break;
            case TAC_PRINT:
                fprintf(out, "  %s %s\n", op_str(it->op), it->arg1);
                break;
        }
    }
}

void tac_print(TacProgram *prog)
{
    size_t i;
    for (i = 0; i < prog->count; i++)
        tac_print_fn(stdout, prog->functions[i]);
    if (prog->main)
    {
        fprintf(stdout, "\n// function main\n");
        tac_print_fn(stdout, prog->main);
    }
}

/* ---- cleanup ------------------------------------------------------ */

static void free_tac_inst(TacInst *it)
{
    if (it->result) free(it->result);
    if (it->arg1) free(it->arg1);
    if (it->arg2) free(it->arg2);
    if (it->binop) free(it->binop);
    if (it->label) free(it->label);
    free(it);
}

static void free_tac_list(TacList *l)
{
    size_t i;
    for (i = 0; i < l->size; i++)
        free_tac_inst(l->items[i]);
    free(l->items);
    l->items = NULL;
    l->size = 0;
    l->capacity = 0;
}

static void free_tac_varlist(TacVarList *l)
{
    size_t i;
    for (i = 0; i < l->size; i++)
    {
        if (l->items[i]->name) free(l->items[i]->name);
        free(l->items[i]);
    }
    free(l->items);
    l->items = NULL;
    l->size = 0;
    l->capacity = 0;
}

static void free_tac_fn(TacFunction *fn)
{
    int i;
    if (!fn) return;
    if (fn->name) free(fn->name);
    if (fn->param_names)
    {
        for (i = 0; i < fn->arity; i++)
            if (fn->param_names[i]) free(fn->param_names[i]);
        free(fn->param_names);
    }
    if (fn->param_types) free(fn->param_types);
    free_tac_varlist(&fn->locals);
    free_tac_list(&fn->body);
    free(fn);
}

void tac_free(TacProgram *prog)
{
    size_t i;
    if (!prog) return;
    for (i = 0; i < prog->count; i++)
        free_tac_fn(prog->functions[i]);
    free(prog->functions);
    if (prog->main)
        free_tac_fn(prog->main);
    free(prog);
}
