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

#include <stdlib.h>
#include <string.h>
#include "ll.h"
#include "build_config.h"

typedef struct tagSB {
  int blockSize;
  int blockIndex;
  int size;
  LL * list;
}SB;

SB * sb_new(int blockSize);

void sb_free(SB * sb);

void sb_append_c(SB * sb, char c);

int sb_size(SB * sb);

SB * sb_reset(SB * sb);

int sb_to_string(SB * sb, char * dst, int dstLen);

void sb_append_s(SB * sb, char * string, int length);

#endif // SB__H__
