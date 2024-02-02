#include "Queue.h"
#include "LinkedList.h"
#define NULL ((void*) 0)

struct Queue Queue_constructor(void);
void Queue_destructor(struct Queue *queue,
                      void (*node_destruction_method)(struct Node* node));

void Queue_push(struct Queue *queue, void *data,int size); 
void* Queue_peek(struct Queue *queue); 
void Queue_pop(struct Queue *queue,
               void (*node_destruction_method)(struct Node* node));


void Queue_push(struct Queue *queue, void *data,int size) {
    LinkedList_insert(&queue->list,queue->list.length,
                       data,size);
}

void* Queue_peek(struct Queue *queue) {
    void *data = LinkedList_retreive(&queue->list, 0);
    return data;
}

void Queue_pop(struct Queue *queue,
            void (*node_destruction_method)(struct Node* node)) {
    LinkedList_remove(&queue->list,0,NULL);
}

struct Queue Queue_constructor() {
    struct Queue queue;
    queue.list = LinkedList_constructor();
    return queue;
}
void Queue_destructor(struct Queue *queue,
                    void (*node_destruction_method)(struct Node* node)) {
    LinkedList_destructor(&queue->list,NULL);

}
