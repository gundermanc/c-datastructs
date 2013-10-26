/**
 * Dynamically Expanding String Buffer
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
#ifndef SB__H__
#define SB__H__

#include "build_config.h"


/* Don't define these functions unless pointers are enabled, since they are required */
#ifdef DATASTRUCT_ENABLE_POINTER


#include <stdlib.h>
#include <string.h>
#include "ll.h"

typedef struct tagSB {
  int blockSize;
  int blockIndex;
  int size;
  LL * list;
}SB;

SB * sb_new(int blockSize);

void sb_free(SB * sb);

void sb_append_char(SB * sb, char c);

int sb_size(SB * sb);

SB * sb_reset(SB * sb);

void sb_append_str(SB * sb, char * string, size_t length);

int sb_to_buffer(SB * sb, char * dst, size_t dstLen, bool nullChar);

#endif /* DATASTRUCT_ENABLE_POINTER */
#endif /* SB__H__ */
