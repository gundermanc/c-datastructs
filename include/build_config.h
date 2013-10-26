/**
 * Gunderman Data Structs
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
#ifndef BUILD__CONFIG__H__
#define BUILD__CONFIG__H__

#include <stdlib.h>


/* Build Configuration parameters:
 * The following definitions can be used to enable or disable features of
 * the datastructures. The advantage of this is that it allows you to minimize
 * memory waste in each linkedlist, stack, or hashtable node due to the UNION. By
 * commenting out the enables for larger types, such as DOUBLE, LONG, and POINTER,
 * you shrink the UNION's minimum size from 64 bits, down to 32 and so on.
 */
#define DATASTRUCT_ENABLE_BOOL
#define DATASTRUCT_ENABLE_DOUBLE
#define DATASTRUCT_ENABLE_LONG
#define DATASTRUCT_ENABLE_INT
#define DATASTRUCT_ENABLE_SHORT
#define DATASTRUCT_ENABLE_CHAR
#define DATASTRUCT_ENABLE_POINTER

/* Boolean Definitions:
 * Some compilers don't come with stdbool.h, so we go ahead and define our own
 * for this project.
 *
 * If you wish to use another bool definition such as stdbool.h, define:
 * DATASTRUCT_SUPRESS_BOOL_DEFINITION before including any headers from this
 * project.
 */
#ifndef DATASTRUCT_SUPRESS_BOOL_DEFINITION
#ifndef bool
#define bool int
#endif /* bool */

#ifndef true
#define true 1
#endif /* true */

#ifndef false
#define false 0
#endif /* false */
#endif /* DATASTRUCT_SUPRESS_BOOL_DEFINITION */

/* standard data union for all data structures */
typedef union {

#ifdef DATASTRUCT_ENABLE_BOOL
  bool boolVal;
#endif /* DATASTRUCT_ENABLE_BOOL */

#ifdef DATASTRUCT_ENABLE_DOUBLE
  double doubleVal;
#endif /* DATASTRUCT_ENABLE_DOUBLE */

#ifdef DATASTRUCT_ENABLE_LONG
  long longVal;
#endif /* DATASTRUCT_ENABLE_LONG */

#ifdef DATASTRUCT_ENABLE_INT
  int intVal;
#endif /* DATASTRUCT_ENABLE_INT */

#ifdef DATASTRUCT_ENABLE_SHORT
  short shortVal;
#endif /* DATASTRUCT_ENABLE_SHORT */

#ifdef DATASTRUCT_ENABLE_CHAR
  char charVal;
#endif /* DATASTRUCT_ENABLE_CHAR */

#ifdef DATASTRUCT_ENABLE_POINTER
  void * pointerVal;
#endif /* DATASTRUCT_ENABLE_POINTER */

}DSValue;

#endif /* BUILD__CONFIG__H__ */
