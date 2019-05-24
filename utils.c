#include "utils.h"

void *xmalloc(size_t size) {
   void *ptr = malloc(size);
   if (!ptr) {
       perror("xmalloc");
       exit(EXIT_FAILURE);
   }

   return ptr;
}


void *xcalloc(size_t n, size_t size) {

    void *ptr = calloc(n, size);
    if (!ptr) {
        perror("xcalloc");
	exit(EXIT_FAILURE);
    }

    return ptr;
}
