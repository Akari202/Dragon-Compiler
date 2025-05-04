#include <stdio.h>
#include <stdlib.h>
#include "lex.yy.h"
#include "symbol_table.h"

extern int yyparse();
extern SymbolTable *symbol_table;

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

        symbol_table = create_symbol_table(10);
        if (symbol_table == NULL) {
            perror("Error creating symbol table");
        }

        if (yyparse() != 0) {
            fprintf(stderr, "Error parsing file: %s\n", argv[i]);
        }

        free_symbol_table(symbol_table);
        fclose(yyin);
    }

    return 0;
}