/**
 * Lexie Custom Hash Table Implementation
 * (C) 2013 Christian Gunderman
 * Part of Lexie-C Project
 */

#ifndef HASHTABLE__H__
#define HASHTABLE__H__

#include <stdlib.h>
#include "ll.h"
#include "build_config.h"

// HashTable Item Node
typedef struct tagHashTableNode {
  char * key;
  DSValue value;
}HashTableNode;

// HashTable Structure definition
typedef struct tagHashTable {
  LL ** table;
  int tableSize;
  int blockSize;
  int numItems;
}HashTable;

// HashTable Iterator structure defintion
typedef struct tagHashTableIterator {
  HashTable * table;
  int index;
  LLIterator currentIterator;
}HashTableIterator;

HashTable * hashtable_new(int tableSize);

bool hashtable_put(HashTable * ht, void * key, size_t keySize,
		   DSValue * newValue, DSValue * oldValue);

bool hashtable_get(HashTable * ht, void * key, size_t keySize, DSValue * value);

void hashtable_iterator_get(HashTable * ht, HashTableIterator * i);

bool hashtable_iterator_has_next(HashTableIterator * i);

void hashtable_node_free(HashTableNode * node);

HashTableNode * hashtable_iterator_remove(HashTableIterator * i);

HashTableNode * hashtable_iterator_next(HashTableIterator * i);

int hashtable_size(HashTable * ht);

#endif
