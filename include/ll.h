/**
 * Generic Unioned Linked List
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

#ifndef LL__H__
#define LL__H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "build_config.h"

#define LL_TAIL -1

typedef struct LLNode {
  void * nextNode;
  DSValue payload;
}LLNode;

typedef struct LL {
  LLNode * head;
  LLNode * tail;
  int size;
}LL;

typedef struct LLIterator {
  LL * list;
  LLNode * current;
  LLNode * previous;
}LLIterator;

LL * ll_new();
void ll_free(LL * list);
bool ll_append(LL * list, DSValue item);
int ll_size(LL * list);

#ifdef DATASTRUCT_ENABLE_BOOL
bool ll_append_bool(LL * list, bool value);
#endif // DATASTRUCT_ENABLE_BOOL

#ifdef DATASTRUCT_ENABLE_DOUBLE
bool ll_append_double(LL * list, double value);
#endif // DATASTRUCT_ENABLE_DOUBLE

#ifdef DATASTRUCT_ENABLE_LONG
bool ll_append_long(LL * list, long value);
#endif // DATASTRUCT_ENABLE_LONG

#ifdef DATASTRUCT_ENABLE_INT
bool ll_append_int(LL * list, int value);
#endif // DATASTRUCT_ENABLE_INT

#ifdef DATASTRUCT_ENABLE_SHORT
bool ll_append_short(LL * list, short value);
#endif // DATASTRUCT_ENABLE_SHORT

#ifdef DATASTRUCT_ENABLE_CHAR
bool ll_append_char(LL * list, char value);
#endif // DATASTRUCT_ENABLE_CHAR

#ifdef DATASTRUCT_ENABLE_POINTER
bool ll_append_pointer(LL * list, void * value);
#endif // DATASTRUCT_ENABLE_POINTER

LLNode * ll_get_node(LL * list, int index);

void ll_iter_get(LLIterator * iteratorObject, LL * list);

bool ll_iter_pop(LLIterator * i, DSValue * value);

bool ll_iter_peek(LLIterator * iteratorObject, DSValue * value);

bool ll_iter_has_next(LLIterator * iteratorObject);

DSValue ll_iter_remove(LLIterator * i);
#endif //LL__H__
