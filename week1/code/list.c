#include "list.h"

// From lecture: constructor - returns empty list
List new_list() {
    List temp;
    temp.head = NULL;
    return temp;
}

// From lecture: traversal - print all nodes
void print_list(List *self) {
    ListNodePtr current = self->head;
    while (current != NULL) {
        printf("%d", current->data);
        current = current->next;
        if (current != NULL)
            printf(", ");
    }
    printf("\n");
}

// From lecture: insert at front (most efficient)
void insert_at_front(List *self, int data) {
    ListNodePtr new_node = malloc(sizeof *new_node);
    new_node->data = data;
    new_node->next = self->head;
    self->head = new_node;
}

// From lecture: insert in sorted order
void insert_in_order(List *self, int data) {
    ListNodePtr current = self->head;
    ListNodePtr prev = NULL;

    ListNodePtr new_node = malloc(sizeof *new_node);
    new_node->data = data;
    new_node->next = NULL;

    while (current != NULL && current->data < data) {
        prev = current;
        current = current->next;
    }

    if (current == self->head) {
        new_node->next = current;
        self->head = new_node;
    } else {
        new_node->next = current;
        prev->next = new_node;
    }
}

// From lecture: delete a value from list
void delete_list(List *self, int data) {
    ListNodePtr current = self->head;
    ListNodePtr prev = NULL;

    while (current != NULL) {
        if (current->data == data) {
            if (prev == NULL) {
                self->head = current->next;
                free(current);
                current = self->head;
            } else {
                prev->next = current->next;
                free(current);
                current = prev->next;
            }
        } else {
            prev = current;
            current = current->next;
        }
    }
}

// From lecture: free all memory
void destroy_list(List *self) {
    ListNodePtr current = self->head;
    while (current != NULL) {
        ListNodePtr to_free = current;
        current = current->next;
        free(to_free);
    }
    self->head = NULL;
}