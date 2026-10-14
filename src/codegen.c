#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "codegen.h"
#include "ast.h"

/* ---- type map for temporary names -------------------------------- */

typedef struct TypeEntry
{
    const char *name;
    Type type;
} TypeEntry;

typedef struct TypeMap
{
    TypeEntry *entries;
    size_t size;
    size_t capacity;
} TypeMap;

static void typemap_set(TypeMap *map, const char *name, Type type)
{
    size_t i;
    for (i = 0; i < map->size; i++)
    {
        if (strcmp(map->entries[i].name, name) == 0)
        {
            if (type != TYPE_UNKNOWN)
                map->entries[i].type = type;
            return;
        }
    }
    if (map->size >= map->capacity)
    {
        size_t newcap = map->capacity ? map->capacity * 2 : 32;
        TypeEntry *ne = (TypeEntry *)realloc(map->entries, newcap * sizeof(TypeEntry));
        if (!ne)
            return;
        map->entries = ne;
        map->capacity = newcap;
    }
    map->entries[map->size].name = strdup(name);
    map->entries[map->size].type = type;
    map->size++;
}

static Type typemap_get(TypeMap *map, const char *name)
{
    size_t i;
    for (i = 0; i < map->size; i++)
        if (strcmp(map->entries[i].name, name) == 0)
            return map->entries[i].type;
    return TYPE_UNKNOWN;
}

static void typemap_free(TypeMap *map)
{
    size_t i;
    for (i = 0; i < map->size; i++)
        free((void *)map->entries[i].name);
    free(map->entries);
    map->entries = NULL;
    map->size = 0;
    map->capacity = 0;
}

/* ---- helpers ------------------------------------------------------ */

static const char *c_type_name(Type t)
{
    switch (t)
    {
        case TYPE_NUMBER:  return "double";
        case TYPE_TEXT:    return "const char *";
        case TYPE_BOOLEAN: return "bool";
        default:           return "double";
    }
}

static int var_is_local(TacFunction *fn, const char *name)
{
    size_t i;
    for (i = 0; i < fn->locals.size; i++)
        if (strcmp(fn->locals.items[i]->name, name) == 0)
            return 1;
    for (i = 0; i < (size_t)fn->arity; i++)
        if (strcmp(fn->param_names[i], name) == 0)
            return 1;
    return 0;
}

static void emit_fn_signature(FILE *out, TacFunction *fn)
{
    int i;
    fprintf(out, "%s %s(", c_type_name(fn->return_type), fn->name);
    for (i = 0; i < fn->arity; i++)
    {
        if (i > 0)
            fprintf(out, ", ");
        fprintf(out, "%s %s", c_type_name(fn->param_types[i]), fn->param_names[i]);
    }
    fprintf(out, ")");
}

static void emit_fn_locals(FILE *out, TacFunction *fn)
{
    size_t i;
    for (i = 0; i < fn->locals.size; i++)
        fprintf(out, "    %s %s;\n",
                c_type_name(fn->locals.items[i]->type), fn->locals.items[i]->name);
    if (fn->locals.size > 0)
        fprintf(out, "\n");
}

/* Buffered argument list between PARAM and CALL instructions. */
typedef struct ArgBuf
{
    const char **args;
    size_t size;
    size_t capacity;
} ArgBuf;

static void argbuf_push(ArgBuf *b, const char *arg)
{
    if (b->size >= b->capacity)
    {
        size_t newcap = b->capacity ? b->capacity * 2 : 8;
        const char **na = (const char **)realloc(b->args, newcap * sizeof(char *));
        if (!na)
            return;
        b->args = na;
        b->capacity = newcap;
    }
    b->args[b->size++] = arg;
}

static void argbuf_clear(ArgBuf *b)
{
    free(b->args);
    b->args = NULL;
    b->size = 0;
    b->capacity = 0;
}

/* ---- label reference counting for Fix #6 ----------------------------- */

typedef struct LabelRef
{
    char *name;
    int count;
} LabelRef;

typedef struct LabelRefs
{
    LabelRef *items;
    size_t size;
    size_t capacity;
} LabelRefs;

static void labelrefs_init(LabelRefs *r)
{
    r->items = NULL;
    r->size = 0;
    r->capacity = 0;
}

