#include "list.h"

int main() {
    // Create empty list
    List my_list = new_list();

    // Test insert_at_front
    printf("Testing insert at front:\n");
    insert_at_front(&my_list, 10);
    insert_at_front(&my_list, 20);
    insert_at_front(&my_list, 30);
    print_list(&my_list);
    // Expected: 30, 20, 10

    // Destroy and start fresh
    destroy_list(&my_list);

    // Test insert_in_order
    printf("Testing insert in order:\n");
    insert_in_order(&my_list, 15);
    insert_in_order(&my_list, 5);
    insert_in_order(&my_list, 25);
    insert_in_order(&my_list, 1);
    print_list(&my_list);
    // Expected: 1, 5, 15, 25

    // Test delete
    printf("After deleting 5:\n");
    delete_list(&my_list, 5);
    print_list(&my_list);
    // Expected: 1, 15, 25

    // Free memory
    destroy_list(&my_list);
    printf("Done!\n");

    return 0;
}