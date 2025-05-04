#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>

// Doubly linked list node
typedef struct ListNode {
    void *data;
    struct ListNode *next;
    struct ListNode *prev;
} ListNode;

// Doubly linked list structure
typedef struct List {
    ListNode *head;
    ListNode *tail;
    size_t size;
} List;

// Create a new list
List *list_create() {
    List *lst = (List *)malloc(sizeof(List));
    if (!lst) {
        perror("Failed to allocate memory for list");
        exit(1);
    }
    lst->head = NULL;
    lst->tail = NULL;
    lst->size = 0;
    return lst;
}

// Append data to the list
void list_append(List *lst, void *data) {
    ListNode *node = (ListNode *)malloc(sizeof(ListNode));
    if (!node) {
        perror("Failed to allocate memory for node");
        exit(1);
    }
    node->data = data;
    node->next = NULL;
    node->prev = lst->tail;

    if (lst->tail) {
        lst->tail->next = node;
    } else {
        lst->head = node;
    }
    lst->tail = node;
    lst->size++;
}

// Remove a node from the list
void list_remove(List *lst, void *data, int (*cmp_func)(void *, void *)) {
    ListNode *current = lst->head;
    while (current) {
        if (cmp_func(current->data, data) == 0) {
            if (current->prev) {
                current->prev->next = current->next;
            } else {
                lst->head = current->next;
            }

            if (current->next) {
                current->next->prev = current->prev;
            } else {
                lst->tail = current->prev;
            }

            free(current);
            lst->size--;
            return;
        }
        current = current->next;
    }
}

// Traverse the list
void list_traverse(List *lst, void (*callback)(void *)) {
    ListNode *current = lst->head;
    while (current) {
        callback(current->data);
        current = current->next;
    }
}

// Free the list
void list_free(List *lst, void (*free_data)(void *)) {
    ListNode *current = lst->head;
    while (current) {
        ListNode *temp = current;
        current = current->next;

        if (free_data) {
            free_data(temp->data);
        }
        free(temp);
    }
    free(lst);
}

#endif