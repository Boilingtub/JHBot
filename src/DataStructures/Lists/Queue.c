#include "Queue.h"
#include "LinkedList.h"
#include <unistd.h>

void push(struct Queue *queue, void *data,int size) {
    queue->list.insert(&queue->list,queue->list.length,
                       data,size);
}

void* peek(struct Queue *queue) {
    void *data = queue->list.retreive(&queue->list, 0);
    return data;
}

void pop(struct Queue *queue) {
    queue->list.remove(&queue->list,0);
}

struct Queue queue_constructor() {
    struct Queue queue;
    queue.list = linked_list_constructor();
    
    queue.push = push;
    queue.pop = pop;
    queue.peek = peek;

    return queue;
}
void queue_destructor(struct Queue *queue) {
    linked_list_destructor(&queue->list);

}
