#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "ast.h"
#include "error.h"
#include "parser.h"
#include "lexer.h"
#include "semantic.h"
#include "tac.h"
#include "codegen.h"

extern FILE *yyin;

static void usage(const char *prog)
{
    (void)prog;
    fprintf(stderr,
        "AlgoSketch Compiler\n"
        "\n"
        "Usage:\n"
        "algosketch <source.algo>\n"
        "algosketch <source.algo> --run\n"
        "algosketch <source.algo> --ast\n"
        "algosketch <source.algo> --tac\n"
        "\n"
        "Options:\n"
        "  --run            run the produced executable after compiling (default)\n"
        "  --ast            print the parsed AST (does not run)\n"
        "  --tac            print generated three-address code (does not run)\n"
        "  -o <base>        output base name (default: source file name)\n"
        "  --cc <command>   C compiler command (default: gcc)\n"
        "  --cflags <flags> extra flags passed to the C compiler\n");
}

static char *strip_ext(const char *path)
{
    const char *slash = strrchr(path, '/');
    const char *bslash = strrchr(path, '\\');
    const char *base = path;
    const char *dot;
    char *out;

    if (slash && (!bslash || slash > bslash))
        base = slash + 1;
    else if (bslash)
        base = bslash + 1;

    dot = strrchr(base, '.');
    if (dot)
    {
        size_t len = (size_t)(dot - base);
        out = (char *)malloc(len + 1);
        memcpy(out, base, len);
        out[len] = 0;
    }
    else
    {
        out = strdup(base);
    }
    return out;
}

int main(int argc, char **argv)
{
    const char *src_file = NULL;
    char *base = NULL;
    char *out_dir = "generated";
    char *c_file = NULL;
    char *exe_file = NULL;
    const char *cc = "gcc";
    const char *cflags = "";
    int show_ast = 0, show_tac = 0, run = 1;
    int exit_code = 0;
    size_t len;
    int i;

    FILE *in = NULL;
    FILE *out = NULL;
    AstNode *program = NULL;
    TacProgram *tac = NULL;
    int b; /* buffer alloc trick */

    for (i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--ast") == 0)
        {
            show_ast = 1;
            run = 0;
        }
        else if (strcmp(argv[i], "--tac") == 0)
        {
            show_tac = 1;
            run = 0;
        }
        else if (strcmp(argv[i], "--run") == 0)
            run = 1;
        else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc)
        {
            base = strdup(argv[++i]);
        }
        else if (strcmp(argv[i], "--cc") == 0 && i + 1 < argc)
        {
            cc = argv[++i];
        }
        else if (strcmp(argv[i], "--cflags") == 0 && i + 1 < argc)
        {
            cflags = argv[++i];
        }
        else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
        {
            usage(argv[0]);
            return 0;
        }
        else if (argv[i][0] != '-')
        {
            src_file = argv[i];
        }
        else
        {
            fprintf(stderr, "unknown option: %s\n", argv[i]);
            usage(argv[0]);
            return 1;
        }
    }

    if (!src_file)
    {
        usage(argv[0]);
        return 1;
    }

    if (!base)
        base = strip_ext(src_file);

    /* ensure output directory exists */
    mkdir(out_dir);

    len = strlen(out_dir) + strlen(base) + 8;
    c_file = (char *)malloc(len);
    exe_file = (char *)malloc(len);
    snprintf(c_file, len, "%s\\%s.c", out_dir, base);
    snprintf(exe_file, len, "%s\\%s.exe", out_dir, base);

    in = fopen(src_file, "r");
    if (!in)
    {
        fprintf(stderr, "AlgoSketch: cannot open source file '%s'\n", src_file);
        return 1;
    }

    yyrestart(in);

    program = parse_program();
    fclose(in);

    if (error_count > 0)
    {
        fprintf(stderr, "AlgoSketch: %d error(s) during lexing/parsing.\n",
                error_count);
        free(c_file); free(exe_file); free(base);
        return 1;
    }

    if (show_ast)
    {
        printf("=== Abstract Syntax Tree ===\n");
        ast_print(program, 0);
        printf("\n");
    }

    if (semantic_analyze(program) != 0)
    {
        fprintf(stderr, "AlgoSketch: semantic analysis failed with %d error(s).\n",
                error_count - 0);
        free(c_file); free(exe_file); free(base);
        ast_free(program);
        return 1;
    }

    (void)b;

    tac = tac_generate(program);

    if (show_tac)
    {
        printf("=== Three Address Code ===\n");
        tac_print(tac);
        printf("\n");
    }

    out = fopen(c_file, "w");
    if (!out)
    {
        fprintf(stderr, "AlgoSketch: cannot open output file '%s'\n", c_file);
        free(c_file); free(exe_file); free(base);
        tac_free(tac);
        ast_free(program);
        return 1;
    }

    codegen_generate(tac, out);
    fclose(out);

    {
        char cmd[4096];
        snprintf(cmd, sizeof(cmd),
                 "%s %s -o \"%s\" \"%s\"",
                 cc, cflags, exe_file,
                 c_file);

        printf("AlgoSketch: compiling generated C with '%s' ...\n", cc);
        b = system(cmd);
        if (b != 0)
        {
            fprintf(stderr, "AlgoSketch: C compilation failed (exit code %d).\n", b);
            free(c_file); free(exe_file); free(base);
            tac_free(tac);
            ast_free(program);
            return 1;
        }
    }

    printf("AlgoSketch: produced '%s'\n", exe_file);

    if (run)
    {
        char cmd[4096];
        snprintf(cmd, sizeof(cmd), "\"%s\"", exe_file);
        printf("AlgoSketch: running '%s'\n", exe_file);
        b = system(cmd);
        if (b != 0)
        {
            fprintf(stderr,
                    "AlgoSketch: executed program failed (system return %d).\n", b);
            exit_code = b;
        }
    }

    free(c_file); free(exe_file); free(base);
    tac_free(tac);
    ast_free(program);

    return exit_code;
}
