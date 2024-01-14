#include "BinarySearchTree.h"
#include <stdlib.h>
#include <string.h>

struct BinarySearchTree BinarySearchTree_constructor(int (*compare)
                                                     (void *data_one,
                                                      void *data_two));

void BinarySearchTree_destructor(struct BinarySearchTree tree);

void* BinarySearchTree_search(struct BinarySearchTree *tree, void *data);
void BinarySearchTree_insert(struct BinarySearchTree *tree, void *data, int size); 


struct Node *BinarySearchTree_create_node(void *data , int size);
void BinarySearchTree_destroy_node(struct Node *node_to_destroy);
struct Node * BinarySearchTree_iterate(struct BinarySearchTree *tree,
                          struct Node *cursor,
                          void *data, int *direction);
void* BinarySearchTree_search(struct BinarySearchTree *tree, void *data);
void BinarySearchTree_insert(struct BinarySearchTree *tree, void *data, int size); 

void recursive_tree_destruction(struct Node *node);

struct BinarySearchTree BinarySearchTree_constructor(int (*compare)
                                                    (void *data_one,
                                                     void *data_two)){
    struct BinarySearchTree tree;
    tree.head = NULL;
    return tree;
}

void BinarySearchTree_destructor(struct BinarySearchTree tree) {
    recursive_tree_destruction(tree.head);
}

struct Node *BinarySearchTree_create_node(void* data, int size) {
    struct Node *new_node = (struct Node*)
                             malloc(sizeof(struct Node));
    *new_node = Node_constructor(data, size);
    return new_node;
}

void BinarySearchTree_destroy_node(struct Node *node_to_destroy) {
    Node_destructor(node_to_destroy);
}

struct Node * BinarySearchTree_iterate(struct BinarySearchTree *tree,
                      struct Node *cursor, void *data,
                      int *direction) {
    int comparison_result = tree->compare(cursor->data,data);
    if (comparison_result == 1) {
        if(cursor->next) {
            return BinarySearchTree_iterate(tree , cursor->next , data , 
                           direction);
        } 
        else {
            *direction = 1;
            return cursor;
        }
    }
    else if(comparison_result == -1) {
        if(cursor->previous) {
            return BinarySearchTree_iterate(tree, cursor->previous , data ,
                           direction);
        }
        else {
            *direction = -1;
            return cursor;
        }
    }
    else {
        *direction = 0;
        return cursor;
    }
}

void * BinarySearchTree_search(struct BinarySearchTree *tree, void *data) {
    int direction = 0;
    struct Node *cursor = BinarySearchTree_iterate(tree , tree->head ,
                                  data , &direction); 
    if(direction == 0) {
        return cursor->data;
    }
    else {
        return NULL;
    }
}


void BinarySearchTree_insert(struct BinarySearchTree *tree , void* data, int size) {
    if(!tree->head) { 
        tree->head = BinarySearchTree_create_node(data,size);
    }
    else {
        int direction = 0;
        struct Node *cursor = BinarySearchTree_iterate(tree , tree->head,
                                  data , &direction);
        if(direction == 1) { 
            cursor->next = BinarySearchTree_create_node(data , size); 
     
            /*printf("\nNode.next created %s: %s\n",(char*)((struct Entry *)
                                                cursor->next->data)->key,
                                                (char*)((struct Entry *)
                                                cursor->next->data)->value);*/
        }
        else if (direction == -1) {
            cursor->previous = BinarySearchTree_create_node(data , size); 
           /* printf("\nNode.previous created %s: %s\n",(char*)((struct Entry *)
                                                    cursor->previous->data)->key,
                                                    (char*)((struct Entry *)
                                                    cursor->previous->data)->value);*/
        }
    }
}


void recursive_tree_destruction(struct Node *node) {
    if(node->previous) {
        recursive_tree_destruction(node->previous);
    }
    if(node->next) {
        recursive_tree_destruction(node->next);
    }
    BinarySearchTree_destroy_node(node);
}

int BinarySearchTree_str_compare(void *data_one, void *data_two)
{
    //printf("starting comparison\n");
    int comparison = strcmp(data_one, data_two);
    
    if (comparison > 0) {
        return 1;
    }
    else if (comparison < 0) {
        return -1;
    }
    else {
        return 0;
    }
}
