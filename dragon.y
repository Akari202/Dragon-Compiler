%{

#include <stdlib.h>
#include <stdio.h>

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
%token NUMBER
%token INTEGER
%token OF
%token OR
%token PROCEDURE
%token PROGRAM
%token REAL
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

%start program

%right THEN ELSE

%%

program : PROGRAM {}
        ;


%%

