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
#include "stack.h"

// allocates a new stack object
// returns a pointer to the stack instance of the stack was allocated 
// successfully, or 0 if unable to allocate it.
Stack * stack_new(int depth) {

  // if given depth is at least 2
  if(depth > 1) {

    // allocate stack object
    Stack * newList = (Stack*)malloc(sizeof(Stack));
    if(newList != 0) {
      newList->size = 0;
      newList->depth = depth + 1;

      // allocate mem for items
      newList->stack = (DSValue*)malloc (sizeof(DSValue) * newList->depth);
      return newList;
    }
  }
  return 0;
}

// frees the struct of an EMPTY stack
void stack_free(Stack * stack) {
  free(stack->stack);
  free(stack); 
}

// adds item to a stack. returns true if success
// false if stack overflow
bool stack_push(Stack * stack, DSValue item) {
  if(stack->size < (stack->depth-1)) {
    stack->stack[stack->size] = item;
    stack->size++;
    return true;
  } else
    return false; // stack overflowed
}

#ifdef DATASTRUCT_ENABLE_BOOL
// pushes a boolean to the stack. returns true if success
// false if stack overflow
bool stack_push_bool(Stack * stack, bool value) {
  DSValue item;
  item.boolVal = value;
  return stack_push(stack, item);
}
#endif // DATASTRUCT_ENABLE_BOOL

#ifdef DATASTRUCT_ENABLE_DOUBLE
// pushes a double to the stack. returns true if success
// false if stack overflow
bool stack_push_double(Stack * stack, double value) {
  DSValue item;
  item.doubleVal = value;
  return stack_push(stack, item);
}
#endif // DATASTRUCT_ENABLE_DOUBLE

#ifdef DATASTRUCT_ENABLE_LONG
// pushes a long to the stack. returns true if success
// false if stack overflow
bool stack_push_long(Stack * stack, long value) {
  DSValue item;
  item.longVal = value;
  return stack_push(stack, item);
}
#endif // DATASTRUCT_ENABLE_LONG

#ifdef DATASTRUCT_ENABLE_INT
// pushes an int to the stack. returns true if success
// false if stack overflow
bool stack_push_int(Stack * stack, int value) {
  DSValue item;
  item.intVal = value;
  return stack_push(stack, item);
}
#endif // DATASTRUCT_ENABLE_INT

#ifdef DATASTRUCT_ENABLE_SHORT
// pushes a short to the stack. returns true if success
// false if stack overflow
bool stack_push_short(Stack * stack, short value) {
  DSValue item;
  item.shortVal = value;
  return stack_push(stack, item);
}
#endif // DATASTRUCT_ENABLE_SHORT

#ifdef DATASTRUCT_ENABLE_CHAR
// pushes a char to the stack. returns true if success
// false if stack overflow
bool stack_push_char(Stack * stack, char value) {
  DSValue item;
  item.charVal = value;
  return stack_push(stack, item);
}
#endif // DATASTRUCT_ENABLE_char

#ifdef DATASTRUCT_ENABLE_POINTER
// pushes a pointer to the stack. returns true if success
// false if stack overflow
bool stack_push_pointer(Stack * stack, void * value) {
  DSValue item;
  item.pointerVal = value;
  return stack_push(stack, item);
}
#endif // DATASTRUCT_ENABLE_POINTER

// returns true if successful, and false if no stack values
bool stack_peek(Stack * stack, DSValue * value) {
  if(stack->size > 0) {
    memcpy(value, &stack->stack[stack->size-1], sizeof(DSValue));
    return true;
  } else 
    return false;
}

// returns true if successful, and false if no stack values
bool stack_pop(Stack * stack, DSValue * value) {
  if(stack_peek(stack, value)) {
    stack->size--;
    return true;
  }
}

// gets the number of items currently in the stack
int stack_size(Stack * stack) {
  return stack->size;
}
