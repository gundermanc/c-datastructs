/**
 * Unioned Linked List
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

#include "ll.h"

/**
 * Creates a new linked list instance
 * returns: new linked list instance, unless malloc error occurs.
 * In malloc error, returns NULL.
 */
LL * ll_new() {
  LL * newList = (LL*)calloc(1, sizeof(LL));
  if(newList != NULL) {
    return newList;
  } else
    return NULL;
}

/**
 * Gets the size of the list.
 * list: an instance of linkedlist.
 * returns: the number of items in the list.
 */
int ll_size(LL * list) {
  return list->size;
}

/**
 * Frees a linked list.
 */
void ll_free(LL * list) {
  LLIterator i;

  /* free all nodes */
  ll_iter_get(&i, list);
  while(ll_iter_has_next(&i)) {
    ll_iter_remove(&i);
  }

  /* free list container */
  free(list);
}

/**
 * Appends an item to the end of the linked list.
 * This is a fast O(1) operation because the linked list
 * caches the tail for fast appending.
 * list: an instance of linkedlist.
 * item: an item to add to the list.
 */
bool ll_append(LL * list, DSValue item) {
  LLNode * newNode = (LLNode*)calloc(1, sizeof(LLNode));
  if(newNode != NULL) {
    newNode->nextNode = NULL;
    newNode->payload = item;

    if(list->tail == NULL) {

      /* list is empty, create new head */
      list->head = newNode;
      list->tail = newNode;
    } else {

      /* list is not empty, append to tail */
      list->tail->nextNode = newNode;
      list->tail = newNode;
    }
    list->size++;
  }
  return newNode != NULL;
}

#ifdef DATASTRUCT_ENABLE_BOOL
/**
 * Appends an item to the end of the linked list.
 * This is a fast O(1) operation because the linked list
 * caches the tail for fast appending.
 * list: an instance of linkedlist.
 * item: an item to add to the list.
 */
bool ll_append_bool(LL * list, bool value) {
  DSValue dsValue;
  dsValue.boolVal = value;
  return ll_append(list, dsValue);
}
#endif /* DATASTRUCT_ENABLE_BOOL */

#ifdef DATASTRUCT_ENABLE_DOUBLE
/**
 * Appends an item to the end of the linked list.
 * This is a fast O(1) operation because the linked list
 * caches the tail for fast appending.
 * list: an instance of linkedlist.
 * item: an item to add to the list.
 */
bool ll_append_double(LL * list, double value) {
  DSValue dsValue;
  dsValue.doubleVal = value;
  return ll_append(list, dsValue);
}
#endif /* DATASTRUCT_ENABLE_DOUBLE */

#ifdef DATASTRUCT_ENABLE_LONG
/**
 * Appends an item to the end of the linked list.
 * This is a fast O(1) operation because the linked list
 * caches the tail for fast appending.
 * list: an instance of linkedlist.
 * item: an item to add to the list.
 */
bool ll_append_long(LL * list, long value) {
  DSValue dsValue;
  dsValue.longVal = value;
  return ll_append(list, dsValue);
}
#endif /* DATASTRUCT_ENABLE_LONG */

#ifdef DATASTRUCT_ENABLE_INT
/**
 * Appends an item to the end of the linked list.
 * This is a fast O(1) operation because the linked list
 * caches the tail for fast appending.
 * list: an instance of linkedlist.
 * item: an item to add to the list.
 */
bool ll_append_int(LL * list, int value) {
  DSValue dsValue;
  dsValue.intVal = value;
  return ll_append(list, dsValue);
}
#endif /* DATASTRUCT_ENABLE_INT */

#ifdef DATASTRUCT_ENABLE_SHORT
/**
 * Appends an item to the end of the linked list.
 * This is a fast O(1) operation because the linked list
 * caches the tail for fast appending.
 * list: an instance of linkedlist.
 * item: an item to add to the list.
 */
bool ll_append_short(LL * list, short value) {
  DSValue dsValue;
  dsValue.shortVal = value;
  return ll_append(list, dsValue);
}
#endif /* DATASTRUCT_ENABLE_SHORT */

