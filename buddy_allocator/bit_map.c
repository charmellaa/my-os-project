#include <assert.h>
#include "bit_map.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

//returns which level node idx is in
int levelIdx(int idx) {
	return (int)floor(log2(idx));
};

//returns index of node idx's buddy
int buddyIdx(int idx) {
	//checks if idx is even or odd
	if (idx&0x1) {
		return idx-1;
	}
	return idx+1;
};

//returns index of node idx's parent
int parentIdx(int idx) {
	return idx/2;
};

//returns the offset of node idx in his level
int startIdx(int idx) {
	return (idx-(1<<levelIdx(idx)));
};


// returns the number of bytes to store bits booleans
int BitMap_getBytes(int bits){
  return bits/8 + ((bits%8)!=0);
}

// initializes a bitmap on an external array
void BitMap_init(BitMap* bit_map, int num_bits, uint8_t* buffer){
  bit_map->buffer=buffer;
  bit_map->num_bits=num_bits;
  bit_map->buffer_size=BitMap_getBytes(num_bits);
}

// sets a the bit bit_num in the bitmap
// status= 0 or 1
void BitMap_setBit(BitMap* bit_map, int bit_num, int status){
  // get byte
  int byte_num=bit_num>>3;
  assert(byte_num<bit_map->buffer_size);
  int bit_in_byte =7-(bit_num & 0x07);
  if (status) {
    bit_map->buffer[byte_num] |= (1<<bit_in_byte);
  } else {
    bit_map->buffer[byte_num] &= ~(1<<bit_in_byte);
  }
}

// inspects the status of the bit bit_num
int BitMap_bit(const BitMap* bit_map, int bit_num){
  int byte_num=bit_num>>3; 
  assert(byte_num<bit_map->buffer_size);
  int bit_in_byte =7-(bit_num & 0x07);
  return (bit_map->buffer[byte_num] & (1<<bit_in_byte))!=0;
}

//checks all children of idx and returns 1 if they're all free
//otherwise it returns 0
int BitMap_checkChildren(BitMap* bit_map, int idx) {
	int level = 1;
	int child = idx*2; //index of first child
	while (child<bit_map->num_bits) { //checking subchildren
		int firstIdx=child;
		int lastIdx=((1<<(level))-1)+child;
		//scan level
		for (int i = firstIdx; i<=lastIdx; i++) {
			if (BitMap_bit(bit_map, i)){
				//bit = 1 -> occupied
				return 0;
			}
		}
		child = child*2; //index of subchild
		level++;
	}
	//all children are free
	return 1;
}

//prints current state of bitmap //0 if at least one of children is 0; 1 if occupied
void BitMap_print(BitMap* bitmap) {
	int num_bits = bitmap->num_bits;
	int i, j;
	for (i = 1; i<num_bits; i++) {
		//if (i==0) {printf("\n");}
		
		if (i==1<<levelIdx(i)) {
			printf("LEVEL %d: ", levelIdx(i));
		        for (j = 1; j<num_bits/(1<<(levelIdx(i)+1)); j++) {
				printf(" "); }
			}
		if ( i==(1<<(levelIdx(i)+1))-1) {
			printf("%d", BitMap_bit(bitmap, i));
			printf("\n"); }
		else {
			printf("%d", BitMap_bit(bitmap, i));
			for (j=1; j<num_bits/(1<<(levelIdx(i))); j++) {
				printf(" "); }
			}
	}
	printf("\n");

}

void BitMap_print2(BitMap* bitmap) {
	for (int i=0; i<bitmap->num_bits; i++) {
		printf("%d", BitMap_bit(bitmap, i));
	}
	printf("\n");
}

void BitMap_print3(BitMap* bitmap) {
	uint8_t *buffer = bitmap->buffer;
	for(int i = 0; i<bitmap->buffer_size; i++) {
		printf("%d %d\n", i, buffer[i]);
	}
}
	
