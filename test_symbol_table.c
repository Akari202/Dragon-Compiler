#include "symbol_table.h"

int main() {
    SymbolTable *symbol_table = create_symbol_table(2);

    // Enter global scope
    push_scope(symbol_table);
    insert_symbol(symbol_table, "x", "integer");

    // Enter local scope
    push_scope(symbol_table);
    insert_symbol(symbol_table, "y", "real");

    // Lookup symbols
    Symbol *symbol = lookup_symbol(symbol_table, "x");
    if (symbol) {
        printf("Found symbol: %s, Type: %s\n", symbol->name, symbol->type);
    } else {
        printf("Symbol not found.\n");
    }

    // Exit local scope
    pop_scope(symbol_table);

    // Verify that "y" is no longer accessible
    symbol = lookup_symbol(symbol_table, "y");
    if (symbol) {
        printf("Found symbol: %s, Type: %s\n", symbol->name, symbol->type);
    } else {
        printf("Symbol not found.\n");
    }

    // Free the symbol table
    free_symbol_table(symbol_table);

    return 0;
}