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

#ifndef HT__H__
#define HT__H__

#include <stdlib.h>
#include <string.h>
#include "build_config.h"

/* HashTable List Node */
typedef struct HTNode {
  void * key;
  size_t keySize;
  void * next;     /* HTNode* */
  DSValue value;
}HTNode;

/* HashTable Structure definition */
typedef struct HT {
  HTNode ** table;
  int tableSize;
  int blockSize;
  float loadFactor;
  int numItems;
} HT;

/* HT Iterator structure defintion */
typedef struct HTIter {
  HT * instance;
  int index;
  HTNode * prevNode;
  HTNode * currentNode;
} HTIter;

HT * ht_new(int tableSize, int blockSize, float loadFactor);

#ifdef DATASTRUCT_ENABLE_BOOL
bool ht_put_bool(HT * ht, char * key, bool newValue, bool * oldValue);
#endif /* DATASTRUCT_ENABLE_BOOL */

#ifdef DATASTRUCT_ENABLE_DOUBLE
bool ht_put_double(HT * ht, char * key, double newValue, double * oldValue);
#endif /* DATASTRUCT_ENABLE_DOUBLE */

#ifdef DATASTRUCT_ENABLE_LONG
bool ht_put_long(HT * ht, char * key, long newValue, long * oldValue);
#endif /* DATASTRUCT_ENABLE_LONG */

#ifdef DATASTRUCT_ENABLE_INT
bool ht_put_int(HT * ht, char * key, int newValue, int * oldValue);
#endif /* DATASTRUCT_ENABLE_INT */

#ifdef DATASTRUCT_ENABLE_SHORT
bool ht_put_short(HT * ht, char * key, short newValue, short * oldValue);
#endif /* DATASTRUCT_ENABLE_SHORT */

#ifdef DATASTRUCT_ENABLE_CHAR
bool ht_put_char(HT * ht, char * key, char newValue, char * oldValue);
#endif /* DATASTRUCT_ENABLE_CHAR */

#ifdef DATASTRUCT_ENABLE_POINTER
bool ht_put_pointer(HT * ht, char * key, void * newValue, void ** oldValue);
#endif /* DATASTRUCT_ENABLE_POINTER */


bool ht_get(HT * ht, char * key, DSValue * value);

bool ht_put(HT * ht, char * key, DSValue * newValue, DSValue * oldValue) ;

bool ht_put_raw_key(HT * ht, void * key, size_t keySize,
		   DSValue * newValue, DSValue * oldValue);

bool ht_get_raw_key(HT * ht, void * key, size_t keySize, DSValue * value);

void ht_iter_get(HT * ht, HTIter * i);

bool ht_iter_has_next(HTIter * i);

bool ht_iter_next(HTIter * i, void * keyBuffer,
			       size_t keyBufferLen, DSValue * value,
			       size_t * keyLen, bool remove);

int ht_size(HT * ht);

void ht_free(HT * ht);

#endif
