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
  HashTable * ht = hashtable_new(10);
  DSValue newValue;
  DSValue storVal;

  newValue.intVal = 5;
  hashtable_put(ht, "Hellp", 5, &newValue, NULL);
  newValue.intVal = 23;
  hashtable_put(ht, "Hello", 5, &newValue, NULL);
  hashtable_get(ht, "Hellp", 5, &storVal);
  printf("%i", storVal.intVal);
  return 0;
}
