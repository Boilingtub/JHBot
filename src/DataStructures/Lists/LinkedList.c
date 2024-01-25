#include "LinkedList.h"
#include "../Common/Node.h"
#include <stdio.h>
#include <stdlib.h>

struct Node * LinkedList_create_node(void* data, int size);
void LinkedList_destroy_node(struct Node *node_to_destroy);

struct LinkedList LinkedList_constructor(void);
void LinkedList_destructor(struct LinkedList *linked_list);

struct Node * LinkedList_iterate(struct  LinkedList *linked_list,
                      int index);
void LinkedList_insert(struct LinkedList *linked_list, 
                 int index ,void* data , int size);
void LinkedList_remove(struct LinkedList *linked_list, int index);
void* LinkedList_retreive(struct LinkedList *linked_list, int index);
void LinkedList_bubble_sort(struct LinkedList *linked_list,
                    int (*compare)(void* a , void* b));
short LinkedList_binary_search(struct LinkedList *linked_list,
                       void *query, int (*compare)(void* a, void* b));


struct LinkedList LinkedList_constructor() {
    struct LinkedList new_list;
    new_list.head = NULL;
    new_list.length = 0; 
    return new_list;
}

void LinkedList_destructor(struct LinkedList *linked_list) {
    for(int i = 0; i < linked_list->length;) {
        LinkedList_remove(linked_list,0);
    }
}

struct Node *LinkedList_create_node(void* data, int size) {
    struct Node *new_node = (struct Node*)
                             malloc(sizeof(struct Node));
    *new_node = Node_constructor(data, size);
    return new_node;
}

void LinkedList_destroy_node(struct Node *node_to_destroy) {
    Node_destructor(node_to_destroy);
}

struct Node * LinkedList_iterate(struct LinkedList *linked_list,int index) {
    if (index < 0 || index >= linked_list->length) {
        printf("\nIndex out of bound... on linked list\n");
        return NULL;
    }

    struct Node *cursor = linked_list->head;
    for (int i = 0; i < index; i++) {
        cursor = cursor->next;
    }
    return cursor;
}

void LinkedList_insert(struct LinkedList *linked_list,
                 int index, void* data, int size) {
    size = size + 2;
    struct Node *node_to_insert = LinkedList_create_node(data, size);
    if(index == 0) {
        node_to_insert->next = linked_list->head;
        linked_list->head = node_to_insert;
    } else {
        struct Node *cursor = LinkedList_iterate(linked_list, index -1);
        node_to_insert->next = cursor->next;
        cursor->next = node_to_insert;
    }
    linked_list->length += 1;
}

void LinkedList_remove(struct LinkedList *linked_list, int index) {
    //printf("removing -> %s\n",(char*)LinkedList_retreive(linked_list,index));
    if(index == 0) { 
        struct Node *node_to_remove = linked_list->head;
        if(node_to_remove) {
            linked_list->head = node_to_remove->next;
            LinkedList_destroy_node(node_to_remove);
        }
    } 
    else {
        struct Node *cursor = LinkedList_iterate(linked_list, index -1);
        struct Node *node_to_remove = cursor->next;
        cursor->next = node_to_remove->next;
        LinkedList_destroy_node(node_to_remove);
    }
    
    linked_list->length -= 1;
}

void* LinkedList_retreive(struct LinkedList *linked_list, int index) {
    struct Node *cursor = LinkedList_iterate(linked_list, index);
    if(cursor) 
        return cursor->data;
    else
        return NULL;
}

void LinkedList_bubble_sort(struct LinkedList *linked_list,
                    int (*compare)(void *a, void *b)) {
    for(struct Node *i = LinkedList_retreive(linked_list, 0);i;i = i->next){
        for(struct Node *n = i->next; n; n=n->next) {
            if(compare(i->data, n->data) > 0) {
                void *temporary = n->data;
                n->data = i->data;
                i->data = temporary;
            } 
        }
    }
}

short LinkedList_binary_search(struct LinkedList *linked_list, void *query,
                       int (*compare)(void *a , void *b)) {
    int position = linked_list->length/2;
    int min_checked =0;
    int max_checked = linked_list->length;

    while(max_checked > min_checked) {
        void *data = LinkedList_retreive(linked_list, position);
        if(compare(data, query) == 1) {
            max_checked = position;
            if(position != (min_checked + position)/2)
                position = (min_checked + position)/2;
            else
                break;
        }
        else if (compare(data, query) == -1) {
            min_checked = position;
            if(position != (max_checked + position)/2)
                position = (max_checked +position)/2;
            else
                break;
        }
        else {
            return 1;
        }
    }
    return 0;
}
