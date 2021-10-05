#include <assert.h>
#include "bit_map.h"
#include <math.h>


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
  return bits/8 + (bits%8)!=0;
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
  int bit_in_byte=byte_num&0x03;
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
  int bit_in_byte=byte_num&0x03;
  return (bit_map->buffer[byte_num] & (1<<bit_in_byte))!=0;
}
