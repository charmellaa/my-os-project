#include "buddy_allocator.h"
#include <stdio.h>

#define BUFFER_SIZE 512
#define BUDDY_LEVELS 3
#define MEMORY_SIZE 512
#define MIN_BUCKET_SIZE (BUFFER_SIZE>>(BUDDY_LEVELS))

uint8_t buffer[BUFFER_SIZE];
char memory[MEMORY_SIZE];

BuddyAllocator alloc;
int main(int argc, char** argv) {
  printf("Initializing...\n ");
  BuddyAllocator_init(&alloc, BUDDY_LEVELS,
                      buffer,
                      BUFFER_SIZE,
                      memory,
                      MIN_BUCKET_SIZE);
  printf("DONE\n");
  BuddyAllocator* a = &alloc;
  printf("\n----MALLOC----\n");
  void* p1 = BuddyAllocator_malloc(&alloc, 509);
  printf("Address allocated: %p\n", p1);
  BitMap_print(&a->bitmap_tree);
  printf("\n------\n");
  

  void* p2 = BuddyAllocator_malloc(&alloc, 50);
  printf("Address allocated: %p\n", p2);
  BitMap_print(&a->bitmap_tree);
  printf("\n------\n");

  void* p3 = BuddyAllocator_malloc(&alloc,200);
  printf("Address allocated: %p\n", p3);
  BitMap_print(&a->bitmap_tree);
  printf("\n------\n");

  void* p4 = BuddyAllocator_malloc(&alloc,32);
  printf("Address allocated: %p\n", p4);
  BitMap_print(&a->bitmap_tree);
  printf("\n------\n");

  void* p5 = BuddyAllocator_malloc(&alloc, 88);
  printf("Address allocated: %p\n", p5);
  BitMap_print(&a->bitmap_tree);
  printf("\n\n");
 
  printf("\n----FREE----\n");
  BuddyAllocator_free(&alloc, p3);
  BitMap_print(&a->bitmap_tree);
  printf("\n------\n");
  BuddyAllocator_free(&alloc, p5);
  BitMap_print(&a->bitmap_tree);
  printf("\n\n");
  printf("----TEST 3: SUCCESS!----\n");
 

}
