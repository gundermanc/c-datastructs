/**
 * Unioned Dynamically Expanding HashTable
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

#include "ht.h"
#include "lookup3.h"
#include <stdio.h>

/**
 * Hashes a key to an integer value
 * If you have a problem with the hash function I use, feel free to change it.
 * ht: the context of the HashTable
 * key: the key to hash
 * keySize: the length of the key to hash, in bytes.
 */
static long hash_to_index(HashTable * ht, void * key, size_t keySize) {

  /*
   * Hashes index using Bob Jenkins Lookup3.c. Obviously, this is not
   * as complex a hash as Sha256, but Sha and other high end hashes are
   * not neccessary for something as small scale as a hash table.
   * for more info, read Bob Jenkins webpage on hashing algorithms.
   * <http://burtleburtle.net/bob/hash/doobs.html>
   */
  return hashlittle(key,  keySize, 0) % ht->tableSize;
}

/**
 * Frees a node struct
 * node: the node to free.
 */
static void node_free(HashTableNode * node) {
  free(node->key);
  free(node);
}

/**
 * Gets the last node in a linked list of nodes.
 * node: the starting node in the list
 * returns: The the last node in the list
 */
static HashTableNode * node_list_last(HashTableNode * node) {
  if(node == NULL) {
    return NULL;
  }

  /* iterate to last node */
  while(node->next != NULL) {
    node = node->next;
  }

  return node;
}

/**
 * Iterates to the end of list and appends node.
 * list: list that will receive node.
 * node: node that will be appended.
 */
static inline void append_node(HashTableNode * list, HashTableNode * node) {
  HashTableNode * tail = node_list_last(list);
  tail->next = node;
}

/**
 * Hashes the key in the node structure and then places the node in the
 * linked list at the hashed index.
 * ht: the instance of the HashTable to add the node to.
 * node: the node to add to the hashtable.
 */
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

/**
 * Rehashes the specified HashTable to the new array size.
 * ht: the instance of hashtable to modify.
 * newSize: the new length for the hashtable array.
 */
static void rehash_table(HashTable * ht, size_t newSize) {
  int i = 0;
  HashTableNode ** oldTable = ht->table;
  size_t oldSize = ht->tableSize;

  /* reset table */
  ht->tableSize = newSize;
  ht->table = calloc(sizeof(HashTableNode*), newSize);
  ht->numItems = 0;

  /* iterate the list heads */
  for(i = 0; i < oldSize; i++) {
    HashTableNode * node = oldTable[i];

    // put each node in the list in its new hash index
    while(node != NULL) {
      HashTableNode * currentNode = node;

      node = node->next;
      put_node(ht, currentNode);
    }
  }

  free(oldTable);
}

/**
 * Checks the loading of the hashtable. If hashtable is loaded beyond load
 * factor, rehash_table is called, expanding table by ht->blockSize.
 * ht: the hashtable instance.
 */
static void check_load_factor(HashTable * ht) {
  if(ht->numItems >= (ht->tableSize * ht->loadFactor)) {
    rehash_table(ht, ht->tableSize + ht->blockSize);
  }
}

/**
 * Creates a new hashtable.
 * tableSize: initial size of the hashtable's array. (number of "buckets")
 * blockSize: the number by which tableSize will be increased each time table is
 * rehashed.
 * loadFactor: A value defining a percentage of load upon which the hashtable
 * will be expanded. For example, if the hashtable has loadFactor of 0.75f and
 * is 100 buckets in size, it will automatically add another blockSize buckets
 * when 75 of the 100 buckets are full.
 * returns: pointer to a new HashTable struct.
 */
HashTable * ht_new(int tableSize, int blockSize, float loadFactor) {
  HashTable * ht = calloc(1, sizeof(HashTable));

  /* alloc array of pointers */
  ht->table = calloc(1, sizeof(HashTableNode*) * tableSize);
  ht->tableSize = tableSize;
  ht->blockSize = blockSize;
  ht->loadFactor = loadFactor;

  return ht;
}

