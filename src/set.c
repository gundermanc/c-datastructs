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

HashSet * set_new() {
  HashSet * s = calloc(sizeof(HashSet), 1);
  s->ht = ht_new(10, 10, 0.8f);

  return s;
}

/* returns true if the value was already in the set */
bool set_add(HashSet * s, void * value, size_t valueLen) {
  DSValue newDSValue;

  /* makes no difference what the value is as long as the item is in the
   * hashtable might as well give it a value though
   */
  newDSValue.boolVal = true;

  return ht_put_raw_key(s->ht, value, valueLen, &newDSValue, NULL);
}

// returns true if the item being removed was in the list
bool set_remove(HashSet * s, void * value, size_t valueLen) {

  /* newValue is NULL, telling hashtable to delete */
  return ht_put_raw_key(s->ht, value, valueLen, NULL, NULL);
}

bool set_contains(HashSet * s, void * value, size_t valueLen) {
  DSValue oldDSValue;

  /* we don't really care what the value is, but the hashtable deletes
   * the item if we don't pass in an output buffer for value.
   */
  return ht_get_raw_key(s->ht, value, valueLen, &oldDSValue);
}

int set_size(HashSet * s) {
  return ht_size(s->ht);
}

/**
 * Function basically just wraps the standard hashtable iterator
 * and is only here because it makes it easier to differentiate
 * set related code from HashTable code.
 */
void set_iter_get(HashSet * s, HashSetIterator * i) {
  ht_iter_get(s->ht, i);
}

bool set_iter_has_next(HashSetIterator * i) {
  return ht_iter_has_next(i);
}

bool set_iter_next(HashSetIterator * i, void * valueBuffer, size_t valueBufferLen,
		   size_t * valueLen, bool remove) {
  return ht_iter_next(i, valueBuffer, valueBufferLen, NULL, valueLen, remove);
}

void set_free(HashSet * s) {
  ht_free(s->ht);
  free(s);
}
