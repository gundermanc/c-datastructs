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
#include "ht.h"

int main() {
  HashTable * ht = ht_new(10, 10, 0.75f);
  HashTableIterator i;
  DSValue val;

  val.intVal = 1;
  ht_put(ht, "A", 5, &val, NULL);

  val.intVal = 2;
  ht_put(ht, "Ho", 5, &val, NULL);

  val.intVal = 3;
  ht_put(ht, "bad", 5, &val, NULL);

  val.intVal = 4;
  ht_put(ht, "foobar", 6, &val, NULL);

  val.intVal = 5;
  ht_put(ht, "random", 6, &val, NULL);

  val.intVal = 6;
  ht_put(ht, "sdfsf", 5, &val, NULL);

  ht_iter_get(ht, &i);

  while(ht_iter_has_next(&i)) {
    char key[99] = "" ;
    DSValue value;
    ht_iter_remove(&i, key, 99, &value, NULL, false);
    printf("%s : %i", key, value.intVal);
  }

  printf("\n\n\n");
  ht_iter_get(ht, &i);
  while(ht_iter_has_next(&i)) {
    char key[99];
    DSValue value;
    ht_iter_remove(&i, key, 99, &value, NULL, false);
    printf("%s : %i", key, value.intVal);
  }

  ht_free(ht);
  return 0;
}
