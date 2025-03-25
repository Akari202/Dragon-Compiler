#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct List;

typedef struct {
    struct List* up;
    struct List* down;
    char* value;
} List;

void print_list(List* list);
void destroy_list(List* list);
void append(List* list, char* value);

List* create_list(char* value) {
    List* list = (List*)malloc(sizeof(List));
    if (list == NULL) {
        perror("Failed to allocate memory");
        exit(1);
    }
    list->up = NULL;
    list->down = NULL;
    list->value = value;
    return list;
}

void destroy_list(List* list) {
    if (list != NULL) {
        destroy_list(list->up);
        destroy_list(list->down);
        free(list);
    }
}

void append(List* list, char* value) {
    if (list->down != NULL) {
        append(list->down, value);
    } else {
        List* new_list = (List*)malloc(sizeof(List));
        new_list->value = value;
        new_list->up = list;
        new_list->down = NULL;
        list->down = new_list;
    }
}

void print_list(List* list) {
    printf("%s", list->value);
    if (list->up != NULL) {
        print_list(list->up);
    }
    if (list->down != NULL) {
        print_list(list->down);
    }
}

#endif
