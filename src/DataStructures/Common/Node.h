#ifndef Node_h
#define Node_h
struct Node {
    void *data;
    struct Node *next;
    struct Node *previous;
};

struct Node Node_constructor(void *data, int size);
void Node_destructor(struct Node *node);

#endif
