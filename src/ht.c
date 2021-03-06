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
 * ht: the context of the HT
 * key: the key to hash
 * keySize: the length of the key to hash, in bytes.
 */
static long hash_to_index(HT * ht, void * key, size_t keySize) {

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
static void node_free(HTNode * node) {
  free(node->key);
  free(node);
}

/**
 * Gets the last node in a linked list of nodes.
 * node: the starting node in the list
 * returns: The the last node in the list
 */
static HTNode * node_list_last(HTNode * node) {
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
static void append_node(HTNode * list, HTNode * node) {
  HTNode * tail = node_list_last(list);
  tail->next = node;
}

/**
 * Hashes the key in the node structure and then places the node in the
 * linked list at the hashed index.
 * ht: the instance of the HT to add the node to.
 * node: the node to add to the hashtable.
 */
static void put_node(HT * ht, HTNode * node) {

  int i = hash_to_index(ht, node->key, node->keySize);

  /* if list exists at index, append to the end */
  if(ht->table[i] != NULL) {
    append_node(ht->table[i], node);
  } else {

    /* create new list/head */
    ht->table[i] = node;
  }

  node->next = NULL;
  ht->numItems++;
}

/**
 * Rehashes the specified HT to the new array size.
 * ht: the instance of hashtable to modify.
 * newSize: the new length for the hashtable array.
 */
static bool rehash_table(HT * ht, size_t newSize) {
  int i = 0;
  HTNode ** oldTable = ht->table;
  size_t oldSize = ht->tableSize;

  /* reset table */
  ht->tableSize = newSize;
  ht->table = calloc(sizeof(HTNode*), newSize);
  ht->numItems = 0;

  /* memory alloc error */
  if(ht->table == NULL) {
    return false;
  }

  /* iterate the list heads */
  for(i = 0; i < oldSize; i++) {
    HTNode * node = oldTable[i];

    /* put each node in the list in its new hash index */
    while(node != NULL) {
      HTNode * currentNode = node;

      node = node->next;
      put_node(ht, currentNode);
    }
  }

  free(oldTable);
  return true;
}

/**
 * Checks the loading of the hashtable. If hashtable is loaded beyond load
 * factor, rehash_table is called, expanding table by ht->blockSize.
 * ht: the hashtable instance.
 */
static void check_load_factor(HT * ht) {
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
 * returns: pointer to a new HT struct.
 */
HT * ht_new(int tableSize, int blockSize, float loadFactor) {
  HT * ht = calloc(1, sizeof(HT));

  /* check for successful memory allocation of container*/
  if(ht == NULL) {
    return NULL;
  }

  /* alloc array of linked list pointers */
  ht->table = calloc(1, sizeof(HTNode*) * tableSize);
  if(ht->table == NULL) {
    free(ht);
    return NULL;
  }

  ht->tableSize = tableSize;
  ht->blockSize = blockSize;
  ht->loadFactor = loadFactor;

  return ht;
}

/**
 * Helper method that exchanges old values in the hashtable with new
 * ones.
 * ht: hashtable instance.
 * newValue: the new value to set.
 * prevNode: the node before the current node.
 * deleteValOnNull: If true, deletes the value if newValue is null.
 * i: current linked list index
 */
static void exchange_values(HT * ht, DSValue * newValue,
			    HTNode * curNode, HTNode * prevNode,
			    bool deleteValOnNull, int i) {
  /* if newValue is provided, store it in the pre-exising node */
  if(newValue != NULL) {
    memcpy(&ht->table[i]->value, newValue, sizeof(DSValue));
  } else if(deleteValOnNull) {

    /* newValue is NULL, delete the value */
    if(prevNode != NULL) {
      prevNode->next = curNode->next;
      node_free(curNode);
    } else {

      /* at head, delete list */
      node_free(ht->table[i]);
      ht->table[i] = NULL;
    }
    ht->numItems--;
  }
}

/**
 * Copies the key from the current node to the provided key buffer
 * currentNode: the node to copy the key from.
 * keyBuffer: The buffer to copy the key to.
 * keyBufferLen: the length of the key buffer, in bytes.
 */
static void copy_node_key(HTNode * currentNode, void * keyBuffer,
			  size_t keyBufferLen, size_t * keyLen) {

  if(keyBuffer != NULL) {
    size_t writeSize = keyBufferLen < currentNode->keySize
      ? keyBufferLen:currentNode->keySize;

    /* copy only as much of the key as will fit in the buffer */
    memcpy(keyBuffer, currentNode->key, writeSize);

    if(keyLen != NULL) {
      *keyLen = currentNode->keySize;
    }
  }

}

/**
 * Copies the value from the provided node to a DSValue buffer.
 * currentNode: the node whos value will be copied.
 * value: the DSValue buffer that will recv. the value.
 */
static void copy_node_value(HTNode * currentNode, DSValue * value) {
  if(value != NULL) {
    memcpy(value, &currentNode->value, sizeof(DSValue));
  }
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
static bool find_value(HT * ht, int i, void * key, size_t keySize,
		       DSValue * newValue, DSValue * oldValue,
		       bool deleteValOnNull) {
  HTNode * curNode = ht->table[i];
  HTNode * prevNode = NULL;

  /* while tokens remain, keep going */
  while(curNode != NULL) {

    /* if the keys are the same... */
    if(curNode->keySize == keySize && memcmp(key, curNode->key, keySize) == 0) {

      /* if oldValue buffer is provided, copy previous value to it */
      copy_node_value(curNode, oldValue);

      /* exchange old value with new one */
      exchange_values(ht, newValue, curNode, prevNode, deleteValOnNull, i);

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
static HTNode * node_new(void * key, size_t keySize, DSValue * value,
				HTNode * next) {
  HTNode * node = calloc(1, sizeof(HTNode));

  if(node != NULL) {
    node->key = calloc(keySize, 1);
    memcpy(node->key, key, keySize);
    node->keySize = keySize;
    memcpy(&node->value, value, sizeof(DSValue));
    node->next = next;
  }

  return node;
}

/**
 * Checks to make sure that a pointer to a boolean isn't null and then
 * copies the new boolean to it.
 * outValue: pointer to the recv. buffer.
 * inValue: the value to set the outValue to.
 */
static void copy_boolean(bool * outValue, bool inValue) {
  if(outValue != NULL) {
    *outValue = inValue;
  }
}

/**
 * Stores a value in the hashtable using a buffer of raw bytes as the key.
 * ht: the hashtable instance.
 * key: the key which the value will be hashted to
 * keySize: The number of bytes from key that will be used for the key.
 * newValue: A pointer to a new value to store. If this value is NULL, the
 * item at the specified key is removed from the list.
 * oldValue: A buffer that will recv. the old value hashed to this key. Pass
 * null if you don't care about the old value.
 * prevValue: a boolean that will receive whether or not there was previously
 * a value at the specified key. If this param is NULL, it is ignored.
 * return: true if the operation is a success, or false if there is a memory
 * allocation error.
 */
bool ht_put_raw_key(HT * ht, void * key, size_t keySize,
		    DSValue * newValue, DSValue * oldValue, bool *  prevValue) {
  int i = hash_to_index(ht, key, keySize);
  bool oldValueExists = false;

  if(ht->table[i] != NULL) {
    oldValueExists = find_value(ht, i, key, keySize, newValue, oldValue, true);
  }

  copy_boolean(prevValue, oldValueExists);

  if(!oldValueExists) {
    if(newValue != NULL) {

      HTNode * newNode = node_new(key, keySize, newValue, NULL);
      if(newNode == NULL) {
	return false;
      }

      put_node(ht, newNode);
    }
  }

  /* expand table if neccessary */
  check_load_factor(ht);

  return true;
}

/**
 * Stores a value in the hashtable using a null terminated string as the key.
 * ht: the hashtable instance.
 * key: the key which the value will be hashted to
 * newValue: A pointer to a new value to store. If value is NULL, value at this
 * key will be removed.
 * oldValue: A buffer that will recv. the old value hashed to this key. Pass
 * null if you don't care about the old value.
 * prevValue: A boolean that will recv. whether or not a value was previously
 * stored at the given key.
 * return: true if new value was placed successfully, or false if a memory
 * allocation error occurred.
 */
bool ht_put(HT * ht, char * key, DSValue * newValue,
	    DSValue * oldValue, bool * prevValue) {
  return ht_put_raw_key(ht, key, strlen(key) + 1, newValue, oldValue, prevValue);
}

/**
 * Puts a value in the hashtable.
 * key: key to hash this value to.
 * newValue: The value to store. If value is NULL, value at this
 * key will be removed.
 * oldValue: A pointer to a variable that will recv. the value previously stored
 * at this key.
 * prevValue: A boolean that will recv. whether or not a value was previously
 * stored at the given key.
 * return: true if new value was placed successfully, or false if a memory
 * allocation error occurred.
 */
#ifdef DATASTRUCT_ENABLE_BOOL
bool ht_put_bool(HT * ht, char * key, bool newValue, bool * oldValue, bool * prevValue) {
  DSValue newDSValue;
  DSValue oldDSValue;
  bool success = false;
  newDSValue.boolVal = newValue;
  success = ht_put(ht, key, &newDSValue, &oldDSValue, prevValue);

  /* if pointer for old value was given, output to it */
  if(oldValue != NULL) {
    *oldValue = oldDSValue.boolVal;
  }
  return success;
}
#endif /* DATASTRUCT_ENABLE_BOOL */

/**
 * Puts a value in the hashtable.
 * key: key to hash this value to.
 * newValue: The value to store. If value is NULL, value at this
 * key will be removed.
 * oldValue: A pointer to a variable that will recv. the value previously stored
 * at this key.
 * prevValue: A boolean that will recv. whether or not a value was previously
 * stored at the given key.
 * return: true if new value was placed successfully, or false if a memory
 * allocation error occurred.
 */
#ifdef DATASTRUCT_ENABLE_DOUBLE
bool ht_put_double(HT * ht, char * key, double newValue, double * oldValue, bool * prevValue) {
  DSValue newDSValue;
  DSValue oldDSValue;
  bool success = false;
  newDSValue.doubleVal = newValue;
  success = ht_put(ht, key, &newDSValue, &oldDSValue, prevValue);

  /* if pointer for old value was given, output to it */
  if(oldValue != NULL) {
    *oldValue = oldDSValue.doubleVal;
  }
  return success;
}
#endif /* DATASTRUCT_ENABLE_DOUBLE */

/**
 * Puts a value in the hashtable.
 * key: key to hash this value to.
 * newValue: The value to store. If value is NULL, value at this
 * key will be removed.
 * oldValue: A pointer to a variable that will recv. the value previously stored
 * at this key.
 * prevValue: A boolean that will recv. whether or not a value was previously
 * stored at the given key.
 * return: true if new value was placed successfully, or false if a memory
 * allocation error occurred.
 */
#ifdef DATASTRUCT_ENABLE_LONG
bool ht_put_long(HT * ht, char * key, long newValue, long * oldValue, bool * prevValue) {
  DSValue newDSValue;
  DSValue oldDSValue;
  bool success = false;
  newDSValue.longVal = newValue;
  success = ht_put(ht, key, &newDSValue, &oldDSValue, prevValue);

  /* if pointer for old value was given, output to it */
  if(oldValue != NULL) {
    *oldValue = oldDSValue.longVal;
  }
  return success;
}
#endif /* DATASTRUCT_ENABLE_LONG */

/**
 * Puts a value in the hashtable.
 * key: key to hash this value to.
 * newValue: The value to store. If value is NULL, value at this
 * key will be removed.
 * oldValue: A pointer to a variable that will recv. the value previously stored
 * at this key.
 * prevValue: A boolean that will recv. whether or not a value was previously
 * stored at the given key.
 * return: true if new value was placed successfully, or false if a memory
 * allocation error occurred.
 */
#ifdef DATASTRUCT_ENABLE_INT
bool ht_put_int(HT * ht, char * key, int newValue, int * oldValue, bool * prevValue) {
  DSValue newDSValue;
  DSValue oldDSValue;
  bool success = false;
  newDSValue.intVal = newValue;
  success = ht_put(ht, key, &newDSValue, &oldDSValue, prevValue);

  /* if pointer for old value was given, output to it */
  if(oldValue != NULL) {
    *oldValue = oldDSValue.intVal;
  }
  return success;
}
#endif /* DATASTRUCT_ENABLE_INT */

/**
 * Puts a value in the hashtable.
 * key: key to hash this value to.
 * newValue: The value to store. If value is NULL, value at this
 * key will be removed.
 * oldValue: A pointer to a variable that will recv. the value previously stored
 * at this key.
 * prevValue: A boolean that will recv. whether or not a value was previously
 * stored at the given key.
 * return: true if new value was placed successfully, or false if a memory
 * allocation error occurred.
 */
#ifdef DATASTRUCT_ENABLE_SHORT
bool ht_put_short(HT * ht, char * key, short newValue, short * oldValue, bool * prevValue) {
  DSValue newDSValue;
  DSValue oldDSValue;
  bool success = false;
  newDSValue.shortVal = newValue;
  success = ht_put(ht, key, &newDSValue, &oldDSValue, prevValue);

  /* if pointer for old value was given, output to it */
  if(oldValue != NULL) {
    *oldValue = oldDSValue.shortVal;
  }
  return success;
}
#endif /* DATASTRUCT_ENABLE_SHORT */

/**
 * Puts a value in the hashtable.
 * key: key to hash this value to.
 * newValue: The value to store. If value is NULL, value at this
 * key will be removed.
 * oldValue: A pointer to a variable that will recv. the value previously stored
 * at this key.
 * prevValue: A boolean that will recv. whether or not a value was previously
 * stored at the given key.
 * return: true if new value was placed successfully, or false if a memory
 * allocation error occurred.
 */
#ifdef DATASTRUCT_ENABLE_CHAR
bool ht_put_char(HT * ht, char * key, char newValue, char * oldValue, bool * prevValue) {
  DSValue newDSValue;
  DSValue oldDSValue;
  bool success = false;
  newDSValue.charVal = newValue;
  success = ht_put(ht, key, &newDSValue, &oldDSValue, prevValue);

  /* if pointer for old value was given, output to it */
  if(oldValue != NULL) {
    *oldValue = oldDSValue.boolVal;
  }
  return success;
}
#endif /* DATASTRUCT_ENABLE_CHAR */

/**
 * Puts a value in the hashtable.
 * key: key to hash this value to.
 * newValue: The value to store. If value is NULL, value at this
 * key will be removed.
 * oldValue: A pointer to a variable that will recv. the value previously stored
 * at this key.
 * prevValue: A boolean that will recv. whether or not a value was previously
 * stored at the given key.
 * return: true if new value was placed successfully, or false if a memory
 * allocation error occurred.
 */
#ifdef DATASTRUCT_ENABLE_POINTER
bool ht_put_pointer(HT * ht, char * key, void * newValue, void ** oldValue, bool * prevValue) {
  DSValue newDSValue;
  DSValue oldDSValue;
  bool success = false;
  newDSValue.pointerVal = newValue;
  success = ht_put(ht, key, &newDSValue, &oldDSValue, prevValue);

  /* if pointer for old value was given, output to it */
  if(oldValue != NULL) {
    *oldValue = oldDSValue.pointerVal;
  }
  return success;
}
#endif /* DATASTRUCT_ENABLE_POINTER */

/**
 * Gets a value hashed from hashtable using raw buffer as key.
 * ht: the hashtable instance
 * key: the key at which the value will be looked up.
 * keySize: the number of bytes from key to be used as the key.
 * value: pointer to a DSValue struct that will receive the stored value,
 * if it exists.
 * returns: true if the specified value exists and false if it does not.
 */
bool ht_get_raw_key(HT * ht, void * key, size_t keySize, DSValue * value) {

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
bool ht_get(HT * ht, char * key, DSValue * value) {
  return ht_get_raw_key(ht, key, strlen(key) + 1, value);
}

/**
 * Moves iterator control to the next bucket containing items.
 * i: a HTIter
 * returns: false if no more buckets, true if buckets remain
 */
static bool iter_next_bucket(HTIter * i) {

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
    return false;
  }

  /* set current bucket to first bucket in list */
  i->prevNode = NULL;
  i->currentNode = i->instance->table[i->index];

  return true;
}

/**
 * Gets an iterator struct for this hashtable.
 * ht: hashtable index.
 * i: A pointer to a buffer the size of HTIter that will recv. the
 * hashtable iterator instance.
 */
void ht_iter_get(HT * ht, HTIter * i) {

  memset(i, 0, sizeof(HTIter));

  /* store table in iterator */
  i->instance = ht;

  /* I really don't like this...but we're starting looking at bucket 0
   * but iter_next_bucket increments index each time its called...
   * make note of this quirk before you modify the code.
   */
  i->index = -1;

  /* "advance" iterator to first bucket: 0 */
  iter_next_bucket(i);
}


/**
 * Checks the iterator for items that have not been iterated over yet by
 * checking the current bucket for the next item. If no more items are left in
 * current bucket, this function iterates through the remaining buckets until it
 * finds the next non-empty bucket, or runs out of buckets to search.
 *
 * Note: this is a O(k) operation, where k is the size of the hashtable
 * (number of slots in the array). This is no problem if the data are evenly
 * distributed across the table, and the table is approximately the size of the
 * number of items. However, in the worst case, all of the items are in one
 * linked list, then this function has to iterate across all of the empty array
 * slots to determine that there are no more items left.
 *
 * i: an iterator object.
 * returns: true if items remain, and false if no items remain.
 */
bool ht_iter_has_next(HTIter * i) {
  bool hasNext = false;

  /* if there are items remaining in current bucket */
  if(i->index < i->instance->tableSize
     && i->currentNode != NULL) {
    hasNext = true;
  } else {

    /* current bucket has no more items, move to the next non-empty bucket and
     * try looking for items again.
     */
    if(iter_next_bucket(i) && i->currentNode != NULL) {
      hasNext = true;
    }
  }

  return hasNext;
}

/**
 * Evaluates the remove argument.
 * remove: If true, remove sthe currentNode from the the hashtable and frees it.
 */
static void iter_remove_node(HTIter * i, HTNode * currentNode) {

  if(currentNode == i->instance->table[i->index]) {

    /* if this is the first node in the list,
     * set head to current head's next node
     */
    i->instance->table[i->index] = currentNode->next;
    node_free(currentNode);
  } else {

    /* if not the first node, set the previous node's next to point to current
     * node's next
     */
    i->prevNode->next = currentNode->next;
    node_free(currentNode);
  }
  i->instance->numItems--;
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
 * returns: true if an uniterated item was found and written to the buffers.
 */
bool ht_iter_next(HTIter * i, void * keyBuffer,
			       size_t keyBufferLen, DSValue * value,
			       size_t * keyLen, bool remove) {

  /* checks to see if a value exists
   * this call also advances the iterator to the next non-empty bucket
   * if neccessary. make note of this. it isn't magic.
   */
  if(ht_iter_has_next(i)) {
    HTNode * currentNode = i->currentNode;

    /* copy the key and value to the external buffers */
    copy_node_key(currentNode, keyBuffer, keyBufferLen, keyLen);
    copy_node_value(currentNode, value);

    /* we've output the value, now advance to next node */
    i->currentNode = currentNode->next;

    /* check if we need to remove node */
    if(remove) {
      iter_remove_node(i, currentNode);
    }

    i->prevNode = currentNode;

    return true;
  }

  return false;
}

/**
 * Gets the number of items in the hashtable.
 * ht: An initialized hashtable instance.
 * returns: the number of items currently hashed in the table.
 */
int ht_size(HT * ht) {
  return ht->numItems;
}

/**
 * Frees the nodes and items in this hashtable, and then frees the struct's
 * memory.
 *
 * Note: Although calling the free function does free the linked list nodes,
 * it does not free the items that they point to. If this hashtable contains
 * pointers to dynamically allocated memory, it must be freed manually.
 * ht: the hashtable instance to free.
 */
void ht_free(HT * ht) {
  HTIter i;

  ht_iter_get(ht, &i);

  /* free linked lists */
  while(ht_iter_next(&i, NULL, 0, NULL, NULL, true) != false);

  /* free the array and struct */
  free(ht->table);
  free(ht);
}
