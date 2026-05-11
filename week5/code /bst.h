#ifndef BST_H
#define BST_H

#include <stdio.h>
#include <stdlib.h>

typedef struct bstNode {
    int data_item;
    struct bstNode *left;
    struct bstNode *right;
} *BSTNodePtr;

typedef struct bst {
    BSTNodePtr root;
} BST;

BST new_bst();
BSTNodePtr find_bst(BST *self, int n);
void insert_bst(BST *self, int n);
void delete_bst(BST *self, int n);
void print_in_order_bst(BST *self);
void destroy_bst(BST *self);
int height_bst(BST *self);
void bst_adhoc_test();
void bst_test();

#endif