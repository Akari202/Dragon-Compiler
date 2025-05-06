%{
#include <stdlib.h>
#include <stdio.h>
#include "list.h"
#include "ast.h"
#include "symbol_table.h"

extern int yylex();
extern int yylineno;
void yyerror(char *error);

%}

%union {
    char *sval;
    int ival;
    float fval;
    struct List *list;
    struct ASTNode *ast;
}

%token <sval> ID
%token <sval> RELOP
%token <sval> ADDOP
%token <sval> MULOP
%token <ival> INUM
%token <fval> FNUM
%token PROGRAM
%token VAR
%token ARRAY
%token OF
%token INTEGER
%token REAL
%token BEGIN_TOKEN
%token END
%token FUNCTION
%token PROCEDURE
%token IF
%token THEN
%token ELSE
%token WHILE
%token DO
%token REPEAT
%token UNTIL
%token FOR
%token NOT
%token <sval> ASSIGNOP
%token DOUBLEDOT
%token DOT

%type <list> identifier_list
%type <list> subprogram_declarations
%type <ast> subprogram_declaration
%type <ast> subprogram_header
%type <list> optional_statements
%type <list> statement_list
%type <list> parameter_list
%type <list> arguments
%type <list> expression_list
%type <sval> type
%type <sval> standard_type
%type <ast> variable
%type <ast> procedure_statement
%type <ast> factor
%type <ast> expression
%type <ast> simple_expression
%type <ast> term
%type <ast> compound_statement
%type <ast> program
%type <ast> declarations
%type <ast> statement

%start program
%right THEN ELSE

%%

program:
    PROGRAM ID '(' identifier_list ')' ';'
    declarations
    subprogram_declarations
    compound_statement
    DOT
    {
        $$ = create_ast_node(AST_TYPE_PROGRAM, $2);
        add_child($$, $4);  // identifier_list
        add_child($$, $7);  // declarations
        add_child($$, $8);  // subprogram_declarations
        add_child($$, $9);  // compound_statement
        root_ast = $$;
    }
    ;

identifier_list:
    ID {
        $$ = list_create();
        list_append($$, create_ast_node(AST_TYPE_IDENTIFIER, $1));
    }
    | identifier_list ',' ID {
        list_append($1, create_ast_node(AST_TYPE_IDENTIFIER, $3));
        $$ = $1;
    }
    | /* empty */ {
        $$ = list_create();
    }
    ;

declarations:
    declarations VAR identifier_list ':' type ';'
    {
        $$ = create_ast_node(AST_TYPE_DECLARATIONS, "declarations");
        add_child($$, $1);  // Previous declarations
        ListNode *node = $3->head;
        while (node) {
            insert_symbol(symbol_table, ((ASTNode *)node->data)->value, $5);
            add_child($$, (ASTNode *)node->data);
            node = node->next;
        }
        list_free($3, free);
    }
    | /* empty */
    {
        $$ = create_ast_node(AST_TYPE_DECLARATIONS, NULL);
    }
    ;

type:
    standard_type {
        $$ = $1;
    }
    | ARRAY '[' INUM DOUBLEDOT INUM ']' OF standard_type {
        $$ = strdup("ARRAY");
    }
    ;

standard_type:
    INTEGER {
        $$ = strdup("INTEGER");
    }
    | REAL {
        $$ = strdup("REAL");
    }
    ;

subprogram_declarations:
    subprogram_declarations subprogram_declaration ';'
    {
        list_append($1, $2);
        $$ = $1;
    }
    | /* empty */
    {
        $$ = list_create();
    }
    ;

subprogram_declaration:
    subprogram_header
    declarations
    subprogram_declarations
    compound_statement
    {
        fprintf(stderr, "DEBUG: subprogram_declaration -> subprogram_header: %p, declarations: %p, subprogram_declarations: %p, compound_statement: %p\n", $1, $2, $3, $4);
        $$ = $1;
        if ($2) add_child($$, $2);  // declarations
        if ($3) add_child($$, $3);  // subprogram_declarations
        if ($4) add_child($$, $4);  // compound_statement
        /* pop_scope(symbol_table); */
    }
    ;

subprogram_header:
    FUNCTION ID arguments ':' standard_type ';'
    {
        $$ = create_ast_node(AST_TYPE_FUNCTION, $2);
        add_child($$, $3);
        add_child($$, create_ast_node(AST_TYPE_IDENTIFIER, $5));
        push_scope(symbol_table);
    }
    | PROCEDURE ID arguments ';'
    {
        $$ = create_ast_node(AST_TYPE_PROCEDURE, $2);
        add_child($$, $3);
        push_scope(symbol_table);
    }
    ;

arguments:
    '(' parameter_list ')'
    {
        $$ = $2;
    }
    | /* empty */
    {
        $$ = NULL;
    }
    ;

parameter_list:
    identifier_list ':' type
    {
        $$ = $1;
        ListNode *node = $1->head;
        while (node) {
            insert_symbol(symbol_table, ((ASTNode *)node->data)->value, $3);
            node = node->next;
        }
    }
    | parameter_list ';' identifier_list ':' type
    {
        ListNode *node = $3->head;
        while (node) {
            insert_symbol(symbol_table, ((ASTNode *)node->data)->value, $5);
            node = node->next;
        }
        list_append($1, $3);
        $$ = $1;
    }
    ;

