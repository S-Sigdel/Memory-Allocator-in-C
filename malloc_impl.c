#include "malloc_impl.h"
#include <stdio.h>  //printing and debugging
#include <unistd.h> // to use sbrk()

#define ALIGNMENT 8 // memory alignment for aligned data
#define ALIGN_SIZE(size) (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))

typedef struct block_header {
  size_t size;               // total size data+header
  struct block_header *next; // pointer to next
  struct block_header *prev; // pointer to previous
  int is_free;               // 0/1
} header_t;

#define HEADER_SIZE ALIGN_SIZE(sizeof(header_t))

static header_t *global_base = NULL; // pointer to header t = null for now

// first fit to check if there is already a free block to allocate a memory
// without extending the heap
static header_t *find_best_fit(size_t size) {
  header_t *current = global_base;
  while (current) {
    if (current->is_free && current->size >= size) {
      return current;
    }
    current = current->next;
  }
  return NULL; // no fit found
}

static header_t *extend_heap(size_t size) {
  void *request = sbrk(0); // os returns the current heap border
  if (sbrk(size) ==        // heap already expanded in this step
      (void *)-1) {        // cast -1 to void pointer data type before comparing
    return NULL;           // sbrk failed
  } // the brk is already moved during the check and we move on
  header_t *new_block = (header_t *)
      request; // assign the sbrk address to the start of the header_t pointer
  new_block->size = size;
  new_block->is_free = 0;
  // implementation of addding to the headers to the linked list
  if (global_base == NULL) {
    global_base = new_block;
    new_block->prev = NULL;
  } else {
    header_t *last_block = global_base;
    while (last_block->next != NULL) {
      last_block = last_block->next;
    }
    last_block->next = new_block;
    new_block->prev = last_block;
  }

  return new_block;
}

void *my_malloc(size_t size) {
  size_t alligned_size = ALIGN_SIZE(size + HEADER_SIZE);
  if (size == 0) {
    return NULL;
  }
  header_t *header = find_best_fit(
      alligned_size); // find if pre-freed heap is available to reuse (best fit)

  if (header) {
    header->is_free = 0;
    printf("Using best fit");
    return (void *)(header + 1); // return, casted as a void pointer
    //
  }

  header = extend_heap(
      alligned_size); // if the above doesn't run, it extends the heap
  if (!header)
    return NULL;
  printf("Using heap extention");
  return (void *)(header + 1);
}

void my_free(void *ptr) {
  if (ptr == NULL) {
    return; // already freed
  }
  header_t *header =
      (header_t *)ptr - 1; // pointer to start of the header of the usable data
  if (header->is_free) {
    fprintf(stderr, "Warning: trying to free a freed block at %p\n", ptr);
    return;
  }
  header->is_free = 1;
}
