#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "ast.h"

int main() {
    printf("Testing AST creation...\n");

    // Create the root node for a program
    ASTNode *program_node = create_ast_node(AST_TYPE_PROGRAM, "MainProgram");
    assert(program_node != NULL);
    assert(program_node->type == AST_TYPE_PROGRAM);

    // Create a compound statement node
    ASTNode *compound_stmt = create_ast_node(AST_TYPE_COMPOUND, NULL);
    assert(compound_stmt != NULL);
    assert(compound_stmt->type == AST_TYPE_COMPOUND);

    // Add the compound statement as a child of the program node
    printf("Adding the compound statement as a child of the program node...\n");
    add_child(program_node, compound_stmt);
    assert(program_node->child_count == 1);
    assert(program_node->children[0] == compound_stmt);

    // Create a variable assignment statement
    ASTNode *assign_stmt = create_ast_node(AST_TYPE_ASSIGNMENT, "=");
    ASTNode *var_node = create_ast_node(AST_TYPE_IDENTIFIER, "x");
    ASTNode *literal_node = create_ast_node(AST_TYPE_LITERAL, "42");

    // Add the variable and literal as children of the assignment statement
    printf("Adding variable and literal to the assignment statement...\n");
    add_child(assign_stmt, var_node);
    add_child(assign_stmt, literal_node);

    // Add the assignment statement to the compound statement
    printf("Adding the assignment statement to the compound statement...\n");
    add_child(compound_stmt, assign_stmt);

    // Print the AST
    printf("\nGenerated Abstract Syntax Tree (AST):\n");
    print_ast(program_node, 0);

    // Free the AST
    printf("\nFreeing AST...\n");
    free_ast(program_node);

    printf("AST test completed!\n");
    return 0;
}