#include <stdio.h>
#include <assert.h>
#include <math.h> // for floor and log2
#include "buddy_allocator.h"


// computes the size in bytes for the allocator
int BuddyAllocator_calcSize(int num_levels) {
  int num_bits=1<<(num_levels+1); // maximum number of allocations, used to determine the max list items; 2^(num_levels+1)
  return BitMap_getBytes(num_bits);
}


void BuddyAllocator_init(BuddyAllocator* alloc, //the allocator
                         int num_levels, //number of levels of the b-tree
                         uint8_t* buffer, //the bitmap
                         int buffer_size, //bitmap's size in bytes
                         char* memory,
                         int min_bucket_size) { //size of each blocks 

  // we need room also for level 0
  alloc->num_levels=num_levels;
  alloc->memory=memory;
  alloc->min_bucket_size=min_bucket_size;
  assert (num_levels<MAX_LEVELS);
  // we need enough memory to handle internal structures
  assert (buffer_size>=BuddyAllocator_calcSize(num_levels));


  printf("BUDDY INITIALIZING\n");
  printf("\tlevels: %d", num_levels);
  printf("\tbucket size:%d\n", min_bucket_size);
  printf("\tmanaged memory %d bytes\n", (1<<num_levels)*min_bucket_size);

  int bits_num = 1<<(num_levels+1);
  int bitmap_bytes = BitMap_getBytes(bits_num);

  
  printf("Initializing bitmap...\n");
  printf("\t Number of bits: %d\n", bits_num);
  printf("\t Size: %d bytes\n", bitmap_bytes);
  BitMap_init(&alloc->bitmap_tree, bits_num, buffer);

  printf("Buddy and bitmap initialized successfully!\n");

  
}

void BuddyAllocator_malloc(BuddyAllocator* alloc, int size) {
  printf("Allocating requested size: %d bytes...\n", size);
  int new_size = size + 4; //adding 4 bytes to store the index
  int memory_size = (1<<alloc->num_levels)*alloc->min_bucket_size; 
  //checking if size to be allocated is bigger than memory available
  if (new_size > memory_size) {
	printf("Not enough memory! Breaking..\n");
	return; 
	}
  //if everything's ok
  int level = 0;
  //dividing the memory in half
  memory_size=memory_size/2;
  //until size is bigger than memory_size, I continue to divide
  //at the same time, level increases
  while(size<=memory_size && level<alloc->num_levels) {
	level++;
	memory_size=memory_size/2;
  }
  printf("Level chosen: %d\n", level);
  

  



}


/*int BuddyAllocator_getBuddy(BuddyAllocator* alloc, int level){
  if (level<0)
    return 0;
  assert(level <= alloc->num_levels);

 //To do...

void BuddyAllocator_releaseBuddy(BuddyAllocator* alloc){
//To do..

}

//allocates memory
void* BuddyAllocator_malloc(BuddyAllocator* alloc, int size) {
  // we determine the level of the page
  int mem_size=(1<<alloc->num_levels)*alloc->min_bucket_size;
  int  level=floor(log2(mem_size/(size+8)));

  // if the level is too small, we pad it to max
  if (level>alloc->num_levels)
    level=alloc->num_levels;

  printf("requested: %d bytes, level %d \n",
         size, level);

  // we get a buddy of that size;
  BuddyListItem* buddy=BuddyAllocator_getBuddy(alloc, level);
  if (! buddy)
    return 0;

  // we write in the memory region managed the buddy address
  BuddyListItem** target= (BuddyListItem**)(buddy->start);
  *target=buddy;
  return buddy->start+8;
}
//releases allocated memory
void BuddyAllocator_free(BuddyAllocator* alloc, void* mem) {
  printf("freeing %p", mem);
  // we retrieve the buddy from the system
  char* p=(char*) mem;
  p=p-8;
  BuddyListItem** buddy_ptr=(BuddyListItem**)p;
  BuddyListItem* buddy=*buddy_ptr;
  //printf("level %d", buddy->level);
  // sanity check;
  assert(buddy->start==p);
  BuddyAllocator_releaseBuddy(alloc, buddy);
  
}*/


