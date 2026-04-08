 #include "list.h"

// makes an empty list, just sets head to null
List new_list() {
    List temp;
    temp.head = NULL;
    return temp;
}

// walks through and prints each value
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

// adds new node at the start, easiest and fastest way to insert
void insert_at_front(List *self, int data) {
    ListNodePtr new_node = malloc(sizeof *new_node);
    new_node->data = data;
    new_node->next = self->head;
    self->head = new_node;
}

// inserts in the right spot to keep list sorted
void insert_in_order(List *self, int data) {
    ListNodePtr current = self->head;
    ListNodePtr prev = NULL;

    ListNodePtr new_node = malloc(sizeof *new_node);
    new_node->data = data;
    new_node->next = NULL;

    // find where to put it
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

// removes all nodes that match the value
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

// frees every node, important to avoid memory leaks
void destroy_list(List *self) {
    ListNodePtr current = self->head;
    while (current != NULL) {
        ListNodePtr to_free = current;
        current = current->next;
        free(to_free);
    }
    self->head = NULL;
}

// builds a new list by inserting each element at the front
// so naturally comes out reversed
List reverse(List *self) {
    List result = new_list();
    ListNodePtr current = self->head;
    while (current != NULL) {
        insert_at_front(&result, current->data);
        current = current->next;
    }
    return result;
}

// merges two sorted lists into one sorted list
// goes through both at the same time, picks smaller each step
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
    // add whatever is left over
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

// helper functions for the menu
void option_insert(List *my_list) {
    int val;
    printf("enter a number: ");
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
    print_list(my_list);
}

void list_test() {

    // testing insert at front
    // tutorial said to insert 5,3,7,2,0 and expect 0,2,7,3,5
    printf("Testing insert_at_front... \n");
    List l = new_list();
    insert_at_front(&l, 5);
    insert_at_front(&l, 3);
    insert_at_front(&l, 7);
    insert_at_front(&l, 2);
    insert_at_front(&l, 0);
    printf("Expected: 0, 2, 7, 3, 5 \n");
    printf(" Result: ");
    print_list(&l);
    destroy_list(&l);

    // testing insert in order
    printf("\nTesting insert_in_order... \n");
    List l2 = new_list();
    insert_in_order(&l2, 5);
    insert_in_order(&l2, 1);
    insert_in_order(&l2, 8);
    insert_in_order(&l2, 3);
    insert_in_order(&l2, 3);
    printf("Expected: 1, 3, 3, 5, 8 \n");
    printf(" Result: ");
    print_list(&l2);
    destroy_list(&l2);

    // testing delete - need to check front middle and end cases
    printf("\nTesting delete_list... \n");
    List l3 = new_list();
    insert_in_order(&l3, 1);
    insert_in_order(&l3, 2);
    insert_in_order(&l3, 3);
    insert_in_order(&l3, 4);
    insert_in_order(&l3, 5);

    delete_list(&l3, 1);
    printf("deleted front (1) - Expected: 2, 3, 4, 5\n");
    printf(" Result: ");
    print_list(&l3);

    delete_list(&l3, 3);
    printf("deleted middle (3) - Expected: 2, 4, 5\n");
    printf(" Result: ");
    print_list(&l3);

    delete_list(&l3, 5);
    printf("deleted end (5) - Expected: 2, 4\n");
    printf(" Result: ");
    print_list(&l3);

    // what happens if the value isnt in the list
    delete_list(&l3, 99);
    printf("deleted missing (99) - Expected: 2, 4\n");
    printf(" Result: ");
    print_list(&l3);
    destroy_list(&l3);

    // testing reverse
    printf("\nTesting reverse... \n");
    List l4 = new_list();
    insert_at_front(&l4, 3);
    insert_at_front(&l4, 2);
    insert_at_front(&l4, 1);
    printf("original: ");
    print_list(&l4);
    List rev = reverse(&l4);
    printf("Expected: 3, 2, 1\n");
    printf(" Result: ");
    print_list(&rev);
    destroy_list(&l4);
    destroy_list(&rev);

    // testing merge with two sorted lists
    printf("\nTesting merge... \n");
    List a = new_list();
    List b = new_list();
    insert_in_order(&a, 1);
    insert_in_order(&a, 3);
    insert_in_order(&a, 5);
    insert_in_order(&b, 2);
    insert_in_order(&b, 4);
    insert_in_order(&b, 6);
    printf("list a: ");
    print_list(&a);
    printf("list b: ");
    print_list(&b);
    List m = merge(&a, &b);
    printf("Expected: 1, 2, 3, 4, 5, 6\n");
    printf(" Result: ");
    print_list(&m);
    destroy_list(&a);
    destroy_list(&b);
    destroy_list(&m);
}

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