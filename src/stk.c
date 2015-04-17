/**
 * Unioned Array Stack OR Array List
 * (C) 2013-2015 Christian Gunderman
 *
 * Doubles as both Stack AND Array List. NOTE: if you use stk_set(),
 * stack functionality may be broken. You should use as stack OR list,
 * not both.
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
      newList->depth = depth + 1;

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
 * Sets an index in the stack (treated as an array).
 *
 * NOTE: This function is an _array_ function and will cause undefined
 * behavior if you try and mix it with push and pop. Use as array OR stack,
 * not both.
 *
 * stack: an instance of stack.
 * value: the item to set in the specified index.
 * index: the index in the _array_ to set.
 */
bool stk_set(Stk * stack, DSValue value, int index) {
  if (index < stack->depth && index >= 0) {
    stack->stack[index] = value;
    return true;
  }

  return false;
}

/**
 * Gets an index in the stack (treated as an array).
 *
 * NOTE: This function is an _array_ function and will cause
 * undefined behavior if you try and mix it with push and pop. Use as
 * array OR stack, not both.
 * stack: an instance of stack.
 * value: the buffer that receives the value at the index.
 */
bool stk_get(Stk * stack, DSValue * value, int index) {
  if (index < stack->depth && index >= 0) {
    *value = stack->stack[index];
    return true;
  }

  return false;
}

/**
 * Attempts to a add an item to the stack.
 * stack: an instance of stack.
 * item: A DSValue to place on the stack.
 * returns true if the item was pushed succcessfully, or false if the stack
 * is full.
 */
bool stk_push(Stk * stack, DSValue item) {
  if(stack->size < (stack->depth-1)) {
    stack->stack[stack->size] = item;
    stack->size++;
    return true;
  } else {

    /* stack is full */
    return false;
  }
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

/**
 * Sets an index in the stack (treated as an array).
 *
 * NOTE: This function is an _array_ function and will cause undefined
 * behavior if you try and mix it with push and pop. Use as array OR stack,
 * not both.
 *
 * stack: an instance of stack.
 * value: the item to set in the specified index.
 * index: the index in the _array_ to set.
 */
bool stk_set_bool(Stk * stack, bool value, int index) {
  DSValue item;
  item.boolVal = value;
  return stk_set(stack, item, index);
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

/**
 * Sets an index in the stack (treated as an array).
 *
 * NOTE: This function is an _array_ function and will cause undefined
 * behavior if you try and mix it with push and pop. Use as array OR stack,
 * not both.
 *
 * stack: an instance of stack.
 * value: the item to set in the specified index.
 * index: the index in the _array_ to set.
 */
bool stk_set_double(Stk * stack, double value, int index) {
  DSValue item;
  item.doubleVal = value;
  return stk_set(stack, item, index);
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

/**
 * Sets an index in the stack (treated as an array).
 *
 * NOTE: This function is an _array_ function and will cause undefined
 * behavior if you try and mix it with push and pop. Use as array OR stack,
 * not both.
 *
 * stack: an instance of stack.
 * value: the item to set in the specified index.
 * index: the index in the _array_ to set.
 */
bool stk_set_long(Stk * stack, long value, int index) {
  DSValue item;
  item.longVal = value;
  return stk_set(stack, item, index);
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

/**
 * Sets an index in the stack (treated as an array).
 *
 * NOTE: This function is an _array_ function and will cause undefined
 * behavior if you try and mix it with push and pop. Use as array OR stack,
 * not both.
 *
 * stack: an instance of stack.
 * value: the item to set in the specified index.
 * index: the index in the _array_ to set.
 */
bool stk_set_int(Stk * stack, int value, int index) {
  DSValue item;
  item.intVal = value;
  return stk_set(stack, item, index);
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

/**
 * Sets an index in the stack (treated as an array).
 *
 * NOTE: This function is an _array_ function and will cause undefined
 * behavior if you try and mix it with push and pop. Use as array OR stack,
 * not both.
 *
 * stack: an instance of stack.
 * value: the item to set in the specified index.
 * index: the index in the _array_ to set.
 */
bool stk_set_short(Stk * stack, short value, int index) {
  DSValue item;
  item.shortVal = value;
  return stk_set(stack, item, index);
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

/**
 * Sets an index in the stack (treated as an array).
 *
 * NOTE: This function is an _array_ function and will cause undefined
 * behavior if you try and mix it with push and pop. Use as array OR stack,
 * not both.
 *
 * stack: an instance of stack.
 * value: the item to set in the specified index.
 * index: the index in the _array_ to set.
 */
bool stk_set_char(Stk * stack, char value, int index) {
  DSValue item;
  item.charVal = value;
  return stk_set(stack, item, index);
}
#endif /* DATASTRUCT_ENABLE_CHAR */

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

/**
 * Sets an index in the stack (treated as an array).
 *
 * NOTE: This function is an _array_ function and will cause undefined
 * behavior if you try and mix it with push and pop. Use as array OR stack,
 * not both.
 *
 * stack: an instance of stack.
 * value: the item to set in the specified index.
 * index: the index in the _array_ to set.
 */
bool stk_set_pointer(Stk * stack, void * value, int index) {
  DSValue item;
  item.pointerVal = value;
  return stk_set(stack, item, index);
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
 * Gets the  value from the stack at the specified offset from the top
 * without removing it.
 * stack: an instance of stack.
 * value: a buffer to recv. the value.
 * returns: true if an item was copied successfully, or false if no
 * item could be copied.
 */
bool stk_peek_offset(Stk * stack, int offset, DSValue * value) {
  if(stack->size > 0 && offset >= 0 && offset < stack->size) {
    memcpy(value, &stack->stack[stack->size-1-offset], sizeof(DSValue));
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
