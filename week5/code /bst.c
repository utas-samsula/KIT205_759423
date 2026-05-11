#include "bst.h"

// empty tree, root is null
BST new_bst() {
    BST temp;
    temp.root = NULL;
    return temp;
}

// recursive find - goes left if smaller, right if bigger
BSTNodePtr find_bst_node(BSTNodePtr self, int n) {
    if (self == NULL || (n == self->data_item)) {
        return self;
    } else if (n < self->data_item) {
        return find_bst_node(self->left, n);
    } else {
        return find_bst_node(self->right, n);
    }
}

BSTNodePtr find_bst(BST *self, int n) {
    return find_bst_node(self->root, n);
}

// recursive insert - walks down until it finds an empty spot
BSTNodePtr insert_bst_node(BSTNodePtr self, int n) {
    if (self == NULL) {
        self = malloc(sizeof *self);
        self->data_item = n;
        self->left = self->right = NULL;
    } else if (n < self->data_item) {
        self->left = insert_bst_node(self->left, n);
    } else if (n > self->data_item) {
        self->right = insert_bst_node(self->right, n);
    }
    return self;
}

void insert_bst(BST *self, int n) {
    self->root = insert_bst_node(self->root, n);
}

// finds the leftmost node in a subtree - used for delete
BSTNodePtr min_node(BSTNodePtr self) {
    BSTNodePtr current = self;
    while (current->left != NULL) {
        current = current->left;
    }
    return current;
}

// delete is trickiest - three cases: no kids, one kid, two kids
BSTNodePtr delete_bst_node(BSTNodePtr self, int n) {
    if (self != NULL) {
        if (n == self->data_item) {
            if (self->left != NULL && self->right != NULL) {
                // two children - replace with in-order successor
                BSTNodePtr successor = min_node(self->right);
                self->data_item = successor->data_item;
                self->right = delete_bst_node(self->right, self->data_item);
            } else {
                // zero or one child
                BSTNodePtr to_free = self;
                if (self->left) {
                    self = self->left;
                } else {
                    self = self->right;
                }
                free(to_free);
            }
        } else if (n < self->data_item) {
            self->left = delete_bst_node(self->left, n);
        } else {
            self->right = delete_bst_node(self->right, n);
        }
    }
    return self;
}

void delete_bst(BST *self, int n) {
    self->root = delete_bst_node(self->root, n);
}

// prints tree structure with brackets so you can see the shape
void print_in_order_bst_node(BSTNodePtr self) {
    if (self == NULL) {
        printf("_");
    } else {
        printf("(");
        print_in_order_bst_node(self->left);
        printf(" %d ", self->data_item);
        print_in_order_bst_node(self->right);
        printf(")");
    }
}

void print_in_order_bst(BST *self) {
    print_in_order_bst_node(self->root);
}

// height helper - returns -1 for empty tree (as per lecture notes)
int height_node(BSTNodePtr self) {
    if (self == NULL) {
        return -1;
    }
    int left_h = height_node(self->left);
    int right_h = height_node(self->right);
    if (left_h > right_h) {
        return left_h + 1;
    } else {
        return right_h + 1;
    }
}

// height of the whole tree
int height_bst(BST *self) {
    return height_node(self->root);
}

// frees everything post-order so children go before parents
void destroy_bst_node(BSTNodePtr self) {
    if (self != NULL) {
        destroy_bst_node(self->left);
        self->left = NULL;
        destroy_bst_node(self->right);
        self->right = NULL;
        free(self);
    }
}

void destroy_bst(BST *self) {
    destroy_bst_node(self->root);
    self->root = NULL;
}

// adhoc test from tutorial - enter numbers, 0 to stop
void bst_adhoc_test() {
    BST tree = new_bst();
    int quit = 0;
    int data;
    while (quit == 0) {
        printf("Enter some data: ");
        scanf("%d", &data);
        if (data != 0) {
            insert_bst(&tree, data);
        } else {
            quit = 1;
        }
    }
    print_in_order_bst(&tree);
    printf("\n");
    destroy_bst(&tree);
}

void bst_test() {

    printf("=== testing insert ===\n");
    BST t = new_bst();
    // inserting the same sequence from the tutorial visualisation
    insert_bst(&t, 1);
    insert_bst(&t, 5);
    insert_bst(&t, 3);
    insert_bst(&t, 9);
    insert_bst(&t, 4);
    insert_bst(&t, 16);
    insert_bst(&t, 12);
    insert_bst(&t, 2);
    printf("tree after inserting 1,5,3,9,4,16,12,2:\n");
    print_in_order_bst(&t);
    printf("\n");
    // in-order should give sorted output
    printf("height: %d\n", height_bst(&t));
    destroy_bst(&t);

    printf("\n=== testing find ===\n");
    BST t2 = new_bst();
    insert_bst(&t2, 10);
    insert_bst(&t2, 5);
    insert_bst(&t2, 15);
    insert_bst(&t2, 3);
    insert_bst(&t2, 7);

    BSTNodePtr found = find_bst(&t2, 7);
    if (found != NULL) {
        printf("found 7 - correct\n");
    } else {
        printf("7 not found - something wrong\n");
    }

    BSTNodePtr not_found = find_bst(&t2, 99);
    if (not_found == NULL) {
        printf("99 not found - correct\n");
    } else {
        printf("found 99 somehow - something wrong\n");
    }
    destroy_bst(&t2);

    printf("\n=== testing delete ===\n");
    BST t3 = new_bst();
    insert_bst(&t3, 10);
    insert_bst(&t3, 5);
    insert_bst(&t3, 15);
    insert_bst(&t3, 3);
    insert_bst(&t3, 7);
    insert_bst(&t3, 12);
    insert_bst(&t3, 20);

    printf("before delete: ");
    print_in_order_bst(&t3);
    printf("\n");

    // delete leaf node
    delete_bst(&t3, 3);
    printf("after deleting leaf (3): ");
    print_in_order_bst(&t3);
    printf("\n");

    // delete node with one child
    delete_bst(&t3, 5);
    printf("after deleting one-child node (5): ");
    print_in_order_bst(&t3);
    printf("\n");

    // delete node with two children - uses in-order successor
    delete_bst(&t3, 15);
    printf("after deleting two-child node (15): ");
    print_in_order_bst(&t3);
    printf("\n");

    // delete root
    delete_bst(&t3, 10);
    printf("after deleting root (10): ");
    print_in_order_bst(&t3);
    printf("\n");

    // try deleting something that isnt there
    delete_bst(&t3, 99);
    printf("after deleting missing (99) - tree unchanged: ");
    print_in_order_bst(&t3);
    printf("\n");
    destroy_bst(&t3);

    printf("\n=== testing height ===\n");
    BST t4 = new_bst();
    printf("empty tree height: %d (expected -1)\n", height_bst(&t4));
    insert_bst(&t4, 5);
    printf("single node height: %d (expected 0)\n", height_bst(&t4));
    insert_bst(&t4, 3);
    insert_bst(&t4, 7);
    printf("three nodes balanced height: %d (expected 1)\n", height_bst(&t4));
    insert_bst(&t4, 1);
    insert_bst(&t4, 2);
    printf("five nodes height: %d (expected 3)\n", height_bst(&t4));
    destroy_bst(&t4);
}