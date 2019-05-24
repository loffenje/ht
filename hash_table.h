#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stddef.h>

typedef struct ht_item_t {
    char *key;
    char *value;
} ht_item;

typedef struct ht_hash_table {
    int size;
    int count;
    size_t base_size;
    ht_item **items;
} ht_hash_table;


ht_hash_table *ht_new();
void ht_insert(ht_hash_table *ht, const char *key, const char *value);
const char *ht_search(ht_hash_table *ht, const char *key);
void ht_delete(ht_hash_table *ht, const char *key);
void ht_del_hash_table(ht_hash_table *ht);

#endif