/**
 * Checks the specified linked list for a prexisting value.
 * ht: the instance of hashtable.
 * i: the index of the linked list in which to look.
 * key: the key pertaining to the value that we are looking for.
 * keySize: the number of bytes in the key buffer that are to be
 * used as the key.
 * newValue: the new value to set for the specified key, or NULL for none.
 * oldValue: pointer to a buffer to recv. the previous value.
 * deleteValOnNull: If newValue is NULL, delete the current value hashed
 * to key.
 * returns: true if the value existed and false otherwise.
 */
static bool find_value(HashTable * ht, int i, void * key, size_t keySize,
		       DSValue * newValue, DSValue * oldValue,
		       bool deleteValOnNull) {
  HashTableNode * curNode = ht->table[i];
  HashTableNode * prevNode = NULL;

  /* while tokens remain, keep going */
  while(curNode != NULL) {

    /* if the keys are the same */
    if(memcmp(key, curNode->key, curNode->keySize < keySize
	      ? curNode->keySize:keySize) == 0) {

      /* if oldValue buffer is provided, copy previous value to it */
      if(oldValue != NULL) {
	memcpy(oldValue, &curNode->value, sizeof(DSValue));
      }

      /* if newValue is provided, store it */
      if(newValue != NULL) {
	memcpy(&curNode->value, newValue, sizeof(DSValue));
      } else if(deleteValOnNull) {

	/* newValue is NULL, delete the value */
	if(prevNode != NULL) {
	  prevNode->next = curNode->next;
	} else {

	  /* at head, delete list */
	  ht->table[i] = NULL;
	}
	node_free(curNode);
      }

      return true;
    }
    /* advance list */
    prevNode = curNode;
    curNode = curNode->next;
  }

  return false;
}

/**
 * Creates a new hashtable node.
 * key: the key to store in the new node.
 * keySize: the number of bytes from key to store in the node.
 * value: the value to copy into the node.
 * returns: a new node.
 */
static HashTableNode * node_new(void * key, size_t keySize, DSValue * value,
				HashTableNode * next) {
  HashTableNode * node = calloc(1, sizeof(HashTableNode));

  node->key = calloc(keySize, 1);
  memcpy(node->key, key, keySize);
  node->keySize = keySize;
  memcpy(&node->value, value, sizeof(DSValue));
  node->next = next;
  return node;
}

/**
 * Stores a value in the hashtable using a buffer of raw bytes as the key.
 * ht: the hashtable instance.
 * key: the key which the value will be hashted to
 * keySize: The number of bytes from key that will be used for the key.
 * newValue: A pointer to a new value to store.
 * oldValue: A buffer that will recv. the old value hashed to this key. Pass
 * null if you don't care about the old value.
 * return: true if oldValue contains a value previously stored at the specified
 * key, or false if no value was stored at this key.
 */
bool ht_put_raw_key(HashTable * ht, void * key, size_t keySize,
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

  /* expand table if neccessary */
  check_load_factor(ht);

  return oldValueExists;
}

/**
 * Stores a value in the hashtable using a null terminated string as the key.
 * ht: the hashtable instance.
 * key: the key which the value will be hashted to
 * newValue: A pointer to a new value to store.
 * oldValue: A buffer that will recv. the old value hashed to this key. Pass
 * null if you don't care about the old value.
 * return: true if oldValue contains a value previously stored at the specified
 * key, or false if no value was stored at this key.
 */
bool ht_put(HashTable * ht, char * key, DSValue * newValue, DSValue * oldValue) {
  return ht_put_raw_key(ht, key, strlen(key) + 1, newValue, oldValue);
}

/**
 * Puts a bool in the hashtable.
 * key: key to hash this value to.
 * newValue: The value to store.
 * oldValue: A pointer to a variable that will recv. the value previously stored
 * at this key.
 * returns: true if the given key previous had a value, and/or it was returned
 * in the oldValue variable, and false if it did not.
 */
