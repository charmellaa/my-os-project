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


  printf("BUDDY INITIALIZING at address %p\n", memory);
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
  //checking if size requested is bigger than memory available
  if (new_size > memory_size) {
	printf("Not enough memory! Allocating failed.\n");
	return; 
	}
  //if everything's ok, i start at the first level
  int level = 0;
  memory_size=memory_size/2;
  //as long as size is bigger than memory_size,
  //and I don't surpass num_levels
  // I continue to divide
  //at the same time, level increases
  while(new_size<=memory_size && level<alloc->num_levels) {
	level++;
	memory_size=memory_size/2;
  }
  
  printf("Level chosen: %d\n", level);
  //now look for a free spot on the given level
  int spot = BuddyAllocator_getBuddy(alloc, level);
  if (!spot) { //no free spots found 
	return;
  }

}

int BuddyAllocator_getBuddy(BuddyAllocator* alloc, int level) {
  //Checking level size
  int lsize = (1<<(alloc->num_levels-level))*alloc->min_bucket_size;
  printf("Block size: %d\n", lsize);

  //first index of this level
  int firstIdx = 1<<level;
  //and the last one
  int lastIdx = (1<<(level+1))-1;

  //now I have to scan this level to see if i find a free spot
  int scan = firstIdx;
  while(scan <= lastIdx) {
	int count = 0;
	int next;
	int parent;
	if (!BitMap_bit(&alloc->bitmap_tree,scan)) { //current bit is 0 (free spot)
	//I have to check that its parents are free
		parent = parentIdx(scan);
		while (parent>0) {
			if (BitMap_bit(&alloc->bitmap_tree,parent)) { //bit = 1
				//printf("Index %d is occupied\n", parent);
				count++; 
				parent = parentIdx(parent); } //check parents above
			else {
				next = buddyIdx(parent*2); //move to the next subtree
				break;
				}
			}  
		//if parents are free, i check children
		if (count==0 && BitMap_checkChildren(&alloc->bitmap_tree, scan)) { 
		//we allocate and we set its bit to 1
		//if its sibling is also occupied, we set the parents' bit to 1
			setBitOne(&alloc->bitmap_tree,scan);
			printf("SUCCESS: Requested allocation at index: %d\n", scan);
			return scan;
			}
		else {
			if (count!=0 && parent != 0) { 
			//i move to the next subtree of the first parent with bit 0 that i found
				scan = next*(1<<count);
			}
			else {
				scan++; //children aren't free, scan next node
			} } }
	else {
			scan++; //node not available, scan next node
		}
  }
  printf("FAILED: No blocks available.\n");
  return 0;

}
  
	

/*

void BuddyAllocator_releaseBuddy(BuddyAllocator* alloc){
//To do..

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


