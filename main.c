#include <stdlib.h>
#include <stdio.h>
#include "sb.h"

int main() {
  SB * s = sb_new(1);
  char foo[200];
  sb_append_s(s, "Hello poop face!", 16);
  sb_append_c(s, 'W');
  
  sb_to_string(s, foo, 200);
  printf(foo);
  sb_free(s);
}
