#ifndef BST_H
#define BST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// BST node stores an owner ID (int)
typedef struct bstNode {
    int owner_id;
    struct bstNode *left;
    struct bstNode *right;
} *BSTNodePtr;

typedef struct bst {
    BSTNodePtr root;
} BST;

BST new_bst();
void insert_bst(BST *self, int owner_id);
int find_bst(BST *self, int owner_id);
void print_bst_inorder(BST *self);
void destroy_bst(BST *self);
int count_bst(BST *self);

#endif
