/**
 * Unioned Array Stack or Array List
 * (C) 2013-2015 Christian Gunderman
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

#ifndef stk__h__
#define stk__h__

#include <stdlib.h>
#include <string.h>
#include "build_config.h"

typedef struct Stk {
  DSValue * stack;
  int depth;
  int size;
}Stk;

Stk * stk_new(int depth);

void stk_free(Stk * stack);

bool stk_set(Stk * stack, DSValue value, int index);

bool stk_get(Stk * stack, DSValue * value, int index);

bool stk_push(Stk * stack, DSValue item);

#ifdef DATASTRUCT_ENABLE_BOOL
bool stk_push_bool(Stk * stack, bool value);
bool stk_set_bool(Stk * stack, bool value, int index);
#endif /* DATASTRUCT_ENABLE_BOOL */

#ifdef DATASTRUCT_ENABLE_DOUBLE
bool stk_push_double(Stk * stack, double value);
bool stk_set_double(Stk * stack, double value, int index);
#endif /* DATASTRUCT_ENABLE_DOUBLE */

#ifdef DATASTRUCT_ENABLE_LONG
bool stk_push_long(Stk * stack, long value);
bool stk_set_long(Stk * stack, long value, int index);
#endif /* DATASTRUCT_ENABLE_LONG */

#ifdef DATASTRUCT_ENABLE_INT
bool stk_push_int(Stk * stack, int value);
bool stk_set_int(Stk * stack, int value, int index);
#endif /* DATASTRUCT_ENABLE_INT */

#ifdef DATASTRUCT_ENABLE_SHORT
bool stk_push_short(Stk * stack, short value);
bool stk_set_short(Stk * stack, short value, int index);
#endif /* DATASTRUCT_ENABLE_SHORT */

#ifdef DATASTRUCT_ENABLE_CHAR
bool stk_push_char(Stk * stack, char value);
bool stk_set_char(Stk * stack, char value, int index);
#endif /* DATASTRUCT_ENABLE_char */

#ifdef DATASTRUCT_ENABLE_POINTER
bool stk_push_pointer(Stk * stack, void * value);
bool stk_set_pointer(Stk * stack, void * value, int index);
#endif /* DATASTRUCT_ENABLE_POINTER */

bool stk_peek(Stk * stack, DSValue * value);

bool stk_peek_offset(Stk * stack, int offset, DSValue * value);

bool stk_pop(Stk * stack, DSValue * value);

int stk_size(Stk * stack);

int stk_depth(Stk * stack);

#endif /* stk__h__ */
