#include "Dictionary.h"
#include "Entry.h"
#include <stdlib.h>

void Dictionary_destructor(struct Dictionary *dictionary);

void Dictionary_insert(struct Dictionary *dictionary, 
                   void *key , unsigned long key_size,
                   void *value, unsigned long value_size);
void* Dictionary_search(struct Dictionary *dictionary, void *key,
                  unsigned long key_size);
char* Dictionary_print(struct Dictionary *dictionary);

int dict_compare_entry_string_keys(void* entry_one, void* entry_two);
int dict_compare_search_string_keys(void *entry , void *search_string); 

void recursive_dictionary_destroy(struct Node *cursor);

struct Dictionary Dictionary_constructor(int (*compare)(
                                        void *key_one,
                                        void *key_two)) { 
    struct Dictionary dictionary;

    dictionary.binary_search_tree = BinarySearchTree_constructor(compare);
    dictionary.keys = LinkedList_constructor();
    return dictionary;
}

void Dictionary_destructor(struct Dictionary *dictionary) {
    LinkedList_destructor(&dictionary->keys);
    recursive_dictionary_destroy(dictionary->binary_search_tree.head);
}

void recursive_dictionary_destroy(struct Node *cursor) {
    if(cursor->previous) {
        recursive_dictionary_destroy(cursor->previous);
    } 
    if (cursor->next) {
        recursive_dictionary_destroy(cursor->next);    
    }
    Entry_destructor((struct Entry *)cursor->data);
    free(cursor);

}

void* Dictionary_search(struct Dictionary *dictionary, void *key,
                   unsigned long key_size) {
    //========= HARD CODING FUNCTION POINTER =========
    dictionary->binary_search_tree.compare = dict_compare_search_string_keys;
    
    void* result = BinarySearchTree_search(&dictionary->binary_search_tree,
                                           key);
    
    if(result) {
        return ((struct Entry*)result)->value;
    }
    else {
        return NULL;
    }
}

void Dictionary_insert(struct Dictionary *dictionary, 
                   void *key , unsigned long key_size,
                   void *value, unsigned long value_size) {
    
    struct Entry entry = Entry_constructor(key, key_size+1, value,
                                           value_size+1);
    //#include <stdio.h>
    //printf("key = %s : value = %s\n",(char*)key,(char*)value);
    
    dictionary->binary_search_tree.compare = dict_compare_entry_string_keys;
    BinarySearchTree_insert(&dictionary->binary_search_tree,
                                          &entry, sizeof(entry));
    LinkedList_insert(&dictionary->keys,
                            dictionary->keys.length,
                            key,
                            key_size);

    
}



#include <string.h>
char* Dictionary_print(struct Dictionary *dictionary) { 
    char* buffer = malloc(10) ;
    strcpy(buffer,"Dict -> \n");
    for(int i= 0;i<dictionary->keys.length;i++) {
        
        char* key = (char*)LinkedList_retreive(&dictionary->keys,i);
        char* value = (char*)Dictionary_search(dictionary,key,sizeof(char[strlen(key)]));
        unsigned int key_value_pair_size = sizeof(char[strlen(key)])+sizeof(char[strlen(value)])+3;
        char key_value_pair[key_value_pair_size];
        strcpy(key_value_pair,key);
        strcat(key_value_pair,": ");
        strcat(key_value_pair,value);
        strcat(key_value_pair,"\n");
        buffer = realloc(buffer,sizeof(char[strlen(buffer)])+key_value_pair_size+2);
        strcat(buffer,key_value_pair);
    }
    return buffer;
}

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
