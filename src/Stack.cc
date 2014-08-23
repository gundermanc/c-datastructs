/**
 * C++ Stack Wrapper Class
 * (C) 2014 Christian Gunderman
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

#include <stdexcept>
#include "Stack.hh"
#include "stk.h"

namespace datastructs {

/**
 * Constructs new stack object.
 * depth: number of item slots to create stack with.
 * blockSize: the number of items to increase stack in size each time
 * it becomes full. if zero, stack does not grow.
 * throws: invalid_argument if depth or blockSize is invalid range.
 */
Stack::Stack(int depth, int blockSize) {

  if(depth < 1) {
    throw new std::invalid_argument("depth must be > 0.");
  }

  if(blockSize < 0) {
    throw new std::invalid_argument("blockSize must be >= 0.");
  }

  this->blockSize = blockSize;
  this->depth = depth;
  this->size = 0;
  this->stack = NULL;
}

/**
 * Destroys stack.
 */
Stack::~Stack() {
  if(this->IsInitialized()) {
    free(this->stack);
  }
}

/**
 * If the stack is not yet initialized, allocates memory for the item
 * stack.
 * returns: false if malloc fails.
 */
bool Stack::Initialize() {
  if(!this->IsInitialized()) {
    this->stack = (DSValue*)calloc (this->depth, sizeof(DSValue));

    if(!this->stack) {
      return false;
    }
  }

  return true;
}

/**
 * Peeks at the top item on the stack. If stackEmpty is provided,
 * it will be set to true if there are no items in the stack, and
 * false if there are.
 * returns: top item in the stack, if there is one.
 */
DSValue Stack::Peek(bool* stackEmpty) {
  DSValue value;
  value.intVal = 0;

  if(stk_peek(this, &value)) {
    if(stackEmpty != NULL) {
      *stackEmpty = false;
    }
  } else {
    if(stackEmpty != NULL) {
      *stackEmpty = true;
    }
  }

  return value; 
}

/**
 * Pops the top item off of the stack, if there is one.
 * stackEmpty: true if stack is empty or not allocated, false if not.
 */
DSValue Stack::Pop(bool* stackEmpty) {
  DSValue value;
  value.intVal = 0;

  if(stk_pop(this, &value)) {
    if(stackEmpty != NULL) {
      *stackEmpty = false;
    }
  } else {
    if(stackEmpty != NULL) {
      *stackEmpty = true;
    }
  }

  return value;
}
} /* namespace datastructs*/
