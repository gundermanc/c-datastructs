/**
 * Unioned HashSet
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

#include "set.h"
#include <stdio.h>

/**
 * Creates a new instance of set.
 * returns: a new set, or NULL if unable to allocate memory.
 */
Set * set_new() {
  Set * s = calloc(sizeof(Set), 1);

  if(s != NULL) {
    s->ht = ht_new(10, 10, 0.8f);

    if(s->ht == NULL) {
      free(s);
      s = NULL;
    }
  }

  return s;
}

/**
 * Adds the specified value to the set.
 * s: an instance of set.
 * value: pointer to a buffer containing a value.
 * valueLen: the number of bytes to copy from the buffer for value.
 * return: returns true if the value previously existed in the set.
 */
bool set_add(Set * s, void * value, size_t valueLen) {
  DSValue newDSValue;

  /* makes no difference what the value is as long as the item is in the
   * hashtable might as well give it a value though
   */
  newDSValue.boolVal = true;

  return ht_put_raw_key(s->ht, value, valueLen, &newDSValue, NULL);
}

/**
 * Removes an item from the set.
 * s: an instance of set.
 * value: the value to remove from the set.
 * valueLen: the number of bytes to copy from the value buffer to treat as the
 * value.
 * returns: true if the value previously existed in the set.
 */
bool set_remove(Set * s, void * value, size_t valueLen) {

  /* newValue is NULL, telling hashtable to delete */
  return ht_put_raw_key(s->ht, value, valueLen, NULL, NULL);
}

/**
 * Checks to see if a value exists in the set.
 * s: an instance of set.
 * value: the value to check for.
 * valueLen: the number of bytes from value buffer to treat as the value.
 * returns: true if the set contains the value in question, and false if
 * it does not.
 */
bool set_contains(Set * s, void * value, size_t valueLen) {
  DSValue oldDSValue;

  /* we don't really care what the value is, but the hashtable deletes
   * the item if we don't pass in an output buffer for value.
   */
  return ht_get_raw_key(s->ht, value, valueLen, &oldDSValue);
}

/**
 * Gets the number of unique items in the set.
 * s: an instance of set
 * returns: the number of items.
 */
int set_size(Set * s) {
  return ht_size(s->ht);
}

/**
 * Gets the iterator for the set.
 *
 * Note:
 * Function basically just wraps the standard hashtable iterator
 * and is only here because it makes it easier to differentiate
 * set related code from HashTable code.
 *
 * s: an instance of set.
 * i: A buffer that will receive the set iterator.
 */
void set_iter_get(Set * s, SetIter * i) {
  ht_iter_get(s->ht, i);
}

/**
 * Determines whether or not the set iterator has iterated through
 * all items yet.
 * i: an instance of set iterator.
 * returns: true if items remain, and false if no items remain.
 */
bool set_iter_has_next(SetIter * i) {
  return ht_iter_has_next(i);
}

/**
 * Gets the next item in the set.
 * i: an instance of set iterator.
 * valueBuffer: the buffer that will recv. the next value.
 * valueBufferLen: the length of the buffer that will recv. the next value.
 * valueLen: Pointer to a size_t value that will recv. the number of bytes
 * written to valueBuffer.
 * bool remove: if true, the value will be removed after it is copied out.
 * returns: true if an uniterated item was found and written to the valueBuffer.
 */
bool set_iter_next(SetIter * i, void * valueBuffer, size_t valueBufferLen,
		   size_t * valueLen, bool remove) {
  return ht_iter_next(i, valueBuffer, valueBufferLen, NULL, valueLen, remove);
}

/**
 * Frees a set.
 * s: an instance of set.
 */
void set_free(Set * s) {
  ht_free(s->ht);
  free(s);
}
