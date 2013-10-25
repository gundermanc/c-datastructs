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

#ifndef SET__H__
#define SET__H__

#include <stdlib.h>
#include <string.h>
#include "build_config.h"
#include "ht.h"

/* Preprocessor Definitions */
#define HashSetIterator HashTableIterator

/* HashSet Struct */
typedef struct tagHashSet {

  /* This is the only thing in the struct, but using a struct gives the ability
   * to add additional functionality in the future.
   */
  HashTable * ht;
}HashSet;

#endif /* SET__H__ */
