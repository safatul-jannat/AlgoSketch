%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "error.h"
#include "lexer.h"

extern int yylex(void);
void yyerror(const char *msg);

static AstNode *wrapper_program;
%}

%code requires {
#include "ast.h"
}

%code provides {
AstNode *parse_program(void);
}

%defines

%union {
    double num;
    char *text;
    AstNode *node;
    AstNodeList list;
}

%token <num> NUMBER_LITERAL
%token <text> STRING_LITERAL
%token <text> ID

%token VAR IF THEN OTHERWISE WHILE FUNCTION BEGIN END RETURN DISPLAY
%token TRUE FALSE AND OR NOT
%token ASSIGN PLUS MINUS MULTIPLY DIVIDE LT GT LE GE EQ NE
%token LPAREN RPAREN COMMA SEMICOLON

%type <node> program function_decl params opt_params block stmt fn_body fn_stmt
%type <node> var_decl assignment display if_stmt while_stmt return_stmt
%type <node> expr logical_or logical_and equality relational additive
%type <node> multiplicative unary primary function_call
%type <list> expr_list opt_expr_list
%type <list> function_list declaration_list executable_list fn_locals fn_execs

%left OR
%left AND
%left NOT
%left EQ NE
%left LT GT LE GE
%left PLUS MINUS
%left MULTIPLY DIVIDE

%%

program
    : function_list declaration_list executable_list {
        size_t li;
        AstNode *p = ast_new(AST_PROGRAM, yylineno);
        for (li = 0; li < $1.size; li++)
            ast_list_append(&p->params, $1.items[li]);
        for (li = 0; li < $2.size; li++)
            ast_list_append(&p->params, $2.items[li]);
        for (li = 0; li < $3.size; li++)
            ast_list_append(&p->params, $3.items[li]);
        free($1.items);
        free($2.items);
        free($3.items);
        wrapper_program = p;
        $$ = p;
    }
    ;

function_list
    : /* empty */ {
        $$.items = NULL;
        $$.size = 0;
        $$.capacity = 0;
    }
    | function_list function_decl {
        ast_list_append(&$1, $2);
        $$ = $1;
    }
    ;

declaration_list
    : /* empty */ {
        $$.items = NULL;
        $$.size = 0;
        $$.capacity = 0;
    }
    | declaration_list var_decl {
        ast_list_append(&$1, $2);
        $$ = $1;
    }
    ;

executable_list
    : /* empty */ {
        $$.items = NULL;
        $$.size = 0;
        $$.capacity = 0;
    }
    | executable_list stmt {
        ast_list_append(&$1, $2);
        $$ = $1;
    }
    ;

function_decl
    : FUNCTION ID LPAREN opt_params RPAREN BEGIN fn_body END {
        $$ = $4;
        $$->kind = AST_FUNCTION_DECL;
        $$->name = $2;
        $$->then_block = $7;
    }
    ;

fn_body
    : fn_locals fn_execs return_stmt {
        size_t li;
        AstNode *blk = ast_new(AST_BLOCK, yylineno);
        for (li = 0; li < $1.size; li++)
            ast_list_append(&blk->params, $1.items[li]);
        for (li = 0; li < $2.size; li++)
            ast_list_append(&blk->params, $2.items[li]);
        ast_list_append(&blk->params, $3);
        free($1.items);
        free($2.items);
        $$ = blk;
    }
    ;

fn_locals
    : /* empty */ {
        $$.items = NULL;
        $$.size = 0;
        $$.capacity = 0;
    }
    | fn_locals var_decl {
        ast_list_append(&$1, $2);
        $$ = $1;
    }
    ;

fn_execs
    : /* empty */ {
        $$.items = NULL;
        $$.size = 0;
        $$.capacity = 0;
    }
    | fn_execs fn_stmt {
        ast_list_append(&$1, $2);
        $$ = $1;
    }
    ;

fn_stmt
    : assignment { $$ = $1; }
    | display { $$ = $1; }
    | if_stmt { $$ = $1; }
    | while_stmt { $$ = $1; }
    ;

opt_params
    : /* empty */ {
        $$ = ast_new(AST_FUNCTION_DECL, yylineno);
    }
    | params {
        $$ = $1;
    }
    ;

params
    : ID {
        AstNode *p = ast_new(AST_IDENTIFIER, yylineno);
        p->name = $1;
        $$ = ast_new(AST_FUNCTION_DECL, yylineno);
        ast_list_append(&$$->params, p);
    }
    | params COMMA ID {
        AstNode *p = ast_new(AST_IDENTIFIER, yylineno);
        p->name = $3;
        ast_list_append(&$1->params, p);
        $$ = $1;
    }
    ;

block
    : /* empty */ {
        $$ = ast_new(AST_BLOCK, yylineno);
    }
    | block stmt {
        ast_list_append(&$1->params, $2);
        $$ = $1;
    }
    | block var_decl {
        ast_list_append(&$1->params, $2);
        $$ = $1;
    }
    ;
stmt
    : assignment { $$ = $1; }
    | display { $$ = $1; }
    | if_stmt { $$ = $1; }
    | while_stmt { $$ = $1; }
    | return_stmt { $$ = $1; }
    ;

var_decl
    : VAR ID ASSIGN expr SEMICOLON {
        $$ = ast_new(AST_VAR_DECL, @2.first_line);
        $$->name = $2;
        $$->value = $4;
    }
    ;

assignment
    : ID ASSIGN expr SEMICOLON {
        $$ = ast_new(AST_ASSIGNMENT, @1.first_line);
        $$->name = $1;
        $$->value = $3;
    }
    ;

display
    : DISPLAY expr SEMICOLON {
        $$ = ast_new(AST_DISPLAY, @1.first_line);
        $$->return_expr = $2;
    }
    ;

