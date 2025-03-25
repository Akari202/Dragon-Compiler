%{

#include <stdlib.h>
#include <stdio.h>
#include "list.h"
#include "ast.h"

extern int yylex();

void yyerror(char *error){
    fprintf(stderr, "Error: %s\n", error);
    exit(1);
}

%}

%token AND
%token ARRAY
%token BOOLEAN
%token CHAR
%token DIV
%token DO
%token ELSE
%token END
%token FOR
%token FUNCTION
%token ID
%token IF
%token MOD
%token NOT
%token INUMBER
%token FNUMBER
%token OF
%token OR
%token PROCEDURE
%token PROGRAM
%token RECORD
%token STRING
%token TBEGIN
%token THEN
%token TO
%token TYPE
%token VAR
%token WHILE

%token NEQ
%token DOTDOT
%token ASSIGNOP
%token LE
%token GE

%token <opval> ADDOP
%token <opval> MULOP
%token <opval> RELOP
%token <ival> REPEAT
%token <sval> UNTIL
%token <sval> IDENTIFIER
%token <ival> INTEGER
%token <fval> REAL

%type <list> declarations
%type <list> identifier_list

%start program

%right THEN ELSE

%union {
    char* opval;
    int ival;
    float fval;
    char* sval;
    struct List* list;
}

%%

program : PROGRAM IDENTIFIER '(' identifier_list ')' ';' {}

identifier_list : IDENTIFIER                        { $$ = create_list($1); }
                | identifier_list ',' IDENTIFIER    { $$ = $1; append($$, $3); }
                ;

declarations : declarations VAR identifier_list ':' type ';' { $$ = $1; append($$, $5) }

%%

