#include "malloc_impl.h"
#include <stdio.h>
int main() {
  int *test = (int *)my_malloc(sizeof(int));
  *test = 10;
  printf("%d", *test);
  printf("%p", test);
}