if_stmt
    : IF expr THEN block OTHERWISE block END {
        $$ = ast_new(AST_IF, @1.first_line);
        $$->cond = $2;
        $$->then_block = $4;
        $$->else_block = $6;
    }
    | IF expr THEN block END {
        $$ = ast_new(AST_IF, @1.first_line);
        $$->cond = $2;
        $$->then_block = $4;
        $$->else_block = NULL;
    }
    ;

while_stmt
    : WHILE expr block END {
        $$ = ast_new(AST_WHILE, @1.first_line);
        $$->cond = $2;
        $$->then_block = $3;
    }
    ;

return_stmt
    : RETURN expr SEMICOLON {
        $$ = ast_new(AST_RETURN, @1.first_line);
        $$->return_expr = $2;
    }
    ;

expr
    : logical_or { $$ = $1; }
    ;

logical_or
    : logical_and { $$ = $1; }
    | logical_or OR logical_and {
        $$ = ast_new(AST_BINARY_EXPR, @2.first_line);
        $$->op = (char *)"or";
        $$->left = $1;
        $$->right = $3;
    }
    ;

logical_and
    : equality { $$ = $1; }
    | logical_and AND equality {
        $$ = ast_new(AST_BINARY_EXPR, @2.first_line);
        $$->op = (char *)"and";
        $$->left = $1;
        $$->right = $3;
    }
    ;

equality
    : relational { $$ = $1; }
    | equality EQ relational {
        $$ = ast_new(AST_BINARY_EXPR, @2.first_line);
        $$->op = (char *)"==";
        $$->left = $1;
        $$->right = $3;
    }
    | equality NE relational {
        $$ = ast_new(AST_BINARY_EXPR, @2.first_line);
        $$->op = (char *)"!=";
        $$->left = $1;
        $$->right = $3;
    }
    ;

relational
    : additive { $$ = $1; }
    | relational LT additive {
        $$ = ast_new(AST_BINARY_EXPR, @2.first_line);
        $$->op = (char *)"<";
        $$->left = $1;
        $$->right = $3;
    }
    | relational GT additive {
        $$ = ast_new(AST_BINARY_EXPR, @2.first_line);
        $$->op = (char *)">";
        $$->left = $1;
        $$->right = $3;
    }
    | relational LE additive {
        $$ = ast_new(AST_BINARY_EXPR, @2.first_line);
        $$->op = (char *)"<=";
        $$->left = $1;
        $$->right = $3;
    }
    | relational GE additive {
        $$ = ast_new(AST_BINARY_EXPR, @2.first_line);
        $$->op = (char *)">=";
        $$->left = $1;
        $$->right = $3;
    }
    ;

additive
    : multiplicative { $$ = $1; }
    | additive PLUS multiplicative {
        $$ = ast_new(AST_BINARY_EXPR, @2.first_line);
        $$->op = (char *)"+";
        $$->left = $1;
        $$->right = $3;
    }
    | additive MINUS multiplicative {
        $$ = ast_new(AST_BINARY_EXPR, @2.first_line);
        $$->op = (char *)"-";
        $$->left = $1;
        $$->right = $3;
    }
    ;

multiplicative
    : unary { $$ = $1; }
    | multiplicative MULTIPLY unary {
        $$ = ast_new(AST_BINARY_EXPR, @2.first_line);
        $$->op = (char *)"*";
        $$->left = $1;
        $$->right = $3;
    }
    | multiplicative DIVIDE unary {
        $$ = ast_new(AST_BINARY_EXPR, @2.first_line);
        $$->op = (char *)"/";
        $$->left = $1;
        $$->right = $3;
    }
    ;

unary
    : NOT unary {
        $$ = ast_new(AST_UNARY_EXPR, @1.first_line);
        $$->op = (char *)"not";
        $$->left = $2;
    }
    | MINUS unary {
        $$ = ast_new(AST_UNARY_EXPR, @1.first_line);
        $$->op = (char *)"-";
        $$->left = $2;
    }
    | primary { $$ = $1; }
    ;

primary
    : NUMBER_LITERAL {
        $$ = ast_new(AST_NUMBER_LITERAL, @1.first_line);
        $$->num_value = $1;
    }
    | STRING_LITERAL {
        $$ = ast_new(AST_STRING_LITERAL, @1.first_line);
        $$->str_value = $1;
    }
    | TRUE {
        $$ = ast_new(AST_BOOLEAN_LITERAL, @1.first_line);
        $$->bool_value = 1;
    }
    | FALSE {
        $$ = ast_new(AST_BOOLEAN_LITERAL, @1.first_line);
        $$->bool_value = 0;
    }
    | ID {
        $$ = ast_new(AST_IDENTIFIER, @1.first_line);
        $$->name = $1;
    }
    | function_call { $$ = $1; }
    | LPAREN expr RPAREN { $$ = $2; }
    ;

function_call
    : ID LPAREN opt_expr_list RPAREN {
        $$ = ast_new(AST_FUNCTION_CALL, @1.first_line);
        $$->name = $1;
        $$->args = $3;
    }
    ;

opt_expr_list
    : /* empty */ { $$.items = NULL; $$.size = 0; $$.capacity = 0; }
    | expr_list { $$ = $1; }
    ;

expr_list
    : expr {
        $$.items = NULL; $$.size = 0; $$.capacity = 0;
        ast_list_append(&$$, $1);
    }
    | expr_list COMMA expr {
        ast_list_append(&$1, $3);
        $$ = $1;
    }
    ;

%%

void yyerror(const char *msg)
{
    error_report(ERROR_SYNTAX, yylineno, "%s", msg);
}

AstNode *parse_program(void)
{
    wrapper_program = NULL;
    yyparse();
    return wrapper_program;
}
