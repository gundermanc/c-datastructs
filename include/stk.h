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
#include <string.h>
#include "build_config.h"

#ifdef __cplusplus
namespace datastructs {
namespace internal {
#endif /* __cplusplus*/

typedef struct Stk {
  DSValue * stack;
  int blockSize;
  int depth;
  int size;
}Stk;

Stk * stk_new(int depth);
Stk * stk_new_resizable(int depth, int blockSize);

void stk_free(Stk * stack);

bool stk_push(Stk * stack, DSValue item);

#ifdef DATASTRUCT_ENABLE_BOOL
bool stk_push_bool(Stk * stack, bool value);
#endif /* DATASTRUCT_ENABLE_BOOL */

#ifdef DATASTRUCT_ENABLE_DOUBLE
bool stk_push_double(Stk * stack, double value);
#endif /* DATASTRUCT_ENABLE_DOUBLE */

#ifdef DATASTRUCT_ENABLE_LONG
bool stk_push_long(Stk * stack, long value);
#endif /* DATASTRUCT_ENABLE_LONG */

#ifdef DATASTRUCT_ENABLE_INT
bool stk_push_int(Stk * stack, int value);
#endif /* DATASTRUCT_ENABLE_INT */

#ifdef DATASTRUCT_ENABLE_SHORT
bool stk_push_short(Stk * stack, short value);
#endif /* DATASTRUCT_ENABLE_SHORT */

#ifdef DATASTRUCT_ENABLE_CHAR
bool stk_push_char(Stk * stack, char value);
#endif /* DATASTRUCT_ENABLE_char */

#ifdef DATASTRUCT_ENABLE_POINTER
bool stk_push_pointer(Stk * stack, void * value);
#endif /* DATASTRUCT_ENABLE_POINTER */

bool stk_peek(Stk * stack, DSValue * value);
bool stk_pop(Stk * stack, DSValue * value);
int stk_size(Stk * stack);

#ifdef __cplusplus
} /* namespace internal */
} /* namespace datastructs*/
#endif

#endif /* stk__h__ */
