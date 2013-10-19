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

#ifndef HASHTABLE__H__
#define HASHTABLE__H__

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

HashTable * hashtable_new(int tableSize, int blockSize, float loadFactor);

bool hashtable_put(HashTable * ht, void * key, size_t keySize,
		   DSValue * newValue, DSValue * oldValue);

bool hashtable_get(HashTable * ht, void * key, size_t keySize, DSValue * value);

void hashtable_iterator_get(HashTable * ht, HashTableIterator * i);

bool hashtable_iterator_has_next(HashTableIterator * i);

void hashtable_node_free(HashTableNode * node);

bool hashtable_iterator_remove(HashTableIterator * i, void * keyBuffer,
			       size_t keyBufferLen, DSValue * value,
			       size_t * keyLen, bool remove);

HashTableNode * hashtable_iterator_next(HashTableIterator * i);

int hashtable_size(HashTable * ht);

void hashtable_free(HashTable * ht);

#endif
