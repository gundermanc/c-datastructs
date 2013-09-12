/**
 * Lexie Custom Hash Table Implementation
 * (C) 2013 Christian Gunderman
 * Part of Lexie-C Project
 */

#include "hashtable.h"
#include "sha256.h"

#include <stdio.h>

// hashes a key to an integer value
static long hash_to_index(char * key) {
  sha256_context ctx;
  uint8 digest[32];
  int i, result = 0;

  sha256_starts(&ctx);
  sha256_update(&ctx, key, strlen(key));
  sha256_finish(&ctx, digest);

  //for(i = 0; i < 32; i++)
  //  result += digest[i];

  result = digest[0] + digest[1] + digest[2];
  return result; //TODO: utilize better hashing mechanizm
}

// instatiates a new hashtable
HashTable * hashtable_new(int tableSize) {
  HashTable * ht = malloc(sizeof(HashTable)); // allocate hashtable object
  int i;

  ht->table = malloc(sizeof(LL*) * tableSize); // allocate array of pointers
  ht->tableSize = tableSize; // store table size

  // zero memory
  memset(ht->table, 0, sizeof(LL*) * ht->tableSize);

  return ht;
}

void * hashtable_put(HashTable * ht, void * key, void * value) {
  int i = hash_to_index(key) % ht->tableSize; // calculate array index
  LLIterator iterator;
  void * oldValue = 0;

  // if no linked list yet
  if(ht->table[i] == 0)
    ht->table[i] = ll_new(); // init linked list

  ll_iterator_get(&iterator, ht->table[i]); // get iterator for table
  // check for prexisting value
  while(ll_iterator_has_next(&iterator)) {
    HashTableNode * node = ll_iterator_pop(&iterator);
    if(node != 0) {
      if(strcmp(node->key, key) == 0) {
        oldValue = node->value;
	      node->value = value;
      }
    }
  }

  // if not prexisting
  if(oldValue == 0) {
    char * keyString = malloc(strlen(key) + 1);
    HashTableNode *node = malloc(sizeof(HashTableNode));

    strcpy(keyString, key); // copy the key string into a new string
    node->key = keyString;
    node->value = value;
    ll_append(ht->table[i], node);
  }
  return oldValue;
}

void * hashtable_get(HashTable * ht, char * key) {
  int i = hash_to_index(key) % ht->tableSize; // calculate array index
  LLIterator iterator;
  void * value = 0;

  // if no linked list yet
  if(ht->table[i] == 0)
    return 0; // no linked list at index

  ll_iterator_get(&iterator, ht->table[i]); // get iterator for table
  // check for prexisting value
  while(ll_iterator_has_next(&iterator)) {
    HashTableNode * node = ll_iterator_pop(&iterator);
    if(node != 0) {
      if(strcmp(node->key, key) == 0) {
        return node->value; // return stored value
      }
    }
  }

  return 0; // nothing here
}

// gets next bucket in iterator
// returns 0 if no more buckets, non zero if buckets remain
static int iterator_update(HashTableIterator * i) {
  i->index++; // current bucket is empty, so go to next one

  // keep looking until a non empty array slot is found
  while(i->index < i->table->tableSize && i->table->table[i->index] == 0)
    i->index++;

  // check and make sure there are buckets left
  if(i->index >= i->table->tableSize) 
    return 0; // no more buckets

  // set new iterator
  ll_iterator_get(&i->currentIterator, i->table->table[i->index]);
}

// gets an iterator object for this hashtable
void hashtable_iterator_get(HashTable * ht, HashTableIterator * i) {
  i->table = ht; // store table in iterator
  i->index = -1;  // start looking at bucket 0

  iterator_update(i);
}

// checks to see if any items remain
int hashtable_iterator_has_next(HashTableIterator * i) {

  // if there are items remaining in current bucket
  if(i->index < i->table->tableSize && ll_iterator_has_next(&i->currentIterator))
    return 1;
  else {
    // current bucket has no more items, try next bucket
    if(iterator_update(i) && ll_iterator_has_next(&i->currentIterator))
      return 1;
  }
  return 0;
}

// gets the next item in the hash table, or returns 0 if no items
// remain
HashTableNode * hashtable_iterator_next(HashTableIterator * i) {
  if(hashtable_iterator_has_next(i)) 
    return ll_iterator_pop(&i->currentIterator);
  return 0;
}

// removes the next item in the hash table, or returns 0 if no items
// remain
HashTableNode * hashtable_iterator_remove(HashTableIterator * i) {
  if(hashtable_iterator_has_next(i)) 
    return ll_iterator_remove(&i->currentIterator);
  return 0;
}

// frees the memory associated with the given node
void hashtable_node_free(HashTableNode * node) {
  free(node->key);
  free(node);
}

// frees the specified HashTable Object
void hashtable_free(HashTable * ht) {
  int i;
  // free linked lists
  for(i = 0; i < ht->tableSize; i++)
    if(ht->table[i] != 0)
      ll_free(ht->table[i]);

  free(ht->table);
  free(ht);
}
