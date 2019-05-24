#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "hash_table.h"
#include "prime.h"
#include "utils.h"

const int TOP_LIMIT    	       = 70;
const int DOWN_LIMIT   	       = 10;
const int DEFAULT_SIZE 	       = 53;
const int HT_INITIAL_BASE_SIZE = 50;
const int HT_PRIME_1 	       = 421;
const int HT_PRIME_2 	       = 659;

static ht_item HT_DELETED_ITEM = {NULL, NULL};

static ht_item *ht_new_item(const char *key, const char *value) {

   ht_item *i = xmalloc(sizeof(ht_item));
   i->key = strdup(key);
   i->value = strdup(value);

   return i;
}


static ht_hash_table *ht_new_sized(const int base_size) {
    ht_hash_table *ht = xmalloc(sizeof(ht_hash_table));
    ht->base_size = base_size;
    ht->size = next_prime(ht->base_size);
	
    ht->count = 0;
    ht->items = xcalloc((size_t) ht->size, sizeof(ht_item *));

    return ht; 
}

ht_hash_table *ht_new() {

    return ht_new_sized(HT_INITIAL_BASE_SIZE);    
}

static void ht_resize(ht_hash_table *ht, const int base_size) {
    if (base_size < HT_INITIAL_BASE_SIZE) {
        return;
    }

    ht_hash_table *new_ht = ht_new_sized(base_size);
    for (int i = 0; i < ht->size; i++) {
        ht_item *item = ht->items[i];
	if (item != NULL && item != &HT_DELETED_ITEM) {
	    ht_insert(new_ht, item->key, item->value);
	}
    }

    ht->base_size = new_ht->base_size;
    ht->count = new_ht->count;

    const int tmp_size = ht->size;
    ht->size = new_ht->size;
    new_ht->size = tmp_size;

    ht_item **tmp_items = ht->items;
    ht->items = new_ht->items;
    new_ht->items = tmp_items;


    ht_del_hash_table(new_ht);
}

static void ht_resize_up(ht_hash_table *ht) {
    const int new_size = ht->base_size * 2;

    ht_resize(ht, new_size);    
}

static void ht_resize_down(ht_hash_table *ht) {
    const int new_size = ht->base_size / 2;

    ht_resize(ht, new_size);
}


static void ht_del_item(ht_item *it) {

    free(it->key);
    free(it->value);
    free(it);
}

static int hash(const char *s, const int a, const int m) {
    long hash = 0;
    const int len_s = strlen(s);
    for (int i = 0; i < len_s; i++) {
        hash += (long)pow(a, len_s - (i+1)) * s[i];
	hash %= m;
    }

   return (int)hash;
}

static int get_hash(const char *s, const int num_buckets, const int attempt) {
    int hash_a = hash(s, HT_PRIME_1, num_buckets);
    int hash_b = hash(s, HT_PRIME_2, num_buckets);

    // open addressing and double hashing
    return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
}

void ht_insert(ht_hash_table *ht, const char *key, const char *value) {
    const int load = ht->count * 100 / ht->size;
    if (load > TOP_LIMIT) {
        ht_resize_up(ht);
    }

    ht_item *item = ht_new_item(key, value);
    int index = get_hash(item->key, ht->size, 0);
    ht_item *curr_item = ht->items[index];
    int i = 1;
    while (curr_item != NULL) {
	if (curr_item != &HT_DELETED_ITEM) {
	    if (strcmp(curr_item->key, key) == 0) {
	        ht_del_item(curr_item);
		ht->items[index] = item;
		return;
	    }
	}	

        index = get_hash(item->key, ht->size, i);
	curr_item = ht->items[index];
	i++;
    }  

    ht->items[index] = item;
    ht->count++;
}

const char *ht_search(ht_hash_table *ht, const char *key) {
    int index = get_hash(key, ht->size, 0);
    ht_item *item = ht->items[index];
    int i = 1;
    while (item != NULL) {
	if (item != &HT_DELETED_ITEM) {    
            if (strcmp(item->key, key) == 0) {
	        return item->value;
	    }
	}
	index = get_hash(key, ht->size, i);
	item = ht->items[index];
	i++;
    } 


    return NULL;
}

void ht_delete(ht_hash_table *ht, const char *key) {
    const int load = ht->count * 100 / ht->size;
    if (load < DOWN_LIMIT) {
        ht_resize_down(ht);
    }    

    int index = get_hash(key, ht->size, 0);
    ht_item *item = ht->items[index];
    int i = 1;
    while (item != NULL) {
        if (item != &HT_DELETED_ITEM) {
	    if (strcmp(item->key, key) == 0) {
	        ht_del_item(item);
  		ht->items[index] = &HT_DELETED_ITEM;
	    }
	}

	index = get_hash(key, ht->size, i);
	item = ht->items[index];
	i++;
    }

    ht->count--;
}

void ht_del_hash_table(ht_hash_table *ht) {
    
    for (int i = 0; i < ht->size; i++) {
        ht_item *it = ht->items[i];
	if (it != NULL) {
	     ht_del_item(it);
	}

	free(ht->items);
	free(ht);
    }
}




