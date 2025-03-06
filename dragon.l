%{

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "dragon.tab.h"

%}

%option yylineno

%%

    /* ignore comments */
"{"(.|"\n")*"}"             { }
"(*"([^*]|"\n")*"*)"        { }
"//".*                      { }

    /* ignore whitespace */
[ \t]                       { }
[\n\r]                      {
                                printf("\n%d", yylineno);
                            }

    /* number tokens */
[0-9]*"."[0-9]+             {
                                fprintf(stderr, "[NUMBER: %f] ", atof(yytext));
                                return NUMBER;
                            }
[0-9]+		                {
                                fprintf(stderr, "[INTEGER: %d] ", atoi(yytext));
                                return INTEGER;
                            }

    /* operator tokens */
"<"|">"|"="|">="|"<="|"<>"  {
					            printf("[RELOP: %s] ", yytext);
					            return RELOP;
				            }
"+"|"-"|"or"        	    {
					            printf("[ADDOP: %s] ", yytext);
					            return ADDOP;
                            }
"*"|"/"|"and"|"mod"|"div"	{
					            printf("[MULOP: %s] ", yytext);
					            return MULOP;
				            }
:=				            {
					            printf("[ASSIGNOP] ");
					            return ASSIGNOP;
				            }
not				            {
					            printf("[NOT] ");
					            return NOT;
				            }

    /* general tokens */
program			            {
					            printf("[PROGRAM] ");
					            return PROGRAM;
				            }
function		            {
					            printf("[FUNCTION] ");
                                return FUNCTION;
                            }
procedure		            {
					            printf("[PROCEDURE] ");
                                return PROCEDURE;
                            }
var				            {
					            printf("[VAR] ");
                                return VAR;
                            }
..		                    {
					            printf("[DOTDOT] ");
                                return DOTDOT;
                            }

    /* control-flow keyword tokens */
if				            {
					            printf("[IF] ");
                                return IF;
                            }
then			            {
                                printf("[THEN] ");
                                return THEN;
                            }
else			            {
					            printf("[ELSE] ");
                                return ELSE;
                            }
do				            {
					            printf("[DO] ");
                                return DO;
				            }
while			            {
					            printf("[WHILE] ");
                                return WHILE;
                            }
for				            {
					            printf("[FOR] " );
                                return FOR;
                            }
repeat			            {
					            printf("[REPEAT] ");
                                return REPEAT;
                            }
until			            {
					            printf("[UNTIL] ");
                                return UNTIL;
                            }
begin			            {
					            printf("[BEGIN] ");
					            return BEGIN;
				            }
end				            {
                                printf("[END] ");
                                return END;
                            }
real                        {
                                printf("[REAL] ");
                                return END;
                            }

    /* identifier tokens */
[a-zA-Z][a-zA-Z0-9]*	    {
					            printf("[IDENTIFIER: %s]", yytext);
					            return IDENTIFIER;
				            }
"'"[a-zA-Z0-9]*"'"		    {
					            printf("[STRING: %s]", yytext);
                                return STRING;
                            }

    /* default */
.				            {
					            printf("{%c} ", yytext[0]);
					            return yytext[0];
				            }
%%