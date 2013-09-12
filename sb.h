/**
 * Dynamically Expanding String Buffer
 * (C) 2013 Christian Gunderman
 * Part of Lexie-C Project
 */
#ifndef SB__H__
#define SB__H__
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
void sb_append_c(SB * sb, char c);
int sb_size(SB * sb);
SB * sb_reset(SB * sb);
int sb_to_string(SB * sb, char * dst, int dstLen);
void sb_append_s(SB * sb, char * string, int length);
#endif // SB__H__
