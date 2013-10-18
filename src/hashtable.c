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

#include "hashtable.h"
#include "sha256.h"

#include <stdio.h>

// hashes a key to an integer value
static long hash_to_index(HashTable * ht, char * key, size_t keySize) {
  sha256_context ctx;
  uint8 digest[32];
  int result = 0;

  sha256_starts(&ctx);
  sha256_update(&ctx, (uint8*)key, keySize);
  sha256_finish(&ctx, digest);

  //for(i = 0; i < 32; i++)
  //  result += digest[i];

  result = digest[0] + digest[1] + digest[2];
  return result % ht->tableSize; //TODO: utilize better hashing mechanizm
}

// gets the last item in the HashTable linked list
static HashTableNode * node_list_last(HashTableNode * node) {
  if(node == NULL) {
    return NULL;
  }

  // iterate to last node
  while(node->next != NULL) {
    node = node->next;
  }

  return node;
}

static inline void append_node(HashTableNode * list, HashTableNode * node) {
  HashTableNode * tail = node_list_last(list);
  tail->next = node;
}

static void put_node(HashTable * ht, HashTableNode * node) {
  int i = hash_to_index(ht, node->key, node->keySize);
  // check for pre-existing list at hashed index
  if(ht->table[i] != NULL) {
    append_node(ht->table[i], node);
  } else {
    // create new list head
    ht->table[i] = node;
  }
  node->next = NULL;
  ht->numItems++;
}

static void rehash_table(HashTable * ht, int newSize) {
  int i = 0;
  HashTableNode ** oldTable = ht->table;
  int oldSize = ht->tableSize;

  printf("REHASH!!\n\n");

  // reset table
  ht->tableSize = newSize;
  ht->table = malloc(sizeof(HashTableNode*) * newSize);
  ht->numItems = 0;

  // iterate the list heads
  for(i = 0; i < oldSize; i++) {
    HashTableNode * node = oldTable[i];

    // put each node in the list in its new hash index
    while(node != NULL) {
      HashTableNode * currentNode = node;

      node = node->next;
      put_node(ht, currentNode);
    }
  }
}

static void check_size(HashTable * ht) {
  if(ht->numItems >= ht->tableSize) {
    rehash_table(ht, ht->tableSize + ht->blockSize);
  }
}

// instantiates a new hashtable
HashTable * hashtable_new(int tableSize) {
  HashTable * ht = malloc(sizeof(HashTable));

  // alloc array of pointers
  ht->table = malloc(sizeof(HashTableNode*) * tableSize);
  ht->tableSize = tableSize;
  ht->blockSize = tableSize;

  // zero memory
  memset(ht->table, 0, sizeof(HashTableNode*) * ht->tableSize);

  return ht;
}

static void node_free(HashTableNode * node) {
  free(node->key);
  free(node);
}

// checks the specified linked list for a prexisting value
// if it exists, the value is copied and the function returns true
// otherwise, the function returns false
static bool find_value(HashTable * ht, int i, void * key, size_t keySize,
		       DSValue * newValue, DSValue * oldValue,
		       bool deleteValOnNull) {
  HashTableNode * curNode = ht->table[i];
  HashTableNode * prevNode = NULL;

  while(curNode != NULL) {

    // if the keys are the same
    if(memcmp(key, curNode->key, curNode->keySize < keySize
	      ? curNode->keySize:keySize) == 0) {

      if(oldValue != NULL) {
	memcpy(oldValue, &curNode->value, sizeof(DSValue));
      }

      if(newValue != NULL) {
	memcpy(&curNode->value, newValue, sizeof(DSValue));
      } else if(deleteValOnNull) {

	// newValue is NULL, delete the value
	if(prevNode != NULL) {
	  prevNode->next = curNode->next;
	} else {
	  // at head, delete list
	  ht->table[i] = NULL;
	}
	node_free(curNode);
      }

      return true;
    }
    // advance list
    prevNode = curNode;
    curNode = curNode->next;
  }

  return false;
}


// creates a new HashTableNode
static HashTableNode * node_new(void * key, size_t keySize, DSValue * value,
				HashTableNode * next) {
  HashTableNode * node = malloc(sizeof(HashTableNode));
  memset(node, 0, sizeof(HashTableNode));

  node->key = malloc(keySize);
  memcpy(node->key, key, keySize);
  node->keySize = keySize;
  memcpy(&node->value, value, sizeof(DSValue));
  node->next = next;
  return node;
}

/*
// appends the specified DSValue to the given linked list.
static void append_value(LL * list, void * key, size_t keySize,
			 DSValue * newValue) {
  char * keyString = malloc(keySize + 1);
  HashTableNode * node = malloc(sizeof(HashTableNode));

  memcpy(keyString, key, keySize);
  node->key = keyString;
  memcpy(&node->value, newValue, sizeof(DSValue));
  ll_append_pointer(list, node);
  }*/

