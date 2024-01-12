#include "Dictionary.h"
#include "Entry.h"
#include <stdlib.h>
#include <stdio.h>

void recursive_dictionary_destroy(struct Node *cursor);
void insert_dict(struct Dictionary *dictionary, 
                   void *key , unsigned long key_size,
                   void *value, unsigned long value_size);
void* search_dict(struct Dictionary *dictionary, void *key,
                  unsigned long key_size);

struct Dictionary dictionary_constructor(int (*compare)(
                                        void *key_one,
                                        void *key_two)) { 
    struct Dictionary dictionary;

    dictionary.binary_search_tree = binary_search_tree_constructor(compare);
    dictionary.keys = linked_list_constructor();
    dictionary.insert = insert_dict;
    dictionary.search = search_dict;
    return dictionary;
}

void dictionary_destructor(struct Dictionary *dictionary) {
    linked_list_destructor(&dictionary->keys);
    recursive_dictionary_destroy(dictionary->binary_search_tree.head);
}

void recursive_dictionary_destroy(struct Node *cursor) {
    if(cursor->previous) {
        recursive_dictionary_destroy(cursor->previous);
    } 
    if (cursor->next) {
        recursive_dictionary_destroy(cursor->next);    
    }
    entry_destructor((struct Entry *)cursor->data);
    free(cursor->data);
    free(cursor);

}

void* search_dict(struct Dictionary *dictionary, void *key,
                   unsigned long key_size) {
    //========= HARD CODING FUNCTION POINTER =========
    dictionary->binary_search_tree.compare = dict_compare_search_string_keys;
    
    void* result = dictionary->binary_search_tree
        .search(&dictionary->binary_search_tree, key);
    
    if(result) {
        return ((struct Entry*)result)->value;
    }
    else {
        return NULL;
    }
}

void insert_dict(struct Dictionary *dictionary, 
                   void *key , unsigned long key_size,
                   void *value, unsigned long value_size) {
    
    struct Entry entry = entry_constructor(key, key_size, value,
                                           value_size);
    dictionary->binary_search_tree.compare = dict_compare_entry_string_keys;
    dictionary->binary_search_tree.insert(&dictionary->binary_search_tree,
                                          &entry, sizeof(entry));
    
    dictionary->keys.insert(&dictionary->keys,
                            dictionary->keys.length,
                            key,
                            key_size);
}

#include <string.h>
int dict_compare_entry_string_keys(void *entry_one, void *entry_two)
{
    if (strcmp( (char *)(((struct Entry *)entry_one)->key),
                (char *)(((struct Entry *)entry_two)->key)) > 0) {
        return 1;
    }
    else 
    if (strcmp( (char *)(((struct Entry *)entry_one)->key),
                (char *)(((struct Entry *)entry_two)->key)) < 0) {
        return -1;
    }
    else
    {
        return 0;
    }
}

int dict_compare_search_string_keys(void *entry , void *search_string) {
    
    if(strcmp((char*)(((struct Entry *)entry)->key),
              (char*)search_string) > 0) {
        return 1;
    }
    else if(strcmp((char*)(((struct Entry *)entry)->key),
              (char*)search_string) < 0) {
        return -1;
    }
    else {
        return 0;
    }

}
