#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "ast.h"

int main() {
    Node* root = create_node(1, true);

    // Level 1
    create_left_child(root, 2, false);
    create_right_child(root, 3, true);

    // Level 2
    create_left_child(root->left, 4, true);
    create_right_child(root->left, 5, false);
    create_left_child(root->right, 6, false);
    create_right_child(root->right, 7, true);

    print_tree(root);
    destroy_tree(root);
    return 0;
}
