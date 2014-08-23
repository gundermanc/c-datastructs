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

#ifndef STACK__HH__
#define STACK__HH__

#include "stk.h"
#include "build_config.h"

namespace datastructs {

class Stack : internal::Stk {
 public:
  Stack(int depth, int blockSize = 0);
  ~Stack();
  bool Initialize();
  
  template <typename TValue> 
  void Push(TValue value) {
    Stack::Initialize();
    stk_push(this, DSValue{value}); 
  }
  
  DSValue Peek(bool* stackEmpty = NULL);
  DSValue Pop(bool* stackEmpty = NULL);
  int Size() { return this->size; }

 private:
  inline bool IsInitialized() { return this->stack != NULL; }
};

} /* namespace datastructs*/

#endif /* STACK__HH__ */
