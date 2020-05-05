/*
 * Copyright (C) 2012 - Ben Hallstein - http://ben.am - :-)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *  
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of
 * the Software.
 *  
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
 * THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 */

/*
 * ABOUT MISTERHEAPY
 * 
 * MisterHeapy is a binary heap for storing objects. You pass it pointers to your objects, which must implement
 * operator<, and it sorts them into a heap, which can then be updated, by index or by reference.
 * 
 * Your objects must also by memory contiguous - for instance, held in std::vector or an array.
 * Additionally, the first item pushed or fast-pushed to the heap must be the first in memory.
 * 
 * It is bog standard, but has capabilities not provided by std's heap functions:
 *
 *     1. It supports live updating of elements by index in average O(1) time (worst-case logN) with `update_at(index)`.
 *     2. By maintaining a lookup table, MisterHeapy enables you to update any object directly, without having to know
 *        its index in the heap.
 *        For instance, to update an object *x, you can simply call `update(x)`.
 *        The lookup table is O(1), so this is as fast as updating by index.
 * 
 * 
 * ~~ Crafting your objects ~~
 * 
 * To be used with MisterHeapy, your objects must implement operator<.
 *    - This should take a pointer to another object X of the same class, and compare itself to X, e.g.:
 *        bool operator< (const MyNode *p) {
 *            return this->x < p->x;
 *        }
 * 
 * 
 * ~~ Creating a MisterHeapy instance ~~
 * 
 * MisterHeapy is a templated class. If your object type is `Node`:
 *     `MisterHeapy<Node> myheapy(32);`
 * The parameter is the maximum size of your heap. Attempting to push elements beyond this limit will fail.
 * 
 * 
 * ~~ Initializing the heap ~~
 * 
 * To set up your initial heap, you can call `push` to add elements.
 *   - If you need to feed it a large initial dataset, you may use fast_push instead, which prevents objects from
 *     being sorted as they are added.
 *   - Since this disorders the heap, though, you MUST then call `reheapify`, which is O(n).
 * 
 * 
 * ~~ IMPORTANT WARNING ~~
 * 
 * MisterHeapy assumes some things about your elements:
 *   - that they are all contiguous in memory (i.e. they are held in a vector or contiguously allocated)
 *   - that the first element pushed or fast_pushed after instantiating or resetting() IS THE FIRST in said
 *     contiguous area of memory
 *   - obviously enough, that your elements do not move in memory after you have added pointers to them to the heap
 *
 * Note: you can disable index tracking, and the ability to update by reference, by #defining _MH_NO_INDEX_TRACKING
 *       before #including MisterHeapy.h
 * 
 */

#ifndef MISTERHEAPY_H
#define MISTERHEAPY_H

#include <stdlib.h>

#define MH_2_TO_THE(x) (1 << (x))

#ifdef _MH_NO_INDEX_TRACKING
#define _MH_IT(x)
#else
#define _MH_IT(x) x
#endif

template <class nodetype>
class MisterHeapy {
public:
	MisterHeapy(int max_size);
	~MisterHeapy();
	
	// Copy constr.
	MisterHeapy(const MisterHeapy &);
	void copyFrom(const MisterHeapy &);
	
	
	void reset();				// Makes the heap effectively as-new.
								// Where possible, reuse heaps rather than creating new ones – this is faster, since
								// it misses out the memory allocations that occur on construct.

	void fast_push(nodetype *x);	// Push onto the heap without sorting into place. Use to initialize, then call...
	void reheapify();				// Set the heap back in order.

	void push(nodetype*);
	nodetype* pop();
	int size();

	void update_at(int i);			// Move the node at the given location up/down the heap until order is restored
_MH_IT(
	void update(nodetype *x);		// Do the same for a given node (without knowing its index)
)

private:
	// Methods
	void swap_nodes(int ind1, int ind2);
	void up_heap(int _ind);
	void down_heap(int _ind);
	void sort_heap();
	
	inline int log_base2(unsigned int x) {
		int ind = 0;
		while (x >>= 1) ++ind;
		return ind;
	}
	
	// Properties
	int n;						// Allocated size
	int length;					// Current size
	nodetype **heap;			// The heap
	
_MH_IT(
	int *indices_in_heap;		// indices_in_heap[pointer to x] = index in heap of x
	nodetype *x_start;			// Pointer to first X
)
};


/* Implementation (a templated class's implementation must be in the same file as its declaration.) */

template <class nodetype>
MisterHeapy<nodetype>::MisterHeapy(int _n) : n(_n) {
	heap = (nodetype**) malloc(sizeof(nodetype*) * n);
	_MH_IT(
		indices_in_heap = (int*) malloc(sizeof(int) * n);
	)
	reset();
}

