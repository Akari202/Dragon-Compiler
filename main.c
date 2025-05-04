#include <stdio.h>
#include <stdbool.h>
#include "lex.yy.h"

extern int yyparse();

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: ./dragon <filename1> <filename2> ...\n");
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        printf("Processing file: %s\n", argv[i]);

        yyin = fopen(argv[i], "r");
        if (yyin == NULL) {
            perror("Error opening file");
            continue;
        }

        if (yyparse() != 0) {
            fprintf(stderr, "Error parsing file: %s\n", argv[i]);
        }

        fclose(yyin);
    }

    return 0;
}