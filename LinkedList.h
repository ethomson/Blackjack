// Blackjack Simulation : Utility Library : LinkedList
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.  See the included file 'COPYING'
// for more information.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// Library to support a doubly-linked list.  Elements can be added
// on either the front or the back of the list, and removed from either
// the front or the back of the list.  This support was added so that
// we can create other container classes like queues and stacks which
// have well-defined in/out characteristics that will simply overload
// the linked-list class and implement their own add/remove functions.
//
// You may call iterate() to walk through the list - the first call
// will return the data from the front of the list and set a "cursor".
// Subsequent calls will increment the "cursor" and return the next
// piece of data in the list.  When the end of the list is reached,
// iterate() will return NULL.  Therefore, if one wishes to store
// integer values which may be zero, it is advisable to use pointers.
// You may remove the data which the cursor is currently pointing to
// by using the remove() function.  If you wish to reset the cursor,
// call the cursor() function.
//
// NOTE: the LinkedList class does not do any memory management of your
// objects.  When you remove an object, we do not call free() or delete().
// Beware of memory leaks.

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdlib.h>
#include <sys/types.h>
#include <stdint.h>
#include "Util.h"
#include "Assert.h"
#include "LinkedListNode.h"

//
// class declaration
//

template <class T>
class LinkedList
{
	public:
		LinkedList();
		T iterate(void);				// walk through the list
		bool add(T data);				// add at current position
		bool addFront(T data);			// add at front of list
		bool addTail(T data);			// add at tail of list
		T remove(void);					// remove current item (or head)
		T removeFront(void);			// remove head item
		T removeTail(void);				// remove tail item
		T fetchFront(void);				// return  the tail item
		T fetchTail(void);				// return  the tail item
		T fetchNumber(uint16_t idx);	// return an indexed item
		uint32_t getCount(void);		// number of entries
		void debug(void);				// dump contents of linked list
		void clear(void);				// clear the linked list
		~LinkedList();

	private:
		LinkedListNode<T> *_first;		// first entry in list
		LinkedListNode<T> *_last;		// last entry in list
		LinkedListNode<T> *_itr_prev;	// previous in iteration loop
		uint32_t _count;				// # items in list
};


//
// function definition
//

// constructor : sets up LinkedList for usage
template <class T>
LinkedList<T>::LinkedList()
{
	// the linked list starts out empty - clear the head and tail node
	_first = NULL;
	_last = NULL;

	// clear the cursor so that the first call to iterate() starts at
	// the beginning
	_itr_prev = NULL;

	// we keep a count of the elements in the list -- clear it
	_count = 0;
}


// iterate : returns the next element's data, and sets up for subsequent
//           calls to iterate by setting a cursor.
//           returns the data on success or NULL at the end of the list.
template <class T>
T LinkedList<T>::iterate(void)
{
	// we've reached the end of the list - we have a cursor setup and
	// the next entry is NULL, thus we simply return NULL.  we also
	// reset the cursor so that subsequent calls will restart the
	// iteration at the beginning of the list.
	if(_itr_prev != NULL && _itr_prev->next == NULL)
	{
		_itr_prev = NULL;
		return NULL;
	}

	// there's no list to iterate over -- ie, _first is NULL when the
	// linked list is empty.  thus, we simply return NULL meaning that
	// iteration is finished at the first call.
	else if(_itr_prev == NULL && _first == NULL)
	{
		_itr_prev = NULL;
		return NULL;
	}

	// this is the first time iterate has been called - there's no
	// cursor setup, so we setup the cursor (_itr_prev and _itr_next)
	// and return the data for the first object.  subsequent calls will
	// return _first->next->data...
	else if(_itr_prev == NULL)
	{
		_itr_prev = _first;
		return _first->data;
	}

	// otherwise, we have a cursor and we're not at the end of the list.
	// so we increment the cursor and return the data.
	else
	{
		_itr_prev = _itr_prev->next;
		return _itr_prev->data;
	}

	// unreached - sanity check
	ASSERT(0);
}


