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
#include "stk.h"

#ifdef __cplusplus
namespace datastructs {
namespace internal {
#endif /* __cplusplus*/

/**
 * Allocates a new stack object.
 * depth: how many indicies deep do you want the stack to be.
 * returns: A new stack object, or NULL if unable to allocate.
 */
Stk * stk_new(int depth) {

  /* if given depth is at least 2 */
  if(depth > 1) {

    /* allocate stack object */
    Stk * newList = (Stk*)calloc(1, sizeof(Stk));
    if(newList != NULL) {
      newList->size = 0;
      newList->depth = depth;

      /* allocate mem for items */
      newList->stack = (DSValue*)calloc (newList->depth, sizeof(DSValue));
      return newList;
    }
  }
  return NULL;
}

/**
 * Allocates a new stack object that automatically grows as you add items.
 * depth: how many indicies deep do you want the stack to be.
 * blockSize: number of items to add each time the stack expands.
 * returns: A new stack object, or NULL if unable to allocate.
 */
Stk * stk_new_resizable(int depth, int blockSize) {

  /* if given depth is at least 2 */
  if(depth > 1) {

    /* allocate stack object */
    Stk * newList = (Stk*)calloc(1, sizeof(Stk));
    if(newList != NULL) {
      newList->size = 0;
      newList->depth = depth;
      newList->blockSize = blockSize;

      /* allocate mem for items */
      newList->stack = (DSValue*)calloc (newList->depth, sizeof(DSValue));
      return newList;
    }
  }
  return NULL;
}

/**
 * Frees a stack and all items currently in it, unless the items are pointers.
 *
 * Note: if stack contains pointers, you will have to pop those items and free
 * them yourself to ensure that the dynamically allocated memory is properly freed.
 *
 * stack: an instance of stack.
 */
void stk_free(Stk * stack) {
  free(stack->stack);
  free(stack);
}

/**
 * Resizes the internal stack to hold newSize number of DSValue items.
 */
static bool resize(Stk * stack, size_t newSize) {
  DSValue * newStack = (DSValue*)calloc(newSize, sizeof(DSValue));

  /* return false on alloc failed */
  if(newStack == NULL) {
    return false;
  }

  /* copy items to new stack and free old stack*/
  memcpy(newStack, stack->stack, sizeof(DSValue) * stack->size);
  free(stack->stack);
  stack->stack = newStack;
  stack->size = newSize;

  return true;
}

/**
 * Attempts to a add an item to the stack.
 * stack: an instance of stack.
 * item: A DSValue to place on the stack.
 * returns true if the item was pushed succcessfully, or false if the stack
 * is full and not resizeable.
 */
bool stk_push(Stk * stack, DSValue item) {

  /* check if stack is too small and resize if allowed */
  if(stack->size >= (stack->depth)) {
    
    /* fail: stack is too small and not resizable */
    if(stack->blockSize <= 0) {
      return false;
    }
    
    resize(stack, stack->size + stack->blockSize);
  }

  /* store item */
  stack->stack[stack->size] = item;
  stack->size++;
  return true;
}

#ifdef DATASTRUCT_ENABLE_BOOL
/**
 * Attempts to a add an item to the stack.
 * stack: an instance of stack.
 * item: A bool to place on the stack.
 * returns true if the item was pushed succcessfully, or false if the stack
 * is full.
 */
bool stk_push_bool(Stk * stack, bool value) {
  DSValue item;
  item.boolVal = value;
  return stk_push(stack, item);
}
#endif /* DATASTRUCT_ENABLE_BOOL */

#ifdef DATASTRUCT_ENABLE_DOUBLE
/**
 * Attempts to a add an item to the stack.
 * stack: an instance of stack.
 * item: A double to place on the stack.
 * returns true if the item was pushed succcessfully, or false if the stack
 * is full.
 */
bool stk_push_double(Stk * stack, double value) {
  DSValue item;
  item.doubleVal = value;
  return stk_push(stack, item);
}
#endif /* DATASTRUCT_ENABLE_DOUBLE */

#ifdef DATASTRUCT_ENABLE_LONG
/**
 * Attempts to a add an item to the stack.
 * stack: an instance of stack.
 * item: A long to place on the stack.
 * returns true if the item was pushed succcessfully, or false if the stack
 * is full.
 */
bool stk_push_long(Stk * stack, long value) {
  DSValue item;
  item.longVal = value;
  return stk_push(stack, item);
}
#endif /* DATASTRUCT_ENABLE_LONG */

#ifdef DATASTRUCT_ENABLE_INT
/**
 * Attempts to a add an item to the stack.
 * stack: an instance of stack.
 * item: An int to place on the stack.
 * returns true if the item was pushed succcessfully, or false if the stack
 * is full.
 */
bool stk_push_int(Stk * stack, int value) {
  DSValue item;
  item.intVal = value;
  return stk_push(stack, item);
}
#endif /* DATASTRUCT_ENABLE_INT */

#ifdef DATASTRUCT_ENABLE_SHORT
/**
 * Attempts to a add an item to the stack.
 * stack: an instance of stack.
 * item: A short to place on the stack.
 * returns true if the item was pushed succcessfully, or false if the stack
 * is full.
 */
bool stk_push_short(Stk * stack, short value) {
  DSValue item;
  item.shortVal = value;
  return stk_push(stack, item);
}
#endif /* DATASTRUCT_ENABLE_SHORT */

#ifdef DATASTRUCT_ENABLE_CHAR
/**
 * Attempts to a add an item to the stack.
 * stack: an instance of stack.
 * item: A char to place on the stack.
 * returns true if the item was pushed succcessfully, or false if the stack
 * is full.
 */
bool stk_push_char(Stk * stack, char value) {
  DSValue item;
  item.charVal = value;
  return stk_push(stack, item);
}
#endif /* DATASTRUCT_ENABLE_char */

#ifdef DATASTRUCT_ENABLE_POINTER
/**
 * Attempts to a add an item to the stack.
 * stack: an instance of stack.
 * item: A pointer to place on the stack.
 * returns true if the item was pushed succcessfully, or false if the stack
 * is full.
 */
bool stk_push_pointer(Stk * stack, void * value) {
  DSValue item;
  item.pointerVal = value;
  return stk_push(stack, item);
}
#endif /* DATASTRUCT_ENABLE_POINTER */

/**
 * Gets the top value from the stack, without removing it.
 * stack: an instance of stack.
 * value: a buffer to recv. the value.
 * returns: true if an item was copied successfully, or false if no
 * item could be copied.
 */
bool stk_peek(Stk * stack, DSValue * value) {
  if(stack->size > 0) {
    memcpy(value, &stack->stack[stack->size-1], sizeof(DSValue));
    return true;
  }
  return false;
}

/**
 * Gets the top value from the stack, and removes it.
 * stack: an instance of stack.
 * value: a buffer to recv. the value.
 * returns: true if an item was copied successfully, or false if no
 * item could be copied.
 */
bool stk_pop(Stk * stack, DSValue * value) {
  if(stk_peek(stack, value)) {
    stack->size--;
    return true;
  }
  return false;
}

/**
 * Gets the number of items in the stack.
 * stack: an instance of stack.
 * returns: the number of items.
 */
int stk_size(Stk * stack) {
  return stack->size;
}

#ifdef __cplusplus
} /* namespace internal */
} /* namespace datastructs*/
#endif
