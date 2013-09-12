/**
 * Build Configuration File
 * (C) 2013 Christian Gunderman
 */
#ifndef build_config__h__
#define build_config__h__

#include <stdlib.h>
#include <stdbool.h>


// build configuration parameters
#define DATASTRUCT_ENABLE_BOOL
#define DATASTRUCT_ENABLE_DOUBLE
#define DATASTRUCT_ENABLE_LONG
#define DATASTRUCT_ENABLE_INT
#define DATASTRUCT_ENABLE_SHORT
#define DATASTRUCT_ENABLE_CHAR
#define DATASTRUCT_ENABLE_POINTER


// standard data union for all data structures
typedef union {

#ifdef DATASTRUCT_ENABLE_BOOL
  bool boolVal;
#endif // DATASTRUCT_ENABLE_BOOL

#ifdef DATASTRUCT_ENABLE_DOUBLE
  double doubleVal;
#endif // DATASTRUCT_ENABLE_DOUBLE

#ifdef DATASTRUCT_ENABLE_LONG
  long longVal;
#endif // DATASTRUCT_ENABLE_LONG

#ifdef DATASTRUCT_ENABLE_INT
  int intVal;
#endif // DATASTRUCT_ENABLE_INT

#ifdef DATASTRUCT_ENABLE_SHORT
  short shortVal;
#endif // DATASTRUCT_ENABLE_SHORT

#ifdef DATASTRUCT_ENABLE_CHAR
  char charVal;
#endif // DATASTRUCT_ENABLE_CHAR

#ifdef DATASTRUCT_ENABLE_POINTER
  void * pointerVal;
#endif // DATASTRUCT_ENABLE_POINTER

}DSValue;

#endif // build_config__h__