// add : add data to the tail of a list with addTail() to avoid code
//       replication.
//       returns true on success.
template <class T>
bool LinkedList<T>::add(T data)
{
	return addTail(data);
}


// addFront : add data to the front of the list (like a stack)
//            returns true on success.
template <class T>
bool LinkedList<T>::addFront(T data)
{
	LinkedListNode<T> *entry;

	entry = new LinkedListNode<T>();

	// since this is going at the head of the list, this node will have
	// no previous entry, and this node's next entry will be what was
	// (previously) the first node in the list.
	entry->next = _first;
	entry->previous = NULL;
	entry->data = data;

	// since the current data is going at the head of the list, we must
	// adjust the (previous) first node in the list to have the previous
	// pointer going to this node.  (recall that this is a doubly-linked
	// list.)
	if(_first != NULL)
		_first->previous = entry;

	// set this node to be the head of the list
	_first = entry;

	// increment the list node count
	_count++;

	return true;
}


// addTail : add data to the tail of the list (like a list)
//           returns true on success.
template <class T>
bool LinkedList<T>::addTail(T data)
{
	LinkedListNode<T> *entry;

	entry = new LinkedListNode<T>;

	// since this is going at the tail of the list, this node will have
	// no next entry, and this node's previous entry will be what was
	// (previously) the tail node in the list.
	entry->next = NULL;
	entry->previous = _last;
	entry->data = data;

	// since the current data is going at the tail of the list, we must
	// adjust the (previous) tail node in the list to have the next
	// pointer going to this node.  (recall that this is a doubly-linked
	// list.)  However, if there is no head node in the list, we need to
	// instead make this the head node in the list (ie, it is the only
	// node.)
	if(_first == NULL)
		_first = entry;
	else
		_last->next = entry;

	// set this node to be the tail of the list
	_last = entry;

	// increment the list entry count
	_count++;

	return true;
}


// remove : remove and return element from current iteration point
//          iterate() must have been called before a call to remove().
//          returns the removed object on success, returns NULL on
//          failure.
template <class T>
T LinkedList<T>::remove(void)
{
	LinkedListNode<T> *entry;
	T data;

	// iterate() has not yet been called
	if(_itr_prev == NULL)
		return NULL;

	// get the previously returned entry to iterate() and get the data
	// so we can return it after the entry is removed.
	entry = _itr_prev;
	data = entry->data;

	// if this is the first element in the list, set the first element
	// to the entry's next element.  if this is not the first element
	// in the list, we need to repair the hole that will be in the
	// list -- ie if the list looks like this:  1 2 3  and we're
	// removing 2, we need to set 1->next to be 3.
	if(entry->previous == NULL)
		_first = entry->next;
	else
		entry->previous->next = entry->next;

	// if this is the last element in the list, set the last element
	// to the entry's previous element.  if this is not the last element
	// in the list, we need to repair the hole in the list -- if the
	// list is  1 2 3  and we're removing 2, we need to set 3->previous
	// to be 1.
	if(entry->next == NULL)
		_last = entry->previous;
	else
		entry->next->previous = entry->previous;

	// free the memory taken up by the node.  note that we don't free
	// the data that was passed to us by the caller.
	delete entry;

	// decrement the count
	_count--;

	// return the data that was in the list
	return data;
}


// removeFront : remove and return element from the front of the list.
//               returns the removed object on success, returns NULL on
//               failure (if the list is empty.)
template <class T>
T LinkedList<T>::removeFront(void)
{
	LinkedListNode<T> *entry;
	T data;

	// if there's no first element (ie, the list is empty) there's nothing
	// to remove.
	if(_first == NULL)
		return NULL;

	// get the head node and get the data so we can return it after the
	// head node is removed.
	entry = _first;
	data = entry->data;

	// the second node becomes the head node
	_first = entry->next;

	// if the second node was NULL then the list is now empty, so update
	// _last.  if not, the new head node should have no previous entry.
	if(_first == NULL)
		_last = NULL;
	else
		_first->previous = NULL;

	// free the memory taken up by the node.  note that we don't free
	// the data that was passed to us by the caller.
	delete entry;

	// decrement the count
	_count--;

	// return the data that was in the list
	return data;
}