static void labelrefs_add(LabelRefs *r, const char *name)
{
    size_t i;
    for (i = 0; i < r->size; i++)
    {
        if (strcmp(r->items[i].name, name) == 0)
        {
            r->items[i].count++;
            return;
        }
    }
    if (r->size >= r->capacity)
    {
        size_t newcap = r->capacity ? r->capacity * 2 : 16;
        LabelRef *na = (LabelRef *)realloc(r->items, newcap * sizeof(LabelRef));
        if (!na)
            return;
        r->items = na;
        r->capacity = newcap;
    }
    r->items[r->size].name = strdup(name);
    r->items[r->size].count = 1;
    r->size++;
}

static int labelrefs_get(LabelRefs *r, const char *name)
{
    size_t i;
    for (i = 0; i < r->size; i++)
        if (strcmp(r->items[i].name, name) == 0)
            return r->items[i].count;
    return 0;
}

static void labelrefs_free(LabelRefs *r)
{
    size_t i;
    for (i = 0; i < r->size; i++)
        free(r->items[i].name);
    free(r->items);
    r->items = NULL;
    r->size = 0;
    r->capacity = 0;
}

/* ---- resolve a variable's type across locals/params/temps -------------- */

static Type resolve_var_type(TacFunction *fn, TypeMap *tmap, const char *name)
{
    size_t i;
    for (i = 0; i < fn->locals.size; i++)
        if (strcmp(fn->locals.items[i]->name, name) == 0)
            return fn->locals.items[i]->type;
    for (i = 0; i < (size_t)fn->arity; i++)
        if (strcmp(fn->param_names[i], name) == 0)
            return fn->param_types[i];
    return typemap_get(tmap, name);
}

