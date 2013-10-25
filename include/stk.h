/**
 * Unioned Array Stack
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

#ifndef stk__h__
#define stk__h__

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "build_config.h"

typedef struct tagStack {
  DSValue * stack;
  int depth;
  int size;
}Stack;

Stack * stk_new(int depth);

void stk_free(Stack * stack);

bool stk_push(Stack * stack, DSValue item);

#ifdef DATASTRUCT_ENABLE_BOOL
bool stk_push_bool(Stack * stack, bool value);
#endif /* DATASTRUCT_ENABLE_BOOL */

#ifdef DATASTRUCT_ENABLE_DOUBLE
bool stk_push_double(Stack * stack, double value);
#endif /* DATASTRUCT_ENABLE_DOUBLE */

#ifdef DATASTRUCT_ENABLE_LONG
bool stk_push_long(Stack * stack, long value);
#endif /* DATASTRUCT_ENABLE_LONG */

#ifdef DATASTRUCT_ENABLE_INT
bool stk_push_int(Stack * stack, int value);
#endif /* DATASTRUCT_ENABLE_INT */

#ifdef DATASTRUCT_ENABLE_SHORT
bool stk_push_short(Stack * stack, short value);
#endif /* DATASTRUCT_ENABLE_SHORT */

#ifdef DATASTRUCT_ENABLE_CHAR
bool stk_push_char(Stack * stack, char value);
#endif /* DATASTRUCT_ENABLE_char */

#ifdef DATASTRUCT_ENABLE_POINTER
bool stk_push_pointer(Stack * stack, void * value);
#endif /* DATASTRUCT_ENABLE_POINTER */

bool stk_peek(Stack * stack, DSValue * value);
bool stk_pop(Stack * stack, DSValue * value);
int stk_size(Stack * stack);

#endif /* stk__h__ */
