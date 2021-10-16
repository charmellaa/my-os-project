#include "buddy_allocator.h"
#include <stdio.h>

#define BUFFER_SIZE 1024
#define BUDDY_LEVELS 5
#define MEMORY_SIZE 1024
#define MIN_BUCKET_SIZE (BUFFER_SIZE>>(BUDDY_LEVELS))

uint8_t buffer[BUFFER_SIZE];
char memory[MEMORY_SIZE];

BuddyAllocator alloc;
int main(int argc, char** argv) {
  printf("Init...\n ");
  BuddyAllocator_init(&alloc, BUDDY_LEVELS,
                      buffer,
                      BUFFER_SIZE,
                      memory,
                      MIN_BUCKET_SIZE);
  printf("DONE\n");
 printf("-----------------------\n");
 printf("The following malloc calls should end successfully:\n");
 printf("\n");
 void* a = BuddyAllocator_malloc(&alloc, 22);//should be at index 32
 printf("\n");
 void* b = BuddyAllocator_malloc(&alloc, 60); //should be at index 17
 printf("\n");
 void* c = BuddyAllocator_malloc(&alloc, 100); //index 9
 printf("\n");
 void* d = BuddyAllocator_malloc(&alloc, 508); //index 3
 printf("\n");
 void* e = BuddyAllocator_malloc(&alloc, 247); //index 5
 printf("\n");
 printf("-----------------------\n");
 printf("The following malloc calls should fail:\n");
 printf("\n");
 void *f = BuddyAllocator_malloc(&alloc, 10000); //not enough memory
 printf("\n");
 void *g = BuddyAllocator_malloc(&alloc, 124); //no free blocks
 printf("\n");
 printf("-----------------------\n");
 
  printf("The following free calls should end successfully:\n");
  printf("\n");
  BuddyAllocator_free(&alloc, b);
  printf("\n");
  BuddyAllocator_free(&alloc, d);
  printf("\n");
  BuddyAllocator_free(&alloc, a);
  printf("\n");
  BuddyAllocator_free(&alloc, c);
  printf("\n");
  BuddyAllocator_free(&alloc, e);
  printf("-----------------------\n");
  printf("The following free calls should fail:\n");
  printf("\n");
  BuddyAllocator_free(&alloc, a); //already free
  printf("\n");
  BuddyAllocator_free(&alloc, f); 
  printf("\n");
  BuddyAllocator_free(&alloc, g);
  printf("\n");
  printf("---------    TEST 1: SUCCESS!!!    ---------- \n");

 }

