#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 100

typedef struct Symbol {
    char *name;
    char *type;
    struct Symbol *next;
} Symbol;

typedef struct HashTable {
    Symbol *table[TABLE_SIZE];
} HashTable;

typedef struct SymbolTable {
    HashTable **scopes;
    int size;
    int capacity;
} SymbolTable;

// Declare functions (no function bodies)
unsigned int hash(const char *key);
HashTable *create_hash_table();
void free_hash_table(HashTable *hash_table);
Symbol *create_symbol(const char *name, const char *type);
void insert_into_hash_table(HashTable *hash_table, const char *name, const char *type);
Symbol *lookup_in_hash_table(HashTable *hash_table, const char *name);

SymbolTable *create_symbol_table(int initial_capacity);
void push_scope(SymbolTable *symbol_table);
void pop_scope(SymbolTable *symbol_table);
void insert_symbol(SymbolTable *symbol_table, const char *name, const char *type);
Symbol *lookup_symbol(SymbolTable *symbol_table, const char *name);
void free_symbol_table(SymbolTable *symbol_table);
void print_symbol_table(SymbolTable *symbol_table);

// Declare the global variable as extern
extern SymbolTable *symbol_table;

#endif