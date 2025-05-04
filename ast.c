#include "ast.h"

ASTNode *root_ast = NULL;

// Function to create a new AST node
ASTNode *create_ast_node(ASTNodeType type, const char *value) {
    printf("Creating node of type: %s, Value: %s\n", get_ast_node_type_name(type), value ? value : "NULL");
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
    node->parent = NULL;
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
        case AST_TYPE_DECLARATIONS: return "Declarations";
        default: return "Unknown";
    }
}

// Function to add a child to an AST node
void add_child(ASTNode *parent, ASTNode *child) {
    if (!parent) {
        fprintf(stderr, "Error: Cannot add child to a NULL parent.\n");
        return;
    }
    if (!child) {
        fprintf(stderr, "Error: Cannot add a NULL child to parent of type %s.\n", get_ast_node_type_name(parent->type));
        return;
    }

    // Check for self-references
    ASTNode *current = parent;
    while (current) {
        if (current == child) {
            fprintf(stderr, "Error: Cannot add circular reference. Node of type %s is already in the ancestry of parent of type %s.\n",
                    get_ast_node_type_name(child->type), get_ast_node_type_name(parent->type));
            return;
        }
        current = current->parent;
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

// Function to print the AST
void print_ast(ASTNode *root) {
    if (!root) {
        fprintf(stderr, "NULL root node encountered during print.\n");
        return;
    }

    // Stack for iterative traversal
    typedef struct {
        ASTNode *node;
        int depth;
    } StackItem;

    int stack_size = 16;  // Initial stack size
    int stack_top = -1;   // Top of the stack
    StackItem *stack = (StackItem *)malloc(stack_size * sizeof(StackItem));
    if (!stack) {
        fprintf(stderr, "Error: Memory allocation failed for AST stack.\n");
        exit(EXIT_FAILURE);
    }

    // Push the root node onto the stack
    stack[++stack_top] = (StackItem){root, 0};

    while (stack_top >= 0) {
        // Pop the top item from the stack
        StackItem current = stack[stack_top--];
        ASTNode *node = current.node;
        int depth = current.depth;

        // Print the node
        for (int i = 0; i < depth; i++) {
            printf("  ");
        }
        printf("Node Type: %s", get_ast_node_type_name(node->type));
        if (node->value) {
            printf(", Value: %s", node->value);
        }
        printf("\n");

        // Push children onto the stack in reverse order so that
        // they are processed in the correct order
        for (int i = node->child_count - 1; i >= 0; i--) {
            if (stack_top + 1 >= stack_size) {
                stack_size *= 2;  // Double the stack size
                stack = (StackItem *)realloc(stack, stack_size * sizeof(StackItem));
                if (!stack) {
                    fprintf(stderr, "Error: Memory allocation failed for AST stack.\n");
                    exit(EXIT_FAILURE);
                }
            }
            stack[++stack_top] = (StackItem){node->children[i], depth + 1};
        }
    }

    free(stack);  // Free the stack memory
}

// Function to free an AST node and its children
void free_ast(ASTNode *node) {
    if (!node) return;

    for (int i = 0; i < node->child_count; i++) {
        if (node->children[i]) {
            free_ast(node->children[i]);
            node->children[i] = NULL;
        }
    }

    if (node->children) {
        free(node->children);
        node->children = NULL;
    }

    if (node->value) {
        free(node->value);
        node->value = NULL;
    }

    free(node);
}