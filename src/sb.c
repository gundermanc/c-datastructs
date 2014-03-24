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

/* Don't build this module if pointers are disabled, they are required */
#ifdef DATASTRUCT_ENABLE_POINTER



/**
 * Creates new dynamically growing string buffer.
 * blockSize: the number of characters to put in each segment.
 * returns: new string buffer, or NULL if unable to allocate the memory.
 */
SB * sb_new(int blockSize) {
  SB * sb;
  char * block;

  /* allocate struct */
  sb = (SB*)calloc(1, sizeof(SB));
  if(sb == NULL) {
    return NULL;
  }

  /* allocate first text block */
  block = (char*)calloc(blockSize, sizeof(char));
  if(block == NULL) {
    free(sb);
    return NULL;
  }

  /* create linked list */
  sb->list = ll_new();
  if(sb->list == NULL) {
    free(sb);
    free(block);
    return NULL;
  }

  /* append first text buffer segment */
  ll_append_pointer(sb->list, block);
  sb->blockSize = blockSize;
  sb->blockIndex = 0;
  sb->size = 0;

  return sb;
}

/**
 * Frees internal linked list of text segments.
 * sb: a stringbuffer instance.
 */
static void  destroy_list_items(SB * sb) {
  LLIter iterator;
  ll_iter_get(&iterator, sb->list);

  /* iterate through all blocks and free them */
  while(ll_iter_has_next(&iterator)) {
    char * block = (char*)ll_iter_remove(&iterator).pointerVal;
    free(block);
  }
}

/**
 * Frees a stringbuffer instance.
 * sb: a string buffer instance
 */
void sb_free(SB * sb) {
  destroy_list_items(sb);
  ll_free(sb->list);
  free(sb);
}

/**
 * Appends a character to a stringbuffer.
 * sb: a stringbuffer instance.
 * c: a character to append to the buffer.
 */
/* TODO: return false if alloc fails */
void sb_append_char(SB * sb, char c) {
  char * array;

  /* current block is full, alloc new one and add to list */
  if(sb->blockIndex == sb->blockSize) {
    char * newBlock = (char*)calloc(sb->blockSize, sizeof(char));
    ll_append_pointer(sb->list, newBlock);
    sb->blockIndex = 0;
  }

  /* insert char into first empty spot
   * for the record, ll_get_node(.., LL_TAIL) is a constant time operation
   */
  array = (char*)ll_get_node(sb->list, LL_TAIL)->payload.pointerVal;
  array[sb->blockIndex] = c;
  sb->blockIndex++;
  sb->size++;
}

/**
 * Appends a string or byte stream to the buffer.
 * sb: an instance of string buffer
 * string: the string to append.
 * length: the number of characters to append.
 */
/* return false if alloc fails */
void sb_append_str(SB * sb, char * string, size_t length) {
  size_t i;

  /* loop and add each character */
  for(i = 0; i < length; i++) {
    sb_append_char(sb, string[i]);
  }
}

/**
 * Resets the string buffer back to its empty state.
 * TODO: actually implement this function. At the moment it destroys the list
 * and creates a new one.
 * sb: the sb to reset.
 * returns: currently returns a new stringbuffer.
 */
SB * sb_reset(SB * sb) {
  int blockSize = sb->blockSize;
  sb_free(sb);
  return sb_new(blockSize);
}

/**
 * Returns number of characters in the stringbuffer.
 * sb: a stringbuffer instance.
 */
int sb_size(SB * sb) {
  return sb->size;
}

/**
 * Copies the stringbuffer to an external buffer.
 * sb: stringbuffer instance.
 * dst: the destination buffer. Must be sb_size() long, +1 if the null terminator
 * character is to be added as well.
 * dstLen: the length of the destination buffer.
 * nullChar: add a null character to the end of the string?
 * returns: number of characters copied.
 */
int sb_to_buffer(SB * sb, char * dst, size_t dstLen, bool nullChar) {
  LLIter iterator;
  int i = 0;
  int blockIndex = 0;

  ll_iter_get(&iterator, sb->list);

  /* iterate through all character blocks in the string buffer */
  while(ll_iter_has_next(&iterator)) {
    DSValue value;
    char * block;

    /* get payload of characters */
    ll_iter_pop(&iterator, &value);
    block = (char*)value.pointerVal;

    /* copy characters to output buffer */
    while(i < sb->size
	  && i < (nullChar ? (dstLen-1) : dstLen)
	  && blockIndex < sb->blockSize) {
      dst[i] = block[blockIndex];
      blockIndex++;
      i++;
    }
    blockIndex = 0;
  }

  if(nullChar) {
    dst[i] = '\0';
  }

  return i;
}

#endif /* DATASTRUCT_ENABLE_POINTER */
