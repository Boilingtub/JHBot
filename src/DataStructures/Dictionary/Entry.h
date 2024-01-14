#ifndef Entry_h
#define Entry_h

struct Entry {
    void *key;
    void *value;
};
struct Entry Entry_constructor(void *key, int key_size,
                               void *value, int value_size);
void Entry_destructor(struct Entry *entry);
#endif 