compound_statement:
    BEGIN_TOKEN optional_statements END
    {
        fprintf(stderr, "DEBUG: compound_statement -> optional_statements: %p\n", $2);
        $$ = create_ast_node(AST_TYPE_COMPOUND, NULL);
        if ($2 != NULL) {
            add_child($$, $2);
        }
    }
    | /* empty */
    {
        fprintf(stderr, "DEBUG: compound_statement -> empty\n");
        $$ = create_ast_node(AST_TYPE_COMPOUND, NULL);
    }
    ;

optional_statements:
    statement_list
    {
        $$ = $1;
    }
    | /* empty */
    {
        $$ = create_ast_node(AST_TYPE_COMPOUND, NULL);
    }
    ;

statement_list:
    statement {
        if ($1) {
            $$ = create_ast_node(AST_TYPE_STATEMENT, NULL);
            add_child($$, $1);
        } else {
            $$ = NULL;
        }
    }
    | statement_list ';' statement
    {
        if ($3) {
            add_child($1, $3);
        }
        $$ = $1;
    }
    ;

statement:
    variable ASSIGNOP expression
    {
        $$ = create_ast_node(AST_TYPE_ASSIGNMENT, $2);
        add_child($$, $1);  // variable
        add_child($$, $3);  // expression
    }
    | procedure_statement
    {
        $$ = $1;
    }
    | compound_statement
    {
        $$ = $1;
    }
    | IF expression THEN statement ELSE statement
    {
        $$ = create_ast_node(AST_TYPE_IF, NULL);
        add_child($$, $2);  // condition
        add_child($$, $4);  // true block
        add_child($$, $6);  // false block
    }
    | IF expression THEN statement
    {
        $$ = create_ast_node(AST_TYPE_IF, NULL);
        add_child($$, $2);  // condition
        add_child($$, $4);  // true block
    }
    | WHILE expression DO statement
    {
        $$ = create_ast_node(AST_TYPE_WHILE, NULL);
        add_child($$, $2);  // condition
        add_child($$, $4);  // body
    }
    ;

variable:
    ID {
        $$ = create_ast_node(AST_TYPE_IDENTIFIER, $1);
    }
    | ID '[' expression ']'
    {
        $$ = create_ast_node(AST_TYPE_ARRAY_ACCESS, $1);
        add_child($$, $3);
    }
    ;

procedure_statement:
    ID {
        $$ = create_ast_node(AST_TYPE_PROCEDURE_CALL, $1);
    }
    | ID '(' expression_list ')'
    {
        $$ = create_ast_node(AST_TYPE_PROCEDURE_CALL, $1);
        add_child($$, $3);
    }
    ;

expression_list:
    expression {
        $$ = create_ast_node(AST_TYPE_EXPRESSION, NULL);
        add_child($$, $1);
    }
    | expression_list ',' expression
    {
        add_child($1, $3);
        $$ = $1;
    }
    ;

expression:
    simple_expression
    {
        $$ = $1;
    }
    | simple_expression RELOP simple_expression
    {
        $$ = create_ast_node(AST_TYPE_RELATIONAL, $2);
        add_child($$, $1);  // left
        add_child($$, $3);  // right
    }
    ;

simple_expression:
    term {
        $$ = $1;
    }
    | simple_expression ADDOP term
    {
        $$ = create_ast_node(AST_TYPE_ARITHMETIC, $2);
        add_child($$, $1);  // left
        add_child($$, $3);  // right
    }
    ;

term:
    factor {
        $$ = $1;
    }
    | term MULOP factor
    {
        $$ = create_ast_node(AST_TYPE_ARITHMETIC, $2);
        add_child($$, $1);  // left
        add_child($$, $3);  // right
    }
    ;

factor:
    ID {
        $$ = create_ast_node(AST_TYPE_IDENTIFIER, $1);
    }
    | ID '(' expression_list ')'
    {
        $$ = create_ast_node(AST_TYPE_FUNCTION_CALL, $1);
        add_child($$, $3);
    }
    | ID '[' expression ']'
    {
        $$ = create_ast_node(AST_TYPE_ARRAY_ACCESS, $1);
        add_child($$, $3);
    }
    | INUM {
        char buffer[32];
        sprintf(buffer, "%d", $1);  // Convert integer to string
        $$ = create_ast_node(AST_TYPE_LITERAL, buffer);
    }
    | FNUM {
        char buffer[32];
        sprintf(buffer, "%f", $1);  // Convert float to string
        $$ = create_ast_node(AST_TYPE_LITERAL, buffer);
    }
    | '(' expression ')' {
        $$ = $2;
    }
    | NOT factor
    {
        $$ = create_ast_node(AST_TYPE_EXPRESSION, "NOT");
        add_child($$, $2);
    }
    ;

%%

void yyerror(char *error) {
    fprintf(stderr, "Error: %s at line %d\n", error, yylineno);
}
