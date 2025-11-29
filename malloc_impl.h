#ifndef MALLOC_IMPL_H
#define MALLOC_IMPL_H

#include <stddef.h> //for size_t (depended on the system)

// function declarations

void *my_malloc(
    size_t size); // void pointer (able to cast to pointer of any data type)
void free(void *ptr);

#endif