#ifdef DATASTRUCT_ENABLE_BOOL
bool ht_put_bool(HashTable * ht, char * key, bool newValue, bool * oldValue) {
  DSValue newDSValue;
  DSValue oldDSValue;
  bool oldValueExists = false;
  newDSValue.boolVal = newValue;
  oldValueExists = ht_put(ht, key, &newDSValue, &oldDSValue);

  /* if pointer for old value was given, output to it */
  if(oldValue != NULL) {
    *oldValue = oldDSValue.boolVal;
  }
  return oldValueExists;
}
#endif // DATASTRUCT_ENABLE_BOOL

/**
 * Puts a double in the hashtable.
 * key: key to hash this value to.
 * newValue: The value to store.
 * oldValue: A pointer to a variable that will recv. the value previously stored
 * at this key.
 * returns: true if the given key previous had a value, and/or it was returned
 * in the oldValue variable, and false if it did not.
 */
#ifdef DATASTRUCT_ENABLE_DOUBLE
bool ht_put_double(HashTable * ht, char * key, double newValue, double * oldValue) {
  DSValue newDSValue;
  DSValue oldDSValue;
  bool oldValueExists = false;
  newDSValue.doubleVal = newValue;
  oldValueExists = ht_put(ht, key, &newDSValue, &oldDSValue);

  /* if pointer for old value was given, output to it */
  if(oldValue != NULL) {
    *oldValue = oldDSValue.doubleVal;
  }
  return oldValueExists;
}
#endif // DATASTRUCT_ENABLE_DOUBLE

/**
 * Puts a long in the hashtable.
 * key: key to hash this value to.
 * newValue: The value to store.
 * oldValue: A pointer to a variable that will recv. the value previously stored
 * at this key.
 * returns: true if the given key previous had a value, and/or it was returned
 * in the oldValue variable, and false if it did not.
 */
#ifdef DATASTRUCT_ENABLE_LONG
bool ht_put_long(HashTable * ht, char * key, long newValue, long * oldValue) {
  DSValue newDSValue;
  DSValue oldDSValue;
  bool oldValueExists = false;
  newDSValue.longVal = newValue;
  oldValueExists = ht_put(ht, key, &newDSValue, &oldDSValue);

  /* if pointer for old value was given, output to it */
  if(oldValue != NULL) {
    *oldValue = oldDSValue.longVal;
  }
  return oldValueExists;
}
#endif // DATASTRUCT_ENABLE_LONG

/**
 * Puts an int in the hashtable.
 * key: key to hash this value to.
 * newValue: The value to store.
 * oldValue: A pointer to a variable that will recv. the value previously stored
 * at this key.
 * returns: true if the given key previous had a value, and/or it was returned
 * in the oldValue variable, and false if it did not.
 */
#ifdef DATASTRUCT_ENABLE_INT
bool ht_put_int(HashTable * ht, char * key, int newValue, int * oldValue) {
  DSValue newDSValue;
  DSValue oldDSValue;
  bool oldValueExists = false;
  newDSValue.intVal = newValue;
  oldValueExists = ht_put(ht, key, &newDSValue, &oldDSValue);

  /* if pointer for old value was given, output to it */
  if(oldValue != NULL) {
    *oldValue = oldDSValue.intVal;
  }
  return oldValueExists;
}
#endif // DATASTRUCT_ENABLE_INT

/**
 * Puts a short in the hashtable.
 * key: key to hash this value to.
 * newValue: The value to store.
 * oldValue: A pointer to a variable that will recv. the value previously stored
 * at this key.
 * returns: true if the given key previous had a value, and/or it was returned
 * in the oldValue variable, and false if it did not.
 */
#ifdef DATASTRUCT_ENABLE_SHORT
bool ht_put_short(HashTable * ht, char * key, short newValue, short * oldValue) {
  DSValue newDSValue;
  DSValue oldDSValue;
  bool oldValueExists = false;
  newDSValue.shortVal = newValue;
  oldValueExists = ht_put(ht, key, &newDSValue, &oldDSValue);

  /* if pointer for old value was given, output to it */
  if(oldValue != NULL) {
    *oldValue = oldDSValue.shortVal;
  }
  return oldValueExists;
}
#endif // DATASTRUCT_ENABLE_SHORT

