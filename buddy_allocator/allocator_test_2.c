#include "buddy_allocator.h"
#include <stdio.h>

#define BUFFER_SIZE 128
#define BUDDY_LEVELS 2
#define MEMORY_SIZE 128
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
  BuddyAllocator* a = &alloc;
  void* p1 = BuddyAllocator_malloc(&alloc, 10);
  printf("---------------\n");
  void* p2 = BuddyAllocator_malloc(&alloc, 14);
  printf("---------------\n");
  void* p3 = BuddyAllocator_malloc(&alloc, 1);
  printf("---------------\n");
  void* p4 = BuddyAllocator_malloc(&alloc, 20);
  printf("-----------------------\n");
  printf("Bitmap: I should see a tree of 1s\n");
  BitMap_print(&a->bitmap_tree);
  printf("-----------------------\n");
  printf("The following malloc calls should fail\n");
  printf("---------------\n");
  void* p5 = BuddyAllocator_malloc(&alloc, 25);
  printf("---------------\n");
  BuddyAllocator_malloc(&alloc, 128);
  printf("---------------\n Now let's free\n");
  BuddyAllocator_free(&alloc, p1);
  printf("---------------\n");
  BuddyAllocator_free(&alloc, p2);
  printf("---------------\n");
  BuddyAllocator_free(&alloc, p4);
  printf("---------------\n");
  BuddyAllocator_free(&alloc, p3);
  printf("-----------------------\n");
  printf("Bitmap: I should see a tree of 0s\n");
  BitMap_print(&a->bitmap_tree);
  printf("---------------------------\n");
  printf("The following free calls should fail\n");
  printf("---------------\n");
  BuddyAllocator_free(&alloc, p5);
  printf("---------------\n");
  BuddyAllocator_free(&alloc, p2);
  printf("---------------\n");
  printf("TEST 2: SUCCESS!\n");

}
