#ifndef Dictionary_h
#define Dictionary_h

#include "../Trees/BinarySearchTree.h"
#include "../Lists/LinkedList.h"

struct Dictionary {
    struct BinarySearchTree binary_search_tree;
    struct LinkedList keys;
};

struct Dictionary Dictionary_constructor(int (*compare)
                                        (void *key_one,
                                        void *key_two));

void Dictionary_destructor(struct Dictionary *dictionary);

void Dictionary_insert(struct Dictionary *dictionary, 
                   void *key , unsigned long key_size,
                   void *value, unsigned long value_size);
void* Dictionary_search(struct Dictionary *dictionary, void *key,
                  unsigned long key_size);
char* Dictionary_print(struct Dictionary *dictionary);

int dict_compare_entry_string_keys(void* entry_one, void* entry_two);
int dict_compare_search_string_keys(void *entry , void *search_string); 
#endif