/**
 * Puts a char in the hashtable.
 * key: key to hash this value to.
 * newValue: The value to store.
 * oldValue: A pointer to a variable that will recv. the value previously stored
 * at this key.
 * returns: true if the given key previous had a value, and/or it was returned
 * in the oldValue variable, and false if it did not.
 */
#ifdef DATASTRUCT_ENABLE_CHAR
bool ht_put_char(HashTable * ht, char * key, char newValue, char * oldValue) {
  DSValue newDSValue;
  DSValue oldDSValue;
  bool oldValueExists = false;
  newDSValue.charVal = newValue;
  oldValueExists = ht_put(ht, key, &newDSValue, &oldDSValue);

  /* if pointer for old value was given, output to it */
  if(oldValue != NULL) {
    *oldValue = oldDSValue.charVal;
  }
  return oldValueExists;
}
#endif // DATASTRUCT_ENABLE_CHAR

/**
 * Puts a pointer in the hashtable.
 * key: key to hash this value to.
 * newValue: The value to store.
 * oldValue: A pointer to a variable that will recv. the value previously stored
 * at this key.
 * returns: true if the given key previous had a value, and/or it was returned
 * in the oldValue variable, and false if it did not.
 */
#ifdef DATASTRUCT_ENABLE_POINTER
bool ht_put_pointer(HashTable * ht, char * key, void * newValue, void ** oldValue) {
  DSValue newDSValue;
  DSValue oldDSValue;
  bool oldValueExists = false;
  newDSValue.pointerVal = newValue;
  oldValueExists = ht_put(ht, key, &newDSValue, &oldDSValue);

  /* if pointer for old value was given, output to it */
  if(oldValue != NULL) {
    *oldValue = oldDSValue.pointerVal;
  }
  return oldValueExists;
}
#endif // DATASTRUCT_ENABLE_POINTER






















/**
 * Gets a value hashed from hashtable using raw buffer as key.
 * ht: the hashtable instance
 * key: the key at which the value will be looked up.
 * keySize: the number of bytes from key to be used as the key.
 * value: pointer to a DSValue struct that will receive the stored value,
 * if it exists.
 * returns: true if the specified value exists and false if it does not.
 */
bool ht_get_raw_key(HashTable * ht, void * key, size_t keySize, DSValue * value) {

  /* calculate array index */
  int i = hash_to_index(ht, key, keySize);

  /* if there is a linked list at the hashed index, try to get the value */
  if(ht->table[i] != NULL) {
    return find_value(ht, i, key, keySize, NULL, value, false);
  }

  return false;
}

/**
 * Gets a value hashed with specified null terminated string key from the
 * hashtable.
 * ht: the hashtable instance
 * key: the key at which the value will be looked up. Null terminated String.
 * value: pointer to a DSValue struct that will receive the stored value,
 * if it exists.
 * returns: true if the specified value exists and false if it does not.
 */
bool ht_get(HashTable * ht, char * key, DSValue * value) {
  return ht_get_raw_key(ht, key, strlen(key) + 1, value);
}

/**
 * Moves iterator control to the next bucket containing items.
 * i: a HashTableIterator.
 * returns: false if no more buckets, true if buckets remain
 */
static bool iterator_next_bucket(HashTableIterator * i) {

  /* if no more buckets, return false */
  if(i->index == i->instance->tableSize) {
    return false;
  }

  /* open next bucket */
  i->index++;

  /* advance iterator until a list is found
   * TODO: perhaps eliminate this redundant size check
   */
  while(i->index < i->instance->tableSize
	&& i->instance->table[i->index] == NULL) {
    i->index++;
  }

  /* if there are no buckets left in the list, return error */
  if(i->index == i->instance->tableSize) {
    return false; // no more buckets
  }

  /* set current bucket to first bucket in list */
  i->prevNode = NULL;
  i->currentNode = i->instance->table[i->index];

  return true;
}

