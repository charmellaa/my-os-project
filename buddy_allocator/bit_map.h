#pragma once
#include <stdint.h>

// simple bit array
typedef struct  {
  uint8_t *buffer;  // externally allocated buffer
  int buffer_size;
  int num_bits; 
} BitMap;

// returns the number of bytes to store bits booleans
int BitMap_getBytes(int bits);

// initializes a bitmap on an external array
void BitMap_init(BitMap* bit_map, int num_bits, uint8_t* buffer);

// sets a the bit bit_num in the bitmap
// status= 0 or 1
void BitMap_setBit(BitMap* bit_map, int bit_num, int status);

// inspects the status of the bit bit_num
int BitMap_bit(const BitMap* bit_map, int bit_num);

//helpers for bitmap implementation

//checks all children of idx and returns 1 if they're all free
//otherwise it returns 0
int BitMap_checkChildren(BitMap* bit_map, int idx);

//returns which level node idx is in
int levelIdx(int idx);

//returns index of node idx's buddy
int buddyIdx(int idx);

//returns index of node idx's parent
int parentIdx(int idx);

//returns the offset of node idx in his level
int startIdx(int idx);

//prints current state of bitmap: 0 if at least one of children is 0, 1 if occupied
void BitMap_print(BitMap* bitmap);
