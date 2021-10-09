#include "buddy_allocator.h"
#include <stdio.h>

#define BUFFER_SIZE 256
#define BUDDY_LEVELS 3
#define MEMORY_SIZE 256
#define MIN_BUCKET_SIZE (BUFFER_SIZE>>(BUDDY_LEVELS))

uint8_t buffer[BUFFER_SIZE];
char memory[MEMORY_SIZE];

BuddyAllocator alloc;
int main(int argc, char** argv) {
  printf("init... ");
  int req_size=BuddyAllocator_calcSize(BUDDY_LEVELS);
  printf("size requested for initialization: %d/BUFFER_SIZE\n", req_size);
  BuddyAllocator_init(&alloc, BUDDY_LEVELS,
                      buffer,
                      BUFFER_SIZE,
                      memory,
                      MIN_BUCKET_SIZE);
  printf("DONE\n");
  BuddyAllocator_malloc(&alloc, 20);
  printf("\n------\n");
  BuddyAllocator_malloc(&alloc, 10);
  printf("\n------\n");
  BuddyAllocator_malloc(&alloc,55);
  printf("\n------\n");
  BuddyAllocator_malloc(&alloc,128);
  printf("\n------\n");
  BuddyAllocator_malloc(&alloc, 22);
  printf("\n------\n");
  BuddyAllocator_malloc(&alloc, 22);
  printf("\n------\n");
  BuddyAllocator_malloc(&alloc, 55);
  printf("\n------\n");
  BuddyAllocator_malloc(&alloc, 22);
  printf("\n------\n");
 

}
  
