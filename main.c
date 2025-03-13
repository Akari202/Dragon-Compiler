#include <stdio.h>
#include "lex.yy.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: ./dragon <filename>\n");
        return 1;
    }
    yyin = fopen(argv[1], "r");
    while(1) {
        yylex();
    };
    fclose(yyin);
    return 0;
}
