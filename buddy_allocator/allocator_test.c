#include "buddy_allocator.h"
#include <stdio.h>

#define BUFFER_SIZE 512
#define BUDDY_LEVELS 5
#define MEMORY_SIZE 512
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
  void* p1 = BuddyAllocator_malloc(&alloc, 509);
  printf("Address allocated: %p\n", p1);
  //just checking
  //printf("index allocated %d\n", *(int*)((char*)p1 - 4));
  printf("\n------\n");

  void* p2 = BuddyAllocator_malloc(&alloc, 10);
  printf("Address allocated: %p\n", p2);
  //printf("index allocated %d\n", *(int*)((char*)p2 - 4));
  printf("\n------\n");

  void* p3 = BuddyAllocator_malloc(&alloc,10);
  printf("Address allocated: %p\n", p3);
  printf("\n------\n");

  void* p4 = BuddyAllocator_malloc(&alloc,10);
  printf("Address allocated: %p\n", p4);
  printf("\n------\n");

  void* p5 = BuddyAllocator_malloc(&alloc, 22);
  printf("Address allocated: %p\n", p5);
  printf("\n------\n");
 
  void* p6 = BuddyAllocator_malloc(&alloc, 10);
  printf("Address allocated: %p\n", p6);
  printf("\n------\n");

  void* p7 = BuddyAllocator_malloc(&alloc, 10);
  printf("Address allocated: %p\n", p7);
  printf("\n------\n");
 
  void* p = BuddyAllocator_malloc(&alloc, 10);
  printf("Address allocated: %p\n", p);
  printf("\n------\n");
  void* p8 = BuddyAllocator_malloc(&alloc, 22);
  printf("Address allocated: %p\n", p8);
  printf("\n------\n");

  void* p9 = BuddyAllocator_malloc(&alloc, 12);
  printf("Address allocated: %p\n", p9);
  printf("\n------\n");

  void* r = BuddyAllocator_malloc(&alloc, 200);
  printf("Address allocated: %p\n", r);
  printf("\n------\n");

  void* q = BuddyAllocator_malloc(&alloc, 10);
  printf("Address allocated: %p\n", q);
  printf("\n------\n");

  void* s = BuddyAllocator_malloc(&alloc, 50);
  printf("Address allocated: %p\n", s);
  printf("\n------\n");

  void* t = BuddyAllocator_malloc(&alloc, 100);
  printf("Address allocated: %p\n", t);
  printf("\n------\n");

}
