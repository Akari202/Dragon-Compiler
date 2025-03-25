#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Node;

typedef struct {
    struct Node* left;
    struct Node* right;
    bool operation;
    int value;
} Node;

void level_print_node(Node* node, int level);
void destroy_tree(Node* node);

Node* create_node(int value, bool is_operation) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (node == NULL) {
        perror("Failed to allocate memory");
        exit(1);
    }
    node->left = NULL;
    node->right = NULL;
    node->operation = is_operation;
    node->value = value;
    return node;
}

Node* create_left_child(Node* node, int value, bool is_operation) {
    Node* left_node = create_node(value, is_operation);
    node->left = left_node;
    return left_node;
}

Node* create_right_child(Node* node, int value, bool is_operation) {
    Node* right_node = create_node(value, is_operation);
    node->right = right_node;
    return right_node;
}

void set_left_child(Node* node, Node* child) {
    node->left = child;
}

void set_right_child(Node* node, Node* child) {
    node->right = child;
}

void destroy_tree(Node* node) {
    if (node != NULL) {
        destroy_tree(node->left);
        destroy_tree(node->right);
        free(node);
    }
}

void print_tree(Node* node) {
    printf("%d is op %d\n", node->value, node->operation);
    level_print_node(node->left, 1);
    level_print_node(node->right, 1);
}

void level_print_node(Node* node, int level) {
    if (node == NULL) {
        return;
    }
    /*int indent = level * 4 - 1;*/
    for (int i = 0; i < level; i++) {
        printf("|    ");
    }
    printf(
            "%s%d\n",
            /*"|%.*s%s%d\n",*/
            /*indent,*/
            /*" ",*/
            node->operation ? "Operation: " : "Value: ",
            node->value
            );
    level_print_node(node->left, level + 1);
    level_print_node(node->right, level + 1);
}

#endif
