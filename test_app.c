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
#include "stk.h"
#include "ht.h"

/* place any scratch code you want to test the library here: */
int main() {
  HashTable * ht = ht_new(10, 10, 0.75f);
  HashTableIterator i;
  DSValue val;

  ht_put_int(ht, "A", 1, NULL);
  ht_put_int(ht, "Food!", 3, NULL);
  ht_put_int(ht, "sdfsf", 6, NULL);

  ht_iter_get(ht, &i);

  while(ht_iter_has_next(&i)) {
    char key[99] = "" ;
    DSValue value;
    ht_iter_remove(&i, key, 99, &value, NULL, true);
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
