#ifndef Queue_h
#define Queue_h

#include "LinkedList.h"

struct Queue {
    struct LinkedList list;
};

struct Queue Queue_constructor(void);
void Queue_destructor(struct Queue *queue);

void Queue_push(struct Queue *queue, void *data,int size); 
void* Queue_peek(struct Queue *queue); 
void Queue_pop(struct Queue *queue);
#endif
