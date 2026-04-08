 #include "list.h"

List new_list() {
    List temp;
    temp.head = NULL;
    return temp;
}

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

void insert_at_front(List *self, int data) {
    ListNodePtr new_node = malloc(sizeof *new_node);
    new_node->data = data;
    new_node->next = self->head;
    self->head = new_node;
}

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

void destroy_list(List *self) {
    ListNodePtr current = self->head;
    while (current != NULL) {
        ListNodePtr to_free = current;
        current = current->next;
        free(to_free);
    }
    self->head = NULL;
}

// reverse - builds a new list by inserting each node at front
// so the order flips naturally
List reverse(List *self) {
    List result = new_list();
    ListNodePtr current = self->head;
    while (current != NULL) {
        insert_at_front(&result, current->data);
        current = current->next;
    }
    return result;
}

// merge - takes two sorted lists and combines into one sorted list
// walks both lists simultaneously picking the smaller value each time
List merge(List *list1, List *list2) {
    List merged = new_list();
    ListNodePtr c1 = list1->head;
    ListNodePtr c2 = list2->head;

    while (c1 != NULL && c2 != NULL) {
        if (c1->data <= c2->data) {
            insert_in_order(&merged, c1->data);
            c1 = c1->next;
        } else {
            insert_in_order(&merged, c2->data);
            c2 = c2->next;
        }
    }
    while (c1 != NULL) {
        insert_in_order(&merged, c1->data);
        c1 = c1->next;
    }
    while (c2 != NULL) {
        insert_in_order(&merged, c2->data);
        c2 = c2->next;
    }
    return merged;
}

// option functions for the adhoc menu
void option_insert(List *my_list) {
    int val;
    printf("enter number to insert: ");
    scanf("%d", &val);
    insert_at_front(my_list, val);
}

void option_delete(List *my_list) {
    int val;
    printf("enter number to delete: ");
    scanf("%d", &val);
    delete_list(my_list, val);
}

void option_print(List *my_list) {
    printf("list: ");
    print_list(my_list);
}

// list_test - unit tests for every function including edge cases
void list_test() {
    printf("=== Testing insert_at_front ===\n");
    List l1 = new_list();
    insert_at_front(&l1, 5);
    insert_at_front(&l1, 3);
    insert_at_front(&l1, 7);
    insert_at_front(&l1, 2);
    insert_at_front(&l1, 0);
    printf("Expected: 0, 2, 7, 3, 5\n");
    printf("Result:   ");
    print_list(&l1);
    destroy_list(&l1);

    printf("\n=== Testing insert_in_order ===\n");
    List l2 = new_list();
    insert_in_order(&l2, 5);
    insert_in_order(&l2, 1);
    insert_in_order(&l2, 8);
    insert_in_order(&l2, 3);
    insert_in_order(&l2, 3); // duplicate
    printf("Expected: 1, 3, 3, 5, 8\n");
    printf("Result:   ");
    print_list(&l2);
    destroy_list(&l2);

    printf("\n=== Testing delete_list ===\n");
    List l3 = new_list();
    insert_in_order(&l3, 1);
    insert_in_order(&l3, 2);
    insert_in_order(&l3, 3);
    insert_in_order(&l3, 4);
    insert_in_order(&l3, 5);

    delete_list(&l3, 1); // front
    printf("delete front (1) - Expected: 2, 3, 4, 5\n");
    printf("Result: ");
    print_list(&l3);

    delete_list(&l3, 3); // middle
    printf("delete middle (3) - Expected: 2, 4, 5\n");
    printf("Result: ");
    print_list(&l3);

    delete_list(&l3, 5); // end
    printf("delete end (5) - Expected: 2, 4\n");
    printf("Result: ");
    print_list(&l3);

    delete_list(&l3, 99); // value not in list
    printf("delete missing (99) - Expected: 2, 4\n");
    printf("Result: ");
    print_list(&l3);
    destroy_list(&l3);

    printf("\n=== Testing reverse ===\n");
    List l4 = new_list();
    insert_at_front(&l4, 3);
    insert_at_front(&l4, 2);
    insert_at_front(&l4, 1);
    printf("Original: ");
    print_list(&l4);
    List rev = reverse(&l4);
    printf("Expected: 3, 2, 1\n");
    printf("Result:   ");
    print_list(&rev);
    destroy_list(&l4);
    destroy_list(&rev);

    printf("\n=== Testing merge ===\n");
    List a = new_list();
    List b = new_list();
    insert_in_order(&a, 1);
    insert_in_order(&a, 3);
    insert_in_order(&a, 5);
    insert_in_order(&b, 2);
    insert_in_order(&b, 4);
    insert_in_order(&b, 6);
    printf("List a: ");
    print_list(&a);
    printf("List b: ");
    print_list(&b);
    List m = merge(&a, &b);
    printf("Expected: 1, 2, 3, 4, 5, 6\n");
    printf("Result:   ");
    print_list(&m);
    destroy_list(&a);
    destroy_list(&b);
    destroy_list(&m);
}

// adhoc test - interactive menu driven testing
void list_adhoc_test() {
    List my_list = new_list();
    int quit = 0;

    while (!quit) {
        int option;
        printf("\n0: quit  1: insert  2: delete  3: print\n> ");
        scanf("%d", &option);

        if (option == 0) quit = 1;
        if (option == 1) option_insert(&my_list);
        if (option == 2) option_delete(&my_list);
        if (option == 3) option_print(&my_list);
    }

    destroy_list(&my_list);
}