// appends the specified DSValue to the given linked list.


//
bool hashtable_put(HashTable * ht, void * key, size_t keySize,
		   DSValue * newValue, DSValue * oldValue) {
  int i = hash_to_index(ht, key, keySize);
  bool oldValueExists = false;

  printf("PUT %i;", i);

  if(ht->table[i] != NULL) {
    oldValueExists = find_value(ht, i, key, keySize, newValue, oldValue,
				true);
  }

  if(!oldValueExists) {
    if(newValue != NULL) {
      put_node(ht, node_new(key, keySize, newValue, NULL));
    }
  }

  check_size(ht);

  return oldValueExists;
  /*
  // check for pre-existing list at hashed index
  if(ht->table[i] == NULL) {
    ht->table[i] = node_new(key, keySize, newValue, NULL);
    ht->numItems++;
    check_size();
  } else {
    oldValueExists = find_value(ht, i, key, keySize, newValue, oldValue,
				true);
    printf("PREXIST : %i", oldValueExists);
    if(newValue == NULL) {
      ht->numItems--;
    } else if(!oldValueExists) {
      append_value(ht->table[i], key, keySize, newValue);
      ht->numItems++;
      check_size();
    }
  }

  return oldValueExists;*/


  /*
  if(ht->table[i] == NULL) {
    // if no linked list at hashed index, make one
    ht->table[i] = ll_new();
  } else {
    oldValueExists = find_value(ht->table[i], key, keySize, newValue, oldValue,
				true);
    if(newValue == NULL) {
      ht->numItems--;
    }
  }

  // if value is not prexisting, we must create a new one
  if(!oldValueExists && newValue != NULL) {
    check_size(ht);
    append_value(ht->table[i], key, keySize, newValue);
    ht->numItems++;
  }

  // returns whether or not oldValue was written
  return oldValueExists;*/
}

// gets a value hashed with specified key from the hashtable. value should be a
// pointer to a DSValue struct that will receive the stored value. function
// returns true if the key exists in the table, and value struct is updated,
// or false if the key and value do not exist.
bool hashtable_get(HashTable * ht, void * key, size_t keySize, DSValue * value) {
  int i = hash_to_index(ht, key, keySize); // calculate array index

  // if there is a linked list at the hashed index
  if(ht->table[i] != NULL) {
    // try to get the value
    return find_value(ht, i, key, keySize, NULL, value, false);
  }

  printf("EMPTY at key: %s\n\n", key);
  return false;
}
/*
// gets next bucket in iterator
// returns false if no more buckets, true if buckets remain
static bool iterator_update(HashTableIterator * i) {
  i->index++; // current bucket is empty, so go to next one

  // keep looking until a non empty array slot is found
  while(i->index < i->table->tableSize && i->table->table[i->index] == NULL)
    i->index++;

  // check and make sure there are buckets left
  if(i->index >= i->table->tableSize)
    return false; // no more buckets

  // set new iterator
  ll_iterator_get(&i->currentIterator, i->table->table[i->index]);

  return true;
  } */

/*
// gets an iterator object for this hashtable
void hashtable_iterator_get(HashTable * ht, HashTableIterator * i) {
  i->table = ht; // store table in iterator
  i->index = -1;  // start looking at bucket 0

  iterator_update(i);
}
*/

/*
// checks to see if any items remain
bool hashtable_iterator_has_next(HashTableIterator * i) {

  // if there are items remaining in current bucket
  if(i->index < i->table->tableSize
     && ll_iterator_has_next(&i->currentIterator))
    return true;
  else {
    // current bucket has no more items, try next bucket
    if(iterator_update(i) && ll_iterator_has_next(&i->currentIterator))
      return true;
  }
  return false;
}

// gets the next item in the hash table, or returns NULL if no items
// remain
HashTableNode * hashtable_iterator_next(HashTableIterator * i) {
  if(hashtable_iterator_has_next(i)) {
    DSValue unionedValue;
    if(ll_iterator_pop(&i->currentIterator, &unionedValue)) {
      return unionedValue.pointerVal;
    }
  }
  return NULL;
}

// removes the next item in the hash table, or returns NULL if no items
// remain
HashTableNode * hashtable_iterator_remove(HashTableIterator * i) {
  if(hashtable_iterator_has_next(i))
    return ll_iterator_remove(&i->currentIterator).pointerVal;
  return NULL;
}
*/
// gets the number of items in the hashtable
int hashtable_size(HashTable * ht) {
  return ht->numItems;
}

// frees the memory associated with the given node
void hashtable_node_free(HashTableNode * node) {
  free(node->key);
  free(node);
}

// frees the specified HashTable Object
void hashtable_free(HashTable * ht) {
  /*int i;

  // free linked lists
  for(i = 0; i < ht->tableSize; i++) {
    if(ht->table[i] != NULL) {
      ll_free(ht->table[i]);
    }
    }*/

  free(ht->table);
  free(ht);
}
