/**
 * Dynamically Expanding String Buffer
 * (C) 2013 Christian Gunderman
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as 
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program.  If not, see 
 * <http://www.gnu.org/licenses/>.
 *
 * Contact Email: gundermanc@gmail.com 
 */
#include "sb.h"

// instantiates a string buffer made of blocks of
// size blockSize bytes
SB * sb_new(int blockSize) {
  SB * sb = (SB*)malloc(sizeof(SB));
  char * block = (char*)malloc(blockSize);
  sb->list = ll_new(); // initialize linked list
  ll_append_pointer(sb->list, block); // place initial, empty block
  sb->blockSize = blockSize; // set size of each char array in list
  sb->blockIndex = 0; // nothing has been added yet, so next item add at i=0
  sb->size = 0;
  return sb; // return new struct
}

// copies string buffer to string
// destination should be sb_size +1
static void  destroy_list_items(SB * sb) {
  LLIterator iterator;
  ll_iterator_get(&iterator, sb->list); // gets iterator for list

  // iterate through all blocks
  while(ll_iterator_has_next(&iterator)) {
    char * block = (char*)ll_iterator_remove(&iterator).pointerVal; // get object payload
    free(block); // free the object payload
    //ll_iterator_remove(&iterator); // remove and free object
  }
}

// frees memory associated with a string buffer
void sb_free(SB * sb) {
  destroy_list_items(sb); // destroys all items in the linked list
  ll_free(sb->list); // free linked list
  free(sb); // free sb structure
}

// appends a char to a string buffer
void sb_append_c(SB * sb, char c) {
  char * array;

  // current block is full, alloc new one
  if(sb->blockIndex == sb->blockSize) {
    char * newBlock = (char*)malloc(sb->blockSize); // allocate new block
    ll_append_pointer(sb->list, newBlock); // insert new block into linked list
    sb->blockIndex = 0;
  }

  // insert char
  array = (char*)ll_get_node(sb->list, LL_TAIL)->payload.pointerVal;
  array[sb->blockIndex] = c;
  sb->blockIndex++; // increment block index  
  sb->size++; // increment size
}

// appends a string to the specified string buffer
void sb_append_s(SB * sb, char * string, int length) {
  int i;

  for(i = 0; i < length; i++) {
    sb_append_c(sb, string[i]);
  }
}

SB * sb_reset(SB * sb) {
  int blockSize = sb->blockSize;
  sb_free(sb);
  return sb_new(blockSize);
}

// gets length of characters in string buffer
int sb_size(SB * sb) {
  return sb->size;
}

// copies string buffer to string
// destination should be sb_size +1
int sb_to_string(SB * sb, char * dst, int dstLen) {
  LLIterator iterator;
  int i = 0;
  int blockIndex = 0;

  ll_iterator_get(&iterator, sb->list); // gets iterator for list

  // iterate through all blocks
  while(ll_iterator_has_next(&iterator)) {
    DSValue value;
    char * block;

    ll_iterator_pop(&iterator, &value); // get payload of chars
    block = (char*)value.pointerVal;

    // copy each char to buffer
    while(i < sb->size && i < (dstLen-1) && blockIndex < sb->blockSize) {
      dst[i] = block[blockIndex];
      blockIndex++;
      i++;
    }
    blockIndex = 0; // reset index of current block
  }
  dst[i] = '\0'; // replace null terminator
  return i; // returns number of characters copied to buffer
}
