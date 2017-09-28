// Blackjack Simulation : Utility Library : LinkedListNode
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
// A node in a linked list.  Contains pointers to the next and
// previous entries (making it suitable for a doubly-linked list)
// and a data entry.

// OH GOD, IT'S C++!!!!?!??!??!??!

#ifndef LINKEDLISTNODE_H
#define LINKEDLISTNODE_H

// forward declaration so we can expose to LinkedList class
template <class T>
class LinkedList;

template <class T>
class LinkedListNode
{
	public:
		LinkedListNode *next;		// next pointer
		LinkedListNode *previous;	// previous pointer
		T data;						// data pointer
};

#endif // LINKEDLISTNODE_H