template <class nodetype>
MisterHeapy<nodetype>::MisterHeapy(const MisterHeapy &h) :
	n(0),
	length(0),
	heap(NULL)
{
	_MH_IT(
		x_start = 0;
		indices_in_heap = NULL;
	)
	copyFrom(h);
}

template <class nodetype>
void MisterHeapy<nodetype>::copyFrom(const MisterHeapy &h) {
	if (n != h.n) {
		if (heap) {
			free(heap);
			_MH_IT( free(indices_in_heap); )
		}
		heap = (nodetype**) malloc(sizeof(nodetype*) * h.n);
		_MH_IT(
			indices_in_heap = (int*) malloc(sizeof(int) * h.n);
		)
	}
	n = h.n;
	length = h.length;
	memcpy(heap, h.heap, sizeof(nodetype*) * n);
	_MH_IT(
		x_start = h.x_start;
		memcpy(indices_in_heap, h.indices_in_heap, sizeof(int) * n);
	)
}


template <class nodetype>
MisterHeapy<nodetype>::~MisterHeapy() {
	free(heap);
	_MH_IT( free(indices_in_heap); )
}

template <class nodetype>
void MisterHeapy<nodetype>::reset() {
	length = 0;
}

template <class nodetype>
void MisterHeapy<nodetype>::fast_push(nodetype *x) {
	if (length >= n)
		return;
	int i = length++;
	heap[i] = x;
	_MH_IT(
		if (i == 0) x_start = x;
		indices_in_heap[x - x_start] = i;
	)
}

template <class nodetype>
inline void MisterHeapy<nodetype>::swap_nodes(int ind1, int ind2) {
	nodetype *x1 = heap[ind1], *x2 = heap[ind2];
	heap[ind1] = x2;
	heap[ind2] = x1;
	_MH_IT(
		indices_in_heap[x2 - x_start] = ind1;
		indices_in_heap[x1 - x_start] = ind2;
	)
}
template <class nodetype>
void MisterHeapy<nodetype>::up_heap(int _ind) {
	int ind = _ind, parent_ind = (ind-1)/2;
	
	while (ind > 0 && *heap[parent_ind] < heap[ind]) {
		swap_nodes(parent_ind, ind);
		ind = parent_ind;
		parent_ind = (ind-1)/2;
	}
}
template <class nodetype>
void MisterHeapy<nodetype>::down_heap(int _ind) {
	int ind = _ind, l_child_ind = ind*2+1, r_child_ind = ind*2+2, larger_child_ind;
	nodetype *our_node = heap[ind];
	
	while (l_child_ind < length) {
		if (r_child_ind < length)
			larger_child_ind = (*heap[l_child_ind] < heap[r_child_ind] ? r_child_ind : l_child_ind);
		else
			larger_child_ind = l_child_ind;
		if (*our_node < heap[larger_child_ind]) {
			swap_nodes(ind, larger_child_ind);
			ind = larger_child_ind;
			l_child_ind = ind*2+1, r_child_ind = ind*2+2;
		}
		else
			break;
	}
}

template <class nodetype>
nodetype* MisterHeapy<nodetype>::pop() {
	nodetype *node_to_pop = heap[0];
	swap_nodes(0, --length);
	down_heap(0);
	return node_to_pop;
}
template <class nodetype>
void MisterHeapy<nodetype>::push(nodetype *x) {
	if (length >= n)
		return;
	heap[length] = x;
	_MH_IT(
		if (length == 0) x_start = x;
		indices_in_heap[x - x_start] = length++;	
	)
	up_heap(length - 1);
}

/* * * * Interlude – some useful heap-related formulae * * * *
 *                                                           *
 *  (indices and depth are zero-based)                       *
 *                                                           *
 *  depth given a number of nodes n:    log(n)               *
 *  depth at a given index x:           log(x+1)             *
 *  index of first element at row d:    2^(d) - 1            *
 *                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

template <class nodetype>
void MisterHeapy<nodetype>::reheapify() {
	// To sort a heap: start at the 2nd-lowest level, calling down_heap on the root of each subtree,
	// and do this for each level until the top of the tree is reached
	int i, j, depth, greatest_depth = log_base2(length);

	for (depth = greatest_depth - 1; depth >= 0; depth--)
		for (i = MH_2_TO_THE(depth) - 1, j = MH_2_TO_THE(depth + 1) - 1; i < j; i++)
			if (i < length)
				down_heap(i);
}
template <class nodetype>
void MisterHeapy<nodetype>::update_at(int i) {
	if (i >= length) return;
	int parent_i = (i-1)/2;	
	if (i == 0 || *heap[i] < heap[parent_i])
		down_heap(i);
	else
		up_heap(i);
}
_MH_IT(
template <class nodetype>
void MisterHeapy<nodetype>::update(nodetype *x) {
	update_at(indices_in_heap[x - x_start]);
}
)
template <class nodetype>
int MisterHeapy<nodetype>::size() {
	return length;
}

#endif