static void emit_body(FILE *out, TacFunction *fn)
{
    TypeMap tmap;
    ArgBuf args;
    LabelRefs refs;
    size_t i;
    int pass;

    tmap.entries = NULL; tmap.size = 0; tmap.capacity = 0;
    args.args = NULL; args.size = 0; args.capacity = 0;
    labelrefs_init(&refs);

    /* Pass 1: discover every temporary variable required by the body and
       its type. Locals and parameters are declared by emit_fn_locals() /
       the signature, so we only need declarations for the remaining names.
       Several passes are used so a temp's type can be taken from another
       temp regardless of instruction order. */
    for (pass = 0; pass < 4; pass++)
    {
        for (i = 0; i < fn->body.size; i++)
        {
            TacInst *it = fn->body.items[i];
            const char *res = NULL;
            Type ty = TYPE_UNKNOWN;

            switch (it->op)
            {
                case TAC_CONSTANT:
                case TAC_BINARY:
                case TAC_UNARY:
                case TAC_CALL:
                case TAC_COPY:
                    res = it->result;
                    ty = it->type;
                    break;
                default:
                    break;
            }

            if (res && !var_is_local(fn, res))
            {
                if (ty == TYPE_UNKNOWN)
                    ty = resolve_var_type(fn, &tmap, it->arg1);
                if (ty == TYPE_UNKNOWN)
                    ty = TYPE_NUMBER;
                typemap_set(&tmap, res, ty);
            }
        }
    }

    /* Emit all temporary declarations before any label / statement. */
    for (i = 0; i < tmap.size; i++)
        fprintf(out, "    %s %s;\n",
                c_type_name(tmap.entries[i].type), tmap.entries[i].name);
    if (tmap.size > 0)
        fprintf(out, "\n");

    /* Collect labels actually targeted by a jump. */
    for (i = 0; i < fn->body.size; i++)
    {
        TacInst *it = fn->body.items[i];
        if ((it->op == TAC_GOTO || it->op == TAC_IF_FALSE ||
             it->op == TAC_IF_TRUE) && it->label)
            labelrefs_add(&refs, it->label);
    }

    for (i = 0; i < fn->body.size; i++)
    {
        TacInst *it = fn->body.items[i];

        switch (it->op)
        {
            case TAC_CONSTANT:
                fprintf(out, "    %s = %s;\n", it->result, it->arg1);
                typemap_set(&tmap, it->result, it->type);
                break;
            case TAC_COPY:
                fprintf(out, "    %s = %s;\n", it->result, it->arg1);
                if (!var_is_local(fn, it->result))
                {
                    Type dty = it->type != TYPE_UNKNOWN ? it->type
                             : resolve_var_type(fn, &tmap, it->arg1);
                    if (dty == TYPE_UNKNOWN)
                        dty = TYPE_NUMBER;
                    typemap_set(&tmap, it->result, dty);
                }
                break;
            case TAC_BINARY:
            {
                Type lty = resolve_var_type(fn, &tmap, it->arg1);
                Type rty = resolve_var_type(fn, &tmap, it->arg2);
                const char *op = it->binop;

                if ((strcmp(op, "==") == 0 || strcmp(op, "!=") == 0) &&
                    (lty == TYPE_TEXT || rty == TYPE_TEXT))
                {
                    fprintf(out, "    %s = strcmp(%s, %s) %s 0;\n",
                            it->result, it->arg1, it->arg2,
                            strcmp(op, "==") == 0 ? "==" : "!=");
                }
                else
                {
                    fprintf(out, "    %s = %s %s %s;\n",
                            it->result, it->arg1, op, it->arg2);
                }
                typemap_set(&tmap, it->result, it->type);
                break;
            }
            case TAC_UNARY:
                if (strcmp(it->arg2, "not") == 0)
                    fprintf(out, "    %s = !%s;\n", it->result, it->arg1);
                else
                    fprintf(out, "    %s = -%s;\n", it->result, it->arg1);
                typemap_set(&tmap, it->result, it->type);
                break;
            case TAC_LABEL:
                if (labelrefs_get(&refs, it->label) > 0)
                    fprintf(out, "%s:\n", it->label);
                break;
            case TAC_GOTO:
                fprintf(out, "    goto %s;\n", it->label);
                break;
            case TAC_IF_FALSE:
                fprintf(out, "    if (!%s) goto %s;\n", it->arg1, it->label);
                break;
            case TAC_IF_TRUE:
                fprintf(out, "    if (%s) goto %s;\n", it->arg1, it->label);
                break;
            case TAC_PARAM:
                argbuf_push(&args, it->arg1);
                break;
            case TAC_CALL:
            {
                size_t a;
                fprintf(out, "    %s = %s(", it->result, it->arg1);
                for (a = 0; a < args.size; a++)
                {
                    if (a > 0)
                        fprintf(out, ", ");
                    fprintf(out, "%s", args.args[a]);
                }
                fprintf(out, ");\n");
                typemap_set(&tmap, it->result, it->type);
                argbuf_clear(&args);
                break;
            }
            case TAC_RETURN:
                fprintf(out, "    return %s;\n", it->arg1);
                break;
            case TAC_PRINT:
            {
                Type ty = resolve_var_type(fn, &tmap, it->arg1);
                if (ty == TYPE_TEXT)
                    fprintf(out, "    printf(\"%%s\\n\", %s);\n", it->arg1);
                else if (ty == TYPE_BOOLEAN)
                    fprintf(out, "    printf(\"%%s\\n\", %s ? \"true\" : \"false\");\n", it->arg1);
                else
                    fprintf(out, "    printf(\"%%g\\n\", %s);\n", it->arg1);
                break;
            }
        }
    }

    labelrefs_free(&refs);
    typemap_free(&tmap);
    argbuf_clear(&args);
}

int codegen_generate(TacProgram *prog, FILE *out)
{
    size_t i;

    fprintf(out, "#include <stdio.h>\n");
    fprintf(out, "#include <stdbool.h>\n");
    fprintf(out, "#include <string.h>\n\n");

    /* prototypes */
    for (i = 0; i < prog->count; i++)
    {
        emit_fn_signature(out, prog->functions[i]);
        fprintf(out, ";\n");
    }
    if (prog->count > 0)
        fprintf(out, "\n");

    /* function definitions */
    for (i = 0; i < prog->count; i++)
    {
        TacFunction *fn = prog->functions[i];
        emit_fn_signature(out, fn);
        fprintf(out, "\n{\n");
        emit_fn_locals(out, fn);
        emit_body(out, fn);
        fprintf(out, "}\n\n");
    }

    /* main */
    fprintf(out, "int main(void)\n{\n");
    emit_fn_locals(out, prog->main);
    emit_body(out, prog->main);
    fprintf(out, "\n    return 0;\n}\n");

    return 0;
}
