#include <stdio.h>

#include "hash_table.h"

int main() {
    char kbuf[7];
    char vbuf[7];
    ht_hash_table *ht = ht_new();
    for (int i = 0; i < 80; i++) {
      snprintf(kbuf, 7, "key_%d", i);
      snprintf(vbuf, 7, "val_%d", i);
      
      ht_insert(ht, kbuf, vbuf);  
    }


    printf("%s\n", ht_search(ht, "key_12"));
    printf("%s\n", ht_search(ht, "key_71"));

    ht_del_hash_table(ht);
}
