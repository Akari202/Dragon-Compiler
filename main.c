#include <stdio.h>
#include <stdbool.h>
#include "lex.yy.h"

extern int yyparse();

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: ./dragon <filename>\n");
        return 1;
    }
    yyin = fopen(argv[1], "r");
    /*while (yylex() != 0);*/
    /*while (yyparse() != 0);*/
    yyparse();
    fclose(yyin);
    return 0;
}
