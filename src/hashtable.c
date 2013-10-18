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
// if you have a problem with the hash function I use, feel free to change it
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

// frees the memory associated with the given node
static void node_free(HashTableNode * node) {
  free(node->key);
  free(node);
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
HashTable * hashtable_new(int tableSize, int blockSize) {
  HashTable * ht = malloc(sizeof(HashTable));

  // alloc array of pointers
  ht->table = malloc(sizeof(HashTableNode*) * tableSize);
  ht->tableSize = tableSize;
  ht->blockSize = blockSize;

  // zero memory
  memset(ht->table, 0, sizeof(HashTableNode*) * ht->tableSize);

  return ht;
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

//
bool hashtable_put(HashTable * ht, void * key, size_t keySize,
		   DSValue * newValue, DSValue * oldValue) {
  int i = hash_to_index(ht, key, keySize);
  bool oldValueExists = false;

  if(ht->table[i] != NULL) {
    oldValueExists = find_value(ht, i, key, keySize, newValue, oldValue, true);
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


  return false;
}

// moves iterator control to the next bucket containing items
// returns false if no more buckets, true if buckets remain
static bool iterator_update(HashTableIterator * i) {

  // advance to next bucket
  i->index++;

  // advance iterator until a list is found
  for(;i->instance->table[i->index] == NULL; i->index++) {

    // if there are no buckets in the list that was found, return error
    if(i->index == i->instance->tableSize) {
      return false; // no more buckets
    }
  }

  // set current bucket to first bucket in list
  i->prevNode = NULL;
  i->currentNode = i->instance->table[i->index];

  return true;
}

// gets an iterator object for this hashtable
void hashtable_iterator_get(HashTable * ht, HashTableIterator * i) {
  // store table in iterator
  i->instance = ht;

  // I really don't like this...but we're starting looking at bucket 0
  // but iterator_update increments index each time its called...
  i->index = -1;  // start looking at bucket 0

  // "advance" iterator to first bucket: 0
  iterator_update(i);
}


// checks to see if any uniterated items remain in the hash table,
// advancing the iterator to the next list with item, if neccessary
// returns true if items remain and false if none remain
bool hashtable_iterator_has_next(HashTableIterator * i) {
  bool hasNext = false;

  // if there are items remaining in current bucket
  if(i->index < i->instance->tableSize
     && i->currentNode != NULL) {
    hasNext = true;
  } else {

    // current bucket has no more items, try next bucket
    if(iterator_update(i) && i->currentNode != NULL) {
      hasNext = true;
    }
  }

  return hasNext;
}

/*
// gets the next item in the hash table, or returns NULL if no items
// remain
HashTableNode * hashtable_iterator_next(HashTableIterator * i) {

  // checks to see if a value exists
  // this call also advances the iterator if neccessary
  if(hashtable_iterator_has_next(i)) {

    // returns the next value
    DSValue unionedValue = i->currentNode->value;
    return unionedValue.pointerVal;
  }

  return NULL;
}
*/

// attempts to remove the next item in the hashtable
// if item exists, it is removed and function returns true
// if keyBuffer != NULL, keyBuffer will receive up to keyBufferLen
// characters of the current key/value pair's key, and if value
// is not null, it will receive the DSValue union of the value.
bool hashtable_iterator_remove(HashTableIterator * i, char * keyBuffer,
			       size_t keyBufferLen, DSValue * value, bool remove) {

  // checks to see if a value exists
  // this call also advances the iterator if neccessary
  if(hashtable_iterator_has_next(i)) {
    HashTableNode * currentNode = i->currentNode;

    // copy out the key buffer
    if(keyBuffer != NULL) {
      strncpy(keyBuffer, currentNode->key, keyBufferLen);
    }

    // copy out the value
    if(value != NULL) {
      memcpy(value, &currentNode->value, sizeof(DSValue));
    }

    i->currentNode = currentNode->next;

    // relink the list to skip current node and free
    if(remove) {
      if(i->prevNode != NULL) {
	i->prevNode->next = currentNode->next;
      }
      node_free(currentNode);
    }

    return true;
  }

  return false;
}

// gets the number of items in the hashtable
int hashtable_size(HashTable * ht) {
  return ht->numItems;
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