// removeTail : remove and return element from the end of the list.
//              returns the removed object on success, returns NULL on
//              failure (if the list is empty.)
template <class T>
T LinkedList<T>::removeTail(void)
{
	LinkedListNode<T> *entry;
	T data;

	// if there's no tail element (ie, the list is empty) there's nothing
	// to remove.
	if(_last == NULL)
		return NULL;

	// get the tail node and get the data so we can return it after the
	// tail node is removed.
	entry = _last;
	data = entry->data;

	// the second to last node becomes the tail node
	_last = entry->previous;

	// if the second to last node was NULL then the list is now empty, so
	// update _first.  if not, the new tail node should have no previous
	// entry.
	if(_last == NULL)
		_first = NULL;
	else
		_last->next = NULL;

	// free the memory taken up by the node.  note that we don't free
	// the data that was passed to us by the caller.
	delete entry;

	// decrement the count
	_count--;

	// return the data that was in the list
	return data;
}


// fetchFront : returns the element at the front of the linked list
template <class T>
T LinkedList<T>::fetchFront(void)
{
	LinkedListNode<T> *entry;
	T data;

	// if there's no front element (ie, the list is empty) there's nothing
	// to fetch
	if(_first == NULL)
		return NULL;

	// get the tail node and return the data
	entry = _first;
	data = entry->data;

	return data;
}


// fetchTail : returns the element at the tail of the linked list
template <class T>
T LinkedList<T>::fetchTail(void)
{
	LinkedListNode<T> *entry;
	T data;

	// if there's no tail element (ie, the list is empty) there's nothing
	// to fetch
	if(_last == NULL)
		return NULL;

	// get the tail node and return the data
	entry = _last;
	data = entry->data;

	return data;
}


// fetchNumber : returns an item by index
template <class T>
T LinkedList<T>::fetchNumber(uint16_t idx)
{
	LinkedListNode<T> *entry;
	uint16_t cnt = 0;

	for(entry = _first; entry != NULL; entry = entry->next)
	{
		if(++cnt == idx)
			return entry->data;
	}

	return NULL;
}


// getCount : returns number of nodes in the linked list
template <class T>
uint32_t LinkedList<T>::getCount(void)
{
	return _count;
}


// debug : dump the contents of the linked list
template <class T>
void LinkedList<T>::debug(void)
{
	uint32_t i = 0;
	LinkedListNode<T> *entry;

	// walk the linked list and dump nodes
	for(entry = _first; entry != NULL; entry = entry->next)
	{
		printf("NODE %d (at mem %0.8lx)--\n", i, entry);
		printf("  prev = %0.8lx\n", entry->previous);
		printf("  next = %0.8lx\n", entry->next);
		printf("  data = %0.8lx\n", entry->data);
		printf("\n");
		i++;
	}
}


// clear : clears the linked list
template <class T>
void LinkedList<T>::clear(void)
{
	LinkedListNode<T> *entry, *next;

	// walk the linked list and destroy nodes
	for(entry = _first; entry != NULL; entry = next)
	{
		next = entry->next;

		delete entry;
	}

	// the linked list starts out empty - clear the head and tail node
	_first = NULL;
	_last = NULL;

	// clear the cursor so that the first call to iterate() starts at
	// the beginning
	_itr_prev = NULL;

	// we keep a count of the elements in the list -- clear it
	_count = 0;
}


// destructor : destroys a linked list
template <class T>
LinkedList<T>::~LinkedList()
{
	LinkedListNode<T> *entry, *next;

	// walk the linked list and destroy nodes
	for(entry = _first; entry != NULL; entry = next)
	{
		next = entry->next;

		delete entry;
	}
}

#endif // LINKEDLIST_H