#ifdef DATASTRUCT_ENABLE_CHAR
/**
 * Appends an item to the end of the linked list.
 * This is a fast O(1) operation because the linked list
 * caches the tail for fast appending.
 * list: an instance of linkedlist.
 * item: an item to add to the list.
 */
bool ll_append_char(LL * list, char value) {
  DSValue dsValue;
  dsValue.charVal = value;
  return ll_append(list, dsValue);
}
#endif /* DATASTRUCT_ENABLE_CHAR */

#ifdef DATASTRUCT_ENABLE_POINTER
/**
 * Appends an item to the end of the linked list.
 * This is a fast O(1) operation because the linked list
 * caches the tail for fast appending.
 * list: an instance of linkedlist.
 * item: an item to add to the list.
 */
bool ll_append_pointer(LL * list, void * value) {
  DSValue dsValue;
  dsValue.pointerVal = value;
  return ll_append(list, dsValue);
}
#endif /* DATASTRUCT_ENABLE_POINTER */

/**
 * Returns the node at specified index.
 * list: an instance of linked list.
 * index: the index of the node to get.
 * returns: a pointer to the node, or NULL if node not exist.
 */
LLNode * ll_get_node(LL * list, int index) {
  int i = 0;
  LLNode * node;

  /* if tail is requested, return cached tail pointer */
  if(index == LL_TAIL)
    return list->tail;

  /* iterate through entire list until index matches */
  for(node = list->head;
      node != NULL; node = (LLNode*) node->nextNode, i++) {
    if(i == index)
      return node;
  }

  /* node not found */
  return NULL;
}

/**
 * Gets an iterator for iterating through the list
 * i: pointer to an LLIterator struct that will recv. the iterator data.
 * list: a linked list instance.
 */
void ll_iter_get(LLIterator * i, LL * list) {
  memset(i, 0, sizeof(LLIterator));

  i->list = list;
  i->current = list->head;
}

/**
 * Gets next item from list iterator and moves cursor forward.
 * i: the list iterator
 * value: a DSValue that will recv. the value. If this value is
 * NULL, it will not be written to.
 */
bool ll_iter_pop(LLIterator * i, DSValue * value) {
  if(i->current != NULL) {
    DSValue payload = i->current->payload;

    i->previous = i->current;
    i->current = (LLNode*)i->current->nextNode;

    if(value != NULL) {
      memcpy(value, &payload, sizeof(DSValue));
    }

    return true;
  } else {

    /* no more items */
    return false;
  }
}

/**
 * Gets the next item from the list iterator, but does not move the
 * cursor forward. Subsequent calls to ll_iter_peek will give the
 * same DSValue unless ll_iter_pop, or ll_iter_remove is called in
 * between ll_iter_peek calls.
 * i: linked list iterator.
 * value: a buffer that will recv. the DSValue.
 * returns: true if item was written to value, or false if no items
 * remain.
 */
bool ll_iter_peek(LLIterator * i, DSValue * value) {
  if(i->current != NULL) {
    memcpy(value, &i->current->payload, sizeof(DSValue));
    return true;
  }
  return false;
}

/**
 * Checks to see if list iterator has uniterated items remaining.
 * i: list iterator.
 * returns: true if items remain, and false if not.
 */
inline bool ll_iter_has_next(LLIterator * i) {
  return (i->current != NULL);
}

/**
 * Removes the next item in the iterator from the linked list.
 * Use in conjunction with ll_iter_peek or ll_iter_has_next to find
 * and remove values.
 * i: the linked list iterator.
 */
DSValue ll_iter_remove(LLIterator * i) {
  DSValue payload;

  if(ll_iter_has_next(i)) {
    LLNode * node = i->current;

    payload = i->current->payload;
    i->list->size--;

    /* Don't remember what this does....
     * TODO: figure out what this line is
     */
    if(i->list->size != 0) {
      i->list->tail = NULL;
    }

    if(i->current == i->list->head) {

      /* handle if node is head */
      i->list->head = (LLNode*)i->current->nextNode;
    } else {

      /* handle if node if not head */
      i->previous->nextNode = i->current->nextNode;
    }
    i->current = (LLNode*)i->current->nextNode;
    free(node);
  }

  return payload;
}
