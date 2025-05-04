#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    AST_TYPE_DECLARATIONS
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

// Function to create a new AST node
ASTNode *create_ast_node(ASTNodeType type, const char *value) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    if (!node) {
        fprintf(stderr, "Error: Memory allocation failed for ASTNode.\n");
        exit(EXIT_FAILURE);
    }
    node->type = type;
    node->value = value ? strdup(value) : NULL;
    node->children = NULL;
    node->child_count = 0;
    node->child_capacity = 0;
    node->parent = NULL;  // Initialize parent to NULL
    return node;
}

// Helper function to get string representation of ASTNodeType
const char *get_ast_node_type_name(ASTNodeType type) {
    switch (type) {
        case AST_TYPE_PROGRAM: return "Program";
        case AST_TYPE_STATEMENT: return "Statement";
        case AST_TYPE_EXPRESSION: return "Expression";
        case AST_TYPE_LITERAL: return "Literal";
        case AST_TYPE_IDENTIFIER: return "Identifier";
        case AST_TYPE_IF: return "If";
        case AST_TYPE_WHILE: return "While";
        case AST_TYPE_FOR: return "For";
        case AST_TYPE_ASSIGNMENT: return "Assignment";
        case AST_TYPE_FUNCTION_CALL: return "Function Call";
        case AST_TYPE_PROCEDURE_CALL: return "Procedure Call";
        case AST_TYPE_COMPOUND: return "Compound";
        case AST_TYPE_RELATIONAL: return "Relational";
        case AST_TYPE_ARITHMETIC: return "Arithmetic";
        case AST_TYPE_ARRAY_ACCESS: return "Array Access";
        default: return "Unknown";
    }
}

void add_child(ASTNode *parent, ASTNode *child) {
    if (!parent) {
        fprintf(stderr, "Error: Cannot add child to a NULL parent.\n");
        return;
    }
    if (!child) {
        fprintf(stderr, "Error: Cannot add a NULL child to parent of type %s.\n", get_ast_node_type_name(parent->type));
        return;
    }

    // Resize the children array if necessary
    if (parent->child_count == parent->child_capacity) {
        parent->child_capacity = parent->child_capacity == 0 ? 2 : parent->child_capacity * 2;
        parent->children = (ASTNode **)realloc(parent->children, sizeof(ASTNode *) * parent->child_capacity);
        if (!parent->children) {
            fprintf(stderr, "Error: Memory allocation failed while resizing children array.\n");
            exit(EXIT_FAILURE);
        }
    }

    // Add the child and set its parent
    parent->children[parent->child_count++] = child;
    child->parent = parent;

    printf("Successfully added child of type %s to parent of type %s.\n",
           get_ast_node_type_name(child->type), get_ast_node_type_name(parent->type));
}

void print_ast(ASTNode *node, int depth) {
    if (!node) {
        fprintf(stderr, "NULL node encountered during print.\n");
        return;
    }

    // Check for circular references
    ASTNode *ancestor = node->parent;
    while (ancestor) {
        if (ancestor == node) {
            fprintf(stderr, "Warning: Circular reference detected at node of type %s.\n",
                    get_ast_node_type_name(node->type));
            return;
        }
        ancestor = ancestor->parent;
    }

    // Indent based on the depth of the node in the tree
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }

    // Print the node type and value
    printf("Node Type: %s", get_ast_node_type_name(node->type));
    if (node->value) {
        printf(", Value: %s", node->value);
    }
    printf("\n");

    // Recursively print the children
    for (int i = 0; i < node->child_count; i++) {
        print_ast(node->children[i], depth + 1);
    }
}

// Function to free an AST node and its children
void free_ast(ASTNode *node) {
    if (!node) return;

    // Free the children
    for (int i = 0; i < node->child_count; i++) {
        free_ast(node->children[i]);
    }

    free(node->children);
    free(node->value);
    free(node);
}

#endif