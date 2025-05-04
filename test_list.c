#include "list.h"
#include <string.h>

// Comparison function for strings
int compare_strings(void *a, void *b) {
    return strcmp((char *)a, (char *)b);
}

// Callback function to print strings
void print_string(void *data) {
    printf("%s\n", (char *)data);
}

// Free function for strings
void free_string(void *data) {
    free(data);
}

int main() {
    // Create a new list
    List *my_list = list_create();

    // Add some strings to the list
    list_append(my_list, strdup("Alice"));
    list_append(my_list, strdup("Bob"));
    list_append(my_list, strdup("Charlie"));

    // Print the list
    printf("List contents:\n");
    list_traverse(my_list, print_string);

    // Remove an item from the list
    printf("\nRemoving 'Bob'...\n");
    list_remove(my_list, "Bob", compare_strings);

    // Print the list again
    printf("\nList contents:\n");
    list_traverse(my_list, print_string);

    // Free the list
    list_free(my_list, free_string);

    return 0;
}