#ifndef LinkedList_h
#define LinkedList_h

#include "../Common/Node.h"



struct LinkedList {
    struct Node *head;
    int length;
    void (*insert)(struct LinkedList *linked_list, int index,
                   void *data, int size);
    void (*remove)(struct LinkedList *linked_list, int index);
    void* (*retreive)(struct LinkedList *linked_list, int index);
    
    void (*sort)(struct LinkedList *linked_list,
                 int (*compare)(void *a , void *b));
    short (*search)(struct LinkedList *linked_list, void *query,
                    int (*compare)(void *a , void *b));
};
struct LinkedList linked_list_constructor(void);
void linked_list_destructor(struct LinkedList *linked_list);




#endif