/**
 * Gets an iterator struct for this hashtable.
 * ht: hashtable index.
 * i: A hashtable iterator instance.
 */
void ht_iter_get(HashTable * ht, HashTableIterator * i) {

  /* store table in iterator */
  i->instance = ht;

  /* I really don't like this...but we're starting looking at bucket 0
   * but iterator_next_bucket increments index each time its called...
   */
  i->index = -1;

  /* "advance" iterator to first bucket: 0 */
  iterator_next_bucket(i);
}


/**
 * Checks the iterator for items that have not been iterated over yet.
 * i: an iterator object.
 * returns: true if items remain, and false if no items remain.
 */
bool ht_iter_has_next(HashTableIterator * i) {
  bool hasNext = false;

  /* if there are items remaining in current bucket */
  if(i->index < i->instance->tableSize
     && i->currentNode != NULL) {
    hasNext = true;
  } else {

    /* current bucket has no more items, try next non-empty bucket */
    if(iterator_next_bucket(i) && i->currentNode != NULL) {
      hasNext = true;
    }
  }

  return hasNext;
}

/**
 * Gets next item in the hashtable, via the iterator. This method is convenient
 * for listing the entire contents of the hashtable, removing items matching a
 * certain pattern, or removing items that contain pointers to dynamically
 * allocated memory that needs to be freed.
 *
 * I apologize for the complexity and number of arguments, but for the versatility
 * required of this hashmap for my projects, I need full control. Perhaps I'll
 * wrap this method in the future with simpler alternatives.
 *
 * keyBuffer: A buffer to receive the key of the next hashed key/value pair. If
 * NULL, this parameter will be ignored and the key will not be copied out.
 * keyBufferLen: The length of the key buffer. This parameter is ignored if the
 * keyBuffer parameter is NULL.
 * value: the value stored in this hashtable at the specified index.
 * keyLen: pointer to a size_t that will recv. the number of bytes written
 * to the keyBuffer. NOTE: since this hashtable is designed to handle any
 * byte buffer as a key, keys that are Strings that are written to keyBuffer
 * from this method will probably not come with NULL termination characters
 * (unless you fed in the key length in the put method as strlen(text) + 1.
 * remove: if true, removes the value from the hashtable.
 */
bool ht_iter_remove(HashTableIterator * i, void * keyBuffer,
			       size_t keyBufferLen, DSValue * value,
			       size_t * keyLen, bool remove) {

  /* checks to see if a value exists
   * this call also advances the iterator if neccessary.
   * make note of this. it isn't magic.
   */
  if(ht_iter_has_next(i)) {
    HashTableNode * currentNode = i->currentNode;

    /* copy as much of the key as will fit in the buffer */
    if(keyBuffer != NULL) {
      size_t writeSize = keyBufferLen < currentNode->keySize
	? keyBufferLen:currentNode->keySize;

      memcpy(keyBuffer, currentNode->key, writeSize);

      if(keyLen != NULL) {
	*keyLen = currentNode->keySize;
      }
    }

    /* copy out the value */
    if(value != NULL) {
      memcpy(value, &currentNode->value, sizeof(DSValue));
    }

    i->currentNode = currentNode->next;

    /* relink the list to skip current node and free current node */
    if(remove) {

      /* if this is the first node in the list, set head to null */
      if(i->prevNode == NULL) {
	i->instance->table[i->index] = NULL;
      } else {
	i->prevNode->next = currentNode->next;
      }

      node_free(currentNode);
    } else {
      i->prevNode = currentNode;
    }

    return true;
  }

  return false;
}

/* gets the number of items in the hashtable */
int ht_size(HashTable * ht) {
  return ht->numItems;
}

/* frees the specified HashTable Object */
void ht_free(HashTable * ht) {
  HashTableIterator i;

  ht_iter_get(ht, &i);

  /* free linked lists */
  while(ht_iter_remove(&i, NULL, 0, NULL, NULL, true) != false);

  /* free the array and struct */
  free(ht->table);
  free(ht);
}
