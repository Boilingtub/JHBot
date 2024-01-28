#ifndef LinkedList_h
#define LinkedList_h

#include "../Common/Node.h"



struct LinkedList {
    struct Node *head;
    int length;
};

struct LinkedList LinkedList_constructor(void);
void LinkedList_destructor(struct LinkedList *linked_list,
                           void (*node_destruction_method)(struct Node* node));

struct Node * LinkedList_iterate(struct  LinkedList *linked_list,
                      int index);
void LinkedList_insert(struct LinkedList *linked_list, 
                 int index ,void* data , int size);
void LinkedList_remove(struct LinkedList *linked_list, int index,
                           void (*node_destruction_method)(struct Node* node));
void* LinkedList_retreive(struct LinkedList *linked_list, int index);
void LinkedList_bubble_sort(struct LinkedList *linked_list,
                    int (*compare)(void* a , void* b));
short LinkedList_binary_search(struct LinkedList *linked_list,
                       void *query, int (*compare)(void* a, void* b));




#endif
