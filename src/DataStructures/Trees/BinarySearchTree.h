#ifndef BinarySearchTree_h
#define BinarySearchTree_h

#include "../Common/Node.h"

struct BinarySearchTree {
    struct Node *head;
    int (*compare)(void *data_one , void *data_two);
};
struct BinarySearchTree BinarySearchTree_constructor(int (*compare)
                                                     (void *data_one,
                                                      void *data_two));

void BinarySearchTree_destructor(struct BinarySearchTree tree);

void* BinarySearchTree_search(struct BinarySearchTree *tree, void *data);
void BinarySearchTree_insert(struct BinarySearchTree *tree, void *data, int size); 

int binary_search_tree_int_compare(void *data_one, void *data_two);
int binary_search_tree_float_compare(void *data_one, void *data_two);
int binary_search_tree_char_compare(void *data_one, void *data_two);
int binary_search_tree_str_compare(void *data_one, void *data_two);

#endif
