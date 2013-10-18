/**
 * Library Test File
 * (C) 2013 Christian Gunderman
 *
 * This file is available for your convenience while testing.
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

#include <stdlib.h>
#include <stdio.h>
#include "sb.h"
#include "ll.h"
#include "stack.h"
#include "hashtable.h"

int main() {
  HashTable * ht = hashtable_new(2, 10);
  HashTableIterator i;
  DSValue val;
  bool f;

  val.intVal = 1;
  hashtable_put(ht, "A", 5, &val, NULL);

  val.intVal = 2;
  hashtable_put(ht, "Ho", 5, &val, NULL);

  val.intVal = 3;
  hashtable_put(ht, "bad", 5, &val, NULL);

  val.intVal = 4;
  hashtable_put(ht, "foobar", 6, &val, NULL);

  val.intVal = 5;
  hashtable_put(ht, "random", 6, &val, NULL);

  val.intVal = 6;
  hashtable_put(ht, "sdfsf", 5, &val, NULL);

  hashtable_iterator_get(ht, &i);

  while(hashtable_iterator_has_next(&i)) {
    char key[99];
    DSValue value;
    hashtable_iterator_remove(&i, key, 99, &value, true);
    printf("%s : %i", key, value.intVal);
  }
  return 0;
}
