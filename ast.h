#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Enum for different types of AST nodes
typedef enum {
    AST_TYPE_PROGRAM,
    AST_TYPE_STATEMENT,
    AST_TYPE_EXPRESSION,
    AST_TYPE_LITERAL,
    AST_TYPE_IDENTIFIER,
    AST_TYPE_IF,
    AST_TYPE_WHILE,
    AST_TYPE_FOR,
    AST_TYPE_ASSIGNMENT,
    AST_TYPE_FUNCTION_CALL,
    AST_TYPE_PROCEDURE_CALL,
    AST_TYPE_COMPOUND,
    AST_TYPE_RELATIONAL,
    AST_TYPE_ARITHMETIC,
    AST_TYPE_ARRAY_ACCESS,
    AST_TYPE_DECLARATIONS,
    AST_TYPE_FUNCTION,
    AST_TYPE_PROCEDURE
} ASTNodeType;

// Structure for an AST Node
typedef struct ASTNode {
    ASTNodeType type;           // Type of the AST node
    char *value;                // Value (for literals or identifiers)
    struct ASTNode **children;  // Array of pointers to child nodes
    int child_count;            // Number of children
    int child_capacity;         // Capacity of the children array
    struct ASTNode *parent;     // Pointer to the parent node
} ASTNode;

// Function declarations
ASTNode *create_ast_node(ASTNodeType type, const char *value);
const char *get_ast_node_type_name(ASTNodeType type);
void add_child(ASTNode *parent, ASTNode *child);
void print_ast(ASTNode *node);
void free_ast(ASTNode *node);

extern ASTNode *root_ast;

#endif