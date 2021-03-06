#include <stdio.h>
#include <assert.h>
#include <math.h> // for floor and log2
#include "buddy_allocator.h"


// computes the size in bytes for the allocator
int BuddyAllocator_calcSize(int num_levels) {
  int num_bits=1<<(num_levels+1); // maximum number of allocations = maximum number of bits
  return BitMap_getBytes(num_bits);
}


void BuddyAllocator_init(BuddyAllocator* alloc,
                         int num_levels,
                         uint8_t* buffer,
                         int buffer_size,
                         char* memory,
                         int min_bucket_size) { 

  // we need room also for level 0
  alloc->num_levels=num_levels;
  alloc->memory=memory;
  alloc->min_bucket_size=min_bucket_size;
  assert (num_levels<MAX_LEVELS);
  // we need enough memory to handle internal structures
  assert (buffer_size>=BuddyAllocator_calcSize(num_levels));


  printf("BUDDY INITIALIZING at address %p\n", memory);
  printf("\tlevels: %d\n", num_levels);
  printf("\tbucket size: %d bytes\n", min_bucket_size);
  printf("\tmanaged memory: %d bytes\n", (1<<num_levels)*min_bucket_size);

  int bits_num = 1<<(num_levels+1);
  int bitmap_bytes = BitMap_getBytes(bits_num);

  
  printf("Initializing bitmap...\n");
  printf("\t Number of bits: %d\n", bits_num);
  printf("\t Size: %d bytes\n", bitmap_bytes);
  BitMap_init(&alloc->bitmap_tree, bits_num, buffer);

  printf("Buddy and bitmap initialized successfully!\n");

  
}

void* BuddyAllocator_malloc(BuddyAllocator* alloc, int size) {
  printf("Allocating requested size: %d bytes...\n", size);
  int new_size = size + 4; //adding 4 bytes to store the index
  int memory_size = (1<<alloc->num_levels)*alloc->min_bucket_size; 
  //checking if size requested is bigger than memory available
  if (new_size > memory_size) {
	printf("FAILED: Not enough memory!\n");
	return NULL; 
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
  //Checking block size on this level
  int block_size = (1<<(alloc->num_levels-level))*alloc->min_bucket_size;
  printf("\tBlock size: %d\n", block_size);
  
  //now look for a free spot on the given level
  int spot = BuddyAllocator_getBuddy(alloc, level);
  if (!spot) { //no free spots found 
	return NULL;
  }

  //address of block chosen 
  int offset = startIdx(spot);
  char* address = alloc->memory + (offset*block_size);
 
  //write the index on the first 4 bytes of the block chosen
  *((int*) address) = spot;

  //we return the address + sizeof(int);
  printf("\t Address of newly allocated block: %p\n", address+4);
  return (void*) (address+4);

}

int BuddyAllocator_getBuddy(BuddyAllocator* alloc, int level) {
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
		while (parent>0) { //until i reach root node
			if (BitMap_bit(&alloc->bitmap_tree,parent)) {
				//found occupied parent
				count++; 
				parent = parentIdx(parent); } //check parents above
			else { parent = parentIdx(parent); } //keep checking
			}  
		//check children
		//if parents & children are free
		if (count==0 && BitMap_checkChildren(&alloc->bitmap_tree, scan)) { 
		// set its bit to 1
		// if its sibling is also occupied, we set the parents' bit to 1
			BitMap_setBit(&alloc->bitmap_tree, scan, 1);
			int p = scan;
			while (p>1) { //until i reach index 1
				if (BitMap_bit(&alloc->bitmap_tree, buddyIdx(p))) {
					BitMap_setBit(&alloc->bitmap_tree, parentIdx(p), 1); 
				}
				else { break; }
				p=parentIdx(p);
				}
			printf("SUCCESS: Requested allocation at index: %d\n", scan);
			return scan;
			}
		else {
			if (count!=0) { // parents not free
			//i move to the next block under the first parent with bit 0 that i found

				//Next index to be scanned will be under that block

				//'children_blocks' is how many blocks of children (or grandchildren)
				// the first free parent i found earlier has on the current level i am in
				//with 'subtree_blocks' the number of blocks in each of its two subtrees
				int children_blocks = 1<<(count+1);
				int subtree_blocks = children_blocks/2;


				//i store in the variable 'next' the free parents' subtree 
				//in which my current index is,starting from 0
				//to do this i just have to divide the current offset by subtree_blocks
				next = startIdx(scan)/subtree_blocks; // it can only be 0 or 1

				//calculate the new offset of the next index to be scanned
				int new_offset = (next+1)*subtree_blocks;

				//now, i can calculate the new index
				scan = firstIdx+new_offset;
				
				//if new 'scan' is greater than lastIdx, 
				//i should be exiting this huge while loop
			}
			else {
				scan++; //children not free, scan next node
			} 
		} 
	}
	else {
		scan++; //node not available, scan next node
		}
  }
  printf("FAILED: No blocks available.\n");
  return 0;

}
  
	



void BuddyAllocator_releaseBuddy(BuddyAllocator* alloc, int index) {
	
	//check if the given address has bit = 0
	//either it's a free block or only its parent is occupied
	//if it is, something definitely went wrong
        if (!BitMap_bit(&alloc->bitmap_tree, index)) {
		printf("ERROR: You're trying to free a block that has bit 0 in the bitmap...\n");
		return;
	}

	//just to make sure, check if children are free
	assert(BitMap_checkChildren(&alloc->bitmap_tree,index));

	//we can set the bit to 0
	BitMap_setBit(&alloc->bitmap_tree, index, 0);

	//then we set the parent and grandparents' bit to 0
	int p = parentIdx(index);
	while (p>=1) { //until i reach root node
		if (!BitMap_bit(&alloc->bitmap_tree, p)) {break;} //found a free parent->done
		BitMap_setBit(&alloc->bitmap_tree, p, 0); 
		p=parentIdx(p);
	}

	printf("SUCCESS: You have now freed the block at index %d.\n", index);
	return;

}


//releases allocated memory
void BuddyAllocator_free(BuddyAllocator* alloc, void* mem) {
	
	//check if mem is a valid address
	if (!mem) {
		printf("Address not valid.\n");
		return; 
		}
	//printf("mem is %p\n", mem);
	
	char* original_address = (char*) mem;
	//I retrieve the original address by subtracting 4 bytes
	original_address=original_address-4;
	//and the index of buddy that needs to be freed
 	int index =  *((int*) original_address);
	//check if it's a valid index
	assert(index<(1<<((alloc->num_levels)+1)));

	printf("Buddy to be freed: address: %p, with index: %d\n", mem, index);

        BuddyAllocator_releaseBuddy(alloc, index);

	 
} 


