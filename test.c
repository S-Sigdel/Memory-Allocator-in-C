#include "malloc_impl.h"
#include <stdio.h>
int main() {
  int *test = (int *)my_malloc(sizeof(int));
  *test = 10;
  printf("%d\n", *test);
  printf("%p\n", test);

  my_free(test);
  // testing reallocation to the same memory
  int *newtest = (int *)my_malloc(sizeof(int));
  *newtest = 40;

  printf("%d\n", *newtest);
  printf("%p\n", newtest); // should print out the same memory address

  return 0;
}
