#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 100

// Structure to represent a symbol
typedef struct Symbol {
    char *name;
    char *type;
    struct Symbol *next;
} Symbol;

// Structure to represent a hash table (scope)
typedef struct HashTable {
    Symbol *table[TABLE_SIZE];
} HashTable;

// Structure to represent the stack of hash tables (symbol table)
typedef struct SymbolTable {
    HashTable **scopes;
    int size;       // Current number of scopes
    int capacity;   // Capacity of the stack
} SymbolTable;

// Hash function
unsigned int hash(const char *key) {
    unsigned int hash = 0;
    while (*key) {
        hash = (hash << 5) + *key++;
    }
    return hash % TABLE_SIZE;
}

// Create a new hash table
HashTable *create_hash_table() {
    HashTable *hash_table = (HashTable *)malloc(sizeof(HashTable));
    if (!hash_table) {
        fprintf(stderr, "Error: Memory allocation failed for HashTable.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < TABLE_SIZE; i++) {
        hash_table->table[i] = NULL;
    }
    return hash_table;
}

// Create a new symbol
Symbol *create_symbol(const char *name, const char *type) {
    Symbol *new_symbol = (Symbol *)malloc(sizeof(Symbol));
    if (!new_symbol) {
        fprintf(stderr, "Error: Memory allocation failed for Symbol.\n");
        exit(EXIT_FAILURE);
    }
    new_symbol->name = strdup(name);
    new_symbol->type = strdup(type);
    new_symbol->next = NULL;
    return new_symbol;
}

// Insert a symbol into a hash table
void insert_into_hash_table(HashTable *hash_table, const char *name, const char *type) {
    unsigned int index = hash(name);
    Symbol *new_symbol = create_symbol(name, type);

    if (hash_table->table[index] == NULL) {
        hash_table->table[index] = new_symbol;
    } else {
        Symbol *current = hash_table->table[index];
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_symbol;
    }
}

// Lookup a symbol in a hash table
Symbol *lookup_in_hash_table(HashTable *hash_table, const char *name) {
    unsigned int index = hash(name);
    Symbol *current = hash_table->table[index];
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Free a hash table
void free_hash_table(HashTable *hash_table) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Symbol *current = hash_table->table[i];
        while (current != NULL) {
            Symbol *temp = current;
            current = current->next;
            free(temp->name);
            free(temp->type);
            free(temp);
        }
    }
    free(hash_table);
}

// Create a new symbol table
SymbolTable *create_symbol_table(int initial_capacity) {
    SymbolTable *symbol_table = (SymbolTable *)malloc(sizeof(SymbolTable));
    if (!symbol_table) {
        fprintf(stderr, "Error: Memory allocation failed for SymbolTable.\n");
        exit(EXIT_FAILURE);
    }
    symbol_table->scopes = (HashTable **)malloc(sizeof(HashTable *) * initial_capacity);
    if (!symbol_table->scopes) {
        fprintf(stderr, "Error: Memory allocation failed for scopes.\n");
        exit(EXIT_FAILURE);
    }
    symbol_table->size = 0;
    symbol_table->capacity = initial_capacity;
    return symbol_table;
}

// Push a new scope onto the stack
void push_scope(SymbolTable *symbol_table) {
    if (symbol_table->size == symbol_table->capacity) {
        symbol_table->capacity *= 2;
        symbol_table->scopes = (HashTable **)realloc(
            symbol_table->scopes,
            sizeof(HashTable *) * symbol_table->capacity);
        if (!symbol_table->scopes) {
            fprintf(stderr, "Error: Memory allocation failed during scope resizing.\n");
            exit(EXIT_FAILURE);
        }
    }
    symbol_table->scopes[symbol_table->size++] = create_hash_table();
}

// Pop the top scope off the stack
void pop_scope(SymbolTable *symbol_table) {
    if (symbol_table->size == 0) {
        fprintf(stderr, "Error: No scopes to pop.\n");
        return;
    }
    free_hash_table(symbol_table->scopes[--symbol_table->size]);
}

// Insert a symbol into the current scope
void insert_symbol(SymbolTable *symbol_table, const char *name, const char *type) {
    if (symbol_table->size == 0) {
        fprintf(stderr, "Error: No active scope to insert into.\n");
        return;
    }
    HashTable *current_scope = symbol_table->scopes[symbol_table->size - 1];
    if (lookup_in_hash_table(current_scope, name) != NULL) {
        fprintf(stderr, "Error: Symbol '%s' already exists in the current scope.\n", name);
        return;
    }
    insert_into_hash_table(current_scope, name, type);
}

// Lookup a symbol in the symbol table (search down the stack)
Symbol *lookup_symbol(SymbolTable *symbol_table, const char *name) {
    for (int i = symbol_table->size - 1; i >= 0; i--) {
        Symbol *result = lookup_in_hash_table(symbol_table->scopes[i], name);
        if (result != NULL) {
            return result;
        }
    }
    return NULL;
}

// Free the symbol table
void free_symbol_table(SymbolTable *symbol_table) {
    for (int i = 0; i < symbol_table->size; i++) {
        free_hash_table(symbol_table->scopes[i]);
    }
    free(symbol_table->scopes);
    free(symbol_table);
}

#endif