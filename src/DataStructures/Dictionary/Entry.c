#include "Entry.h"
#include <stdlib.h>
#include <string.h>

struct Entry entry_constructor(void* key , int key_size,
                               void* value , int value_size) {

    struct Entry new_entry;
    new_entry.key = malloc(key_size);
    memcpy(new_entry.key , key , key_size);
    new_entry.value = malloc(value_size);
    memcpy(new_entry.value, value , value_size);

    return new_entry;
}

void entry_destructor(struct Entry *entry) {
    free(entry->key);
    free(entry->value);
    free(entry);
} 
