%{
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "dragon.tab.h"

void print_token(const char *token_name, const char *token_value) {
    if (token_value) {
        printf("[TOKEN: %s, VALUE: %s, LINE: %d]\n", token_name, token_value, yylineno);
    } else {
        printf("[TOKEN: %s, LINE: %d]\n", token_name, yylineno);
    }
}

%}

%option yylineno
%option noyywrap

%%

    /* Comments */
"{"([^}]|"\n")*"}"          { /* Ignore comments */ }
"(*"([^*]|"\n")*"*)"        { /* Ignore comments */ }
"//".*                      { /* Ignore comments */ }

    /* Whitespace */
[ \t]                       { /* Ignore whitespace */ }
[\n\r]                      { ++yylineno; }

    /* Numbers */
[0-9]*"."[0-9]+             {
                                yylval.fval = atof(yytext);
                                print_token("FNUM", yytext);
                                return FNUM;
                            }
[0-9]+		                {
                                yylval.ival = atoi(yytext);
                                print_token("INUM", yytext);
                                return INUM;
                            }

    /* Relational Operators */
"<"|">"|"="|"<="|">="|"<>"  {
                                yylval.sval = strdup(yytext);
                                print_token("RELOP", yytext);
                                return RELOP;
                            }

    /* Additive Operators */
"+"|"-"|"or"        	    {
                                yylval.sval = strdup(yytext);
                                print_token("ADDOP", yytext);
                                return ADDOP;
                            }

    /* Multiplicative Operators */
"*"|"/"|"and"|"mod"|"div"	{
                                yylval.sval = strdup(yytext);
                                print_token("MULOP", yytext);
                                return MULOP;
                            }

    /* Assignment Operator */
":="				            {
                                print_token("ASSIGNOP", NULL);
                                return ASSIGNOP;
                            }

    /* Logical NOT */
not				            {
                                print_token("NOT", NULL);
                                return NOT;
                            }

    /* Keywords */
program			            { print_token("PROGRAM", NULL); return PROGRAM; }
function		            { print_token("FUNCTION", NULL); return FUNCTION; }
procedure		            { print_token("PROCEDURE", NULL); return PROCEDURE; }
var				            { print_token("VAR", NULL); return VAR; }
integer                     { print_token("INTEGER", NULL); return INTEGER; }
real                        { print_token("REAL", NULL); return REAL; }
if				            { print_token("IF", NULL); return IF; }
then			            { print_token("THEN", NULL); return THEN; }
else			            { print_token("ELSE", NULL); return ELSE; }
do				            { print_token("DO", NULL); return DO; }
while			            { print_token("WHILE", NULL); return WHILE; }
for				            { print_token("FOR", NULL); return FOR; }
repeat			            { print_token("REPEAT", NULL); return REPEAT; }
until			            { print_token("UNTIL", NULL); return UNTIL; }
begin			            { print_token("BEGIN_TOKEN", NULL); return BEGIN_TOKEN; }
end				            { print_token("END", NULL); return END; }
array			            { print_token("ARRAY", NULL); return ARRAY; }
of				            { print_token("OF", NULL); return OF; }

    /* Double Dot */
"\.\."		                { print_token("DOUBLEDOT", NULL); return DOUBLEDOT; }

    /* Identifiers */
[a-zA-Z][a-zA-Z0-9]*	    {
                                yylval.sval = strdup(yytext);
                                print_token("ID", yytext);
                                return ID;
                            }

    /* Single-character tokens */
";"                         { print_token("SEMICOLON", NULL); return ';'; }
":"                         { print_token("COLON", NULL); return ':'; }
","                         { print_token("COMMA", NULL); return ','; }
"\."                        { print_token("DOT", NULL); return DOT; }
"("                         { print_token("LPAREN", NULL); return '('; }
")"                         { print_token("RPAREN", NULL); return ')'; }
"["                         { print_token("LBRACKET", NULL); return '['; }
"]"                         { print_token("RBRACKET", NULL); return ']'; }

    /* Unrecognized Characters */
.				            {
                                fprintf(stderr, "Unrecognized character: %s at line %d\n", yytext, yylineno);
                                exit(1);
                            }

<<EOF>>                     { print_token("EOF", NULL); return 0; }

%%