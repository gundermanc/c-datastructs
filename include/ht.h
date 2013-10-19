/**
 * Unioned HashTable
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

#ifndef HT__H__
#define HT__H__

#include <stdlib.h>
#include <string.h>
#include "build_config.h"

// HashTable Item Node
typedef struct tagHashTableNode {
  void * key;
  size_t keySize;
  void * next;     /* HashTableNode* */
  DSValue value;
}HashTableNode;

// HashTable Structure definition
typedef struct tagHashTable {
  HashTableNode ** table;
  int tableSize;
  int blockSize;
  float loadFactor;
  int numItems;
}HashTable;

// HashTable Iterator structure defintion
typedef struct tagHashTableIterator {
  HashTable * instance;
  int index;
  HashTableNode * prevNode;
  HashTableNode * currentNode;
}HashTableIterator;

HashTable * ht_new(int tableSize, int blockSize, float loadFactor);

#ifdef DATASTRUCT_ENABLE_BOOL
bool ht_put_bool(HashTable * ht, char * key, bool newValue, bool * oldValue);
#endif // DATASTRUCT_ENABLE_BOOL

#ifdef DATASTRUCT_ENABLE_DOUBLE
bool ht_put_double(HashTable * ht, char * key, double newValue, double * oldValue);
#endif // DATASTRUCT_ENABLE_DOUBLE

#ifdef DATASTRUCT_ENABLE_LONG
bool ht_put_long(HashTable * ht, char * key, long newValue, long * oldValue);
#endif // DATASTRUCT_ENABLE_LONG

#ifdef DATASTRUCT_ENABLE_INT
bool ht_put_int(HashTable * ht, char * key, int newValue, int * oldValue);
#endif // DATASTRUCT_ENABLE_INT

#ifdef DATASTRUCT_ENABLE_SHORT
bool ht_put_short(HashTable * ht, char * key, short newValue, short * oldValue);
#endif // DATASTRUCT_ENABLE_SHORT

#ifdef DATASTRUCT_ENABLE_CHAR
bool ht_put_char(HashTable * ht, char * key, char newValue, char * oldValue);
#endif // DATASTRUCT_ENABLE_CHAR

#ifdef DATASTRUCT_ENABLE_POINTER
bool ht_put_pointer(HashTable * ht, char * key, void * newValue, void ** oldValue);
#endif // DATASTRUCT_ENABLE_POINTER


bool ht_get(HashTable * ht, char * key, DSValue * value);

bool ht_put(HashTable * ht, char * key, DSValue * newValue, DSValue * oldValue) ;

bool ht_put_raw_key(HashTable * ht, void * key, size_t keySize,
		   DSValue * newValue, DSValue * oldValue);

bool ht_get_raw_key(HashTable * ht, void * key, size_t keySize, DSValue * value);

void ht_iter_get(HashTable * ht, HashTableIterator * i);

bool ht_iter_has_next(HashTableIterator * i);

void ht_node_free(HashTableNode * node);

bool ht_iter_remove(HashTableIterator * i, void * keyBuffer,
			       size_t keyBufferLen, DSValue * value,
			       size_t * keyLen, bool remove);

HashTableNode * ht_iter_next(HashTableIterator * i);

int ht_size(HashTable * ht);

void ht_free(HashTable * ht);

#endif
