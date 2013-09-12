/**
 * Lexie Custom Hash Table Implementation
 * (C) 2013 Christian Gunderman
 * Part of Lexie-C Project
 */

#ifndef HASHTABLE__H__
#define HASHTABLE__H__

#include <stdlib.h>
#include "ll.h"

// HashTable Item Node
typedef struct tagHashTableNode {
  char * key;
  void * value;
}HashTableNode;

// HashTable Structure definition
typedef struct tagHashTable {
  LL ** table;
  int tableSize;
}HashTable;

// HashTable Iterator structure defintion
typedef struct tagHashTableIterator {
  HashTable * table;
  int index;
  LLIterator currentIterator;
}HashTableIterator;

HashTable * hashtable_new(int tableSize);
void * hashtable_put(HashTable * ht, void * key, void * value);
void hashtable_free(HashTable * ht);
void * hashtable_get(HashTable * ht, char * key);
void hashtable_iterator_get(HashTable * ht, HashTableIterator * i);
int hashtable_iterator_has_next(HashTableIterator * i);
void hashtable_node_free(HashTableNode * node);
HashTableNode * hashtable_iterator_remove(HashTableIterator * i);
HashTableNode * hashtable_iterator_next(HashTableIterator * i);
#endif
