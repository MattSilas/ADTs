/*	dynArr.c: Dynamic Array implementation. */
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include "dynamicArray.h"
#include "toDoList.h"

struct DynArr
{
	TYPE *data;		/* pointer to the data array */
	int size;		/* Number of elements in the array */
	int capacity;	/* capacity ofthe array */
};

struct bag
{
	struct DynArr *dynArr;
};


/* ************************************************************************
	Dynamic Array Functions
************************************************************************ */

/* Initialize (including allocation of data array) dynamic array.

	param: 	v		pointer to the dynamic array
	param:	cap 	capacity of the dynamic array
	pre:	v is not null
	post:	internal data array can hold capacity elements
	post:	v->data is not null
*/
void _initDynArr(DynArr *v, int capacity)
{
	assert(capacity > 0);
	assert(v!= 0);
	v->data = malloc(sizeof(TYPE) * capacity);
	assert(v->data != 0);
	v->size = 0;
	v->capacity = capacity;

}

/* Allocate and initialize dynamic array.

	param:	cap 	desired capacity for the dyn array
	pre:	none
	post:	none
	ret:	a non-null pointer to a dynArr of cap capacity
			and 0 elements in it.		
*/
DynArr* createDynArr(int cap)
{
	DynArr *r; 
	assert(cap > 0);
	r = malloc(sizeof( DynArr));
	assert(r != 0);
	_initDynArr(r,cap);
	return r;
}

/* Deallocate data array in dynamic array. 

	param: 	v		pointer to the dynamic array
	pre:    v is not null
	post:	d.data points to null
	post:	size and capacity are 0
	post:	the memory used by v->data is freed
*/
void freeDynArr(DynArr *v)
{
	assert(v!=0);

	if(v->data != 0)
	{
		free(v->data); 	/* free the space on the heap */
		v->data = 0;   	/* make it point to null */
	}
	v->size = 0;
	v->capacity = 0;
}

/* Deallocate data array and the dynamic array ure. 

	param: 	v		pointer to the dynamic array
	pre:	v is not null
	post:	the memory used by v->data is freed
	post:	the memory used by d is freed
*/
void deleteDynArr(DynArr *v)
{
	assert (v!= 0);
	freeDynArr(v);
	free(v);
}

/* Resizes the underlying array to be the size cap 

	param: 	v		pointer to the dynamic array
	param:	cap		the new desired capacity
	pre:	v is not null
	post:	v has capacity newCap
*/
void _dynArrSetCapacity(DynArr *v, int newCap)
{
	int i;
	TYPE *oldData;
	int oldSize = v->size;
	oldData = v->data;

	printf("========Resizing========\n");
	/* Create a new dyn array with larger underlying array */
	_initDynArr(v, newCap);

	for(i = 0; i < oldSize; i++){
		v->data[i] = oldData[i];
	}

	v->size = oldSize;
	/* Remember, init did not free the original data */
	free(oldData);

#ifdef ALTERNATIVE
	int i;
	
	/* Create a new underlying array*/
	TYPE *newData = (TYPE*)malloc(sizeof(TYPE)*newCap);
	assert(newData != 0);
	
	/* copy elements to it */
	
	for(i = 0; i < v->size; i++)
	{
		newData[i] = v->data[i];
	}
	
	/* Delete the oldunderlying array*/
	free(v->data);
	/* update capacity and size and data*/
	v->data = newData;
	v->capacity = newCap;
#endif
}

/* Get the size of the dynamic array

	param: 	v		pointer to the dynamic array
	pre:	v is not null
	post:	none
	ret:	the size of the dynamic array
*/
int sizeDynArr(DynArr *v)
{
	assert(v!=0);
	return v->size;
}

/* 	Adds an element to the end of the dynamic array

	param: 	v		pointer to the dynamic array
	param:	val		the value to add to the end of the dynamic array
	pre:	the dynArry is not null
	post:	size increases by 1
	post:	if reached capacity, capacity is doubled
	post:	val is in the last utilized position in the array
*/
void addDynArr(DynArr *v, TYPE val)
{

	assert(v!=0);

	/* Check to see if a resize is necessary */
	if(v->size >= v->capacity)
		_dynArrSetCapacity(v, 2 * v->capacity);
	
	v->data[v->size] = val;
	v->size++;

}

/*	Get an element from the dynamic array from a specified position
	
	param: 	v		pointer to the dynamic array
	param:	pos		integer index to get the element from
	pre:	v is not null
	pre:	v is not empty
	pre:	pos < size of the dyn array and >= 0
	post:	no changes to the dyn Array
	ret:	value stored at index pos
*/

TYPE getDynArr(DynArr *v, int pos)
{
	assert(v!=0);
	assert(pos < v->size);
	assert(pos >= 0);
   
	return v->data[pos];
}

/*	Put an item into the dynamic array at the specified location,
	overwriting the element that was there

	param: 	v		pointer to the dynamic array
	param:	pos		the index to put the value into
	param:	val		the value to insert 
	pre:	v is not null
	pre:	v is not empty
	pre:	pos >= 0 and pos < size of the array
	post:	index pos contains new value, val
*/
void putDynArr(DynArr *v, int pos, TYPE val)
{
	assert(v!=0);
	assert(pos < v->size);
	assert(pos >= 0);
	v->data[pos] = val;
}

/*	Swap two specified elements in the dynamic array

	param: 	v		pointer to the dynamic array
	param:	i,j		the elements to be swapped
	pre:	v is not null
	pre:	v is not empty
	pre:	i, j >= 0 and i,j < size of the dynamic array
	post:	index i now holds the value at j and index j now holds the value at i
*/
void swapDynArr(DynArr *v, int i, int  j)
{
	TYPE  temp;
	assert(v!=0);
	assert(i < v->size);
	assert(j < v->size);
	assert(i >= 0);
	assert(j >= 0);

	temp = v->data[i];
	v->data[i] = v->data[j];
	v->data[j] = temp;

}

/*	Remove the element at the specified location from the array,
	shifts other elements back one to fill the gap

	param: 	v		pointer to the dynamic array
	param:	idx		location of element to remove
	pre:	v is not null
	pre:	v is not empty
	pre:	idx < size and idx >= 0
	post:	the element at idx is removed
	post:	the elements past idx are moved back one
*/
void removeAtDynArr(DynArr *v, int idx){
	int i;
	assert(v!= 0);
	assert(idx < v->size);
	assert(idx >= 0);

   //Move all elements up

   for(i = idx; i < v->size-1; i++){
      v->data[i] = v->data[i+1];
   }

   v->size--;

}



/* ************************************************************************
	Stack Interface Functions
************************************************************************ */

/*	Returns boolean (encoded in an int) demonstrating whether or not the 
	dynamic array stack has an item on it.

	param:	v		pointer to the dynamic array
	pre:	v is not null
	post:	none
	ret:	>0  if empty, otherwise 0
*/
int isEmptyDynArr(DynArr *v)
{
	assert(v!= 0);
	return !(v->size);
	/* alternatively:

	if(v->size == 0)
		return 1;
	else return 0;

	*/



}

/* 	Push an element onto the top of the stack

	param:	v		pointer to the dynamic array
	param:	val		the value to push onto the stack
	pre:	v is not null
	post:	size increases by 1
			if reached capacity, capacity is doubled
			val is on the top of the stack
*/
void pushDynArr(DynArr *v, TYPE val)
{
	assert(v!=0);
	addDynArr(v, val);
}

/*	Returns the element at the top of the stack 

	param:	v		pointer to the dynamic array
	pre:	v is not null
	pre:	v is not empty
	post:	no changes to the stack
*/
TYPE topDynArr(DynArr *v)
{
	assert(v!=0);
	assert(!isEmptyDynArr(v));
	return v->data[v->size-1];
}

/* Removes the element on top of the stack 

	param:	v		pointer to the dynamic array
	pre:	v is not null
	pre:	v is not empty
	post:	size is decremented by 1
			the top has been removed
*/
void popDynArr(DynArr *v)
{
	assert(v!=0);
	assert(! isEmptyDynArr(v));
	v->size--; 
}

/* ************************************************************************
	Bag Interface Functions
************************************************************************ */

/*	Returns boolean (encoded as an int) demonstrating whether or not
	the specified value is in the collection
	true = 1
	false = 0

	param:	v		pointer to the dynamic array
	param:	val		the value to look for in the bag
	pre:	v is not null
	pre:	v is not empty
	post:	no changes to the bag
*/
int containsDynArr(DynArr *v, TYPE val)
{
	int i = 0;

	assert(v!=0);
	assert(!isEmptyDynArr(v));
   
	for(i = 0; i < sizeDynArr(v); i++)
      if(i==1)
         return 1;
      return 0;

}

/*	Removes the first occurrence of the specified value from the collection
	if it occurs

	param:	v		pointer to the dynamic array
	param:	val		the value to remove from the array
	pre:	v is not null
	pre:	v is not empty
	post:	val has been removed
	post:	size of the bag is reduced by 1
*/
void removeDynArr(DynArr *v, TYPE val)
{
	int i = 0;
	assert(v!=0);
	assert(!isEmptyDynArr(v));
	assert(containsDynArr(v,val));  /* Design decision: Error if they try to remove something not in there! */

	for(i = 0; i < sizeDynArr(v); i++)
      if(i==1)
      {
           removeAtDynArr(v,i); 
           break;
      }
}

/* Utility function for debugging */
void _printDynArr(struct DynArr *da)
{
	int i;
	for(i=0; i < da->size; i++)
		printf("DA[%d] == %d\n", i, da->data[i].priority);
}

/*Bag Wrapper Interface */

/* We'll cover this around week 3 - when we talk about ways to try to make
 * user code more general and interchangeable.  This is nice because it completely
 * hides the underlying data structure type and allows users to write code using only
 * these functions for a 'bag'.  If they substitute a bag implemented with another data
 * struct, like a LinkedList, then they dont' have to change their code if that LinkedList
 * uses teh same name  (ie. bag).  HOWEVER, there's one big problem!!  The user CANNOT use
 * two different bag implementations in the same coe  (e.g. this dynamic array bag and a linked list bag)
 *
 * C does not have polymorphic variables
 */

struct bag *createBag()
{
	struct bag *myBag = malloc(sizeof(struct bag));
	myBag->dynArr = createDynArr(20);
	return myBag;
}
void addToBag(struct bag* b, TYPE val)
{
	addDynArr(b->dynArr, val);
}
void removeFromBag(struct bag* b, TYPE val)
{
	removeDynArr(b->dynArr, val);
}
int containsBag(struct bag* b, TYPE val)
{
	return(containsDynArr(b->dynArr, val));
}
int isEmptyBag(struct bag* b)
{
	return(isEmptyDynArr(b->dynArr));
}

void printBag(struct bag *b)
{
	_printDynArr(b->dynArr);
}

/*	Copy elements from a dynamic array to another dynamic array

	param: 	source	pointer to the source dynamic array
	param:	destination		pointer to the destination dynamic array
	pre:	s is not null and s is not empty
	post:	destination is initialized
	post: 	the elements from source are copied to destination
*/
void copyDynArr(DynArr *source, DynArr *destination)
{
  	int i;
	assert(source->size > 0);
	_initDynArr(destination, source->capacity);
	/* copy elements to destination array */
	for(i = 0; i < source->size; i++)
		destination->data[i] = source->data[i];

	destination->size = source->size;
}


/* ************************************************************************
	Heap-based Priority Queue Implementation
************************************************************************ */

/* internal function prototypes */
int _smallerIndexHeap(DynArr *heap, int i, int j);
void _adjustHeap(DynArr *heap, int max, int pos);

/*	Get the index of the smaller node between two nodes in a heap

	param: 	heap	pointer to the heap
	param:	i	index of one node
	param:	j	index of other node
	pre:	i < size and j < size
	ret:	the index of the smaller node
*/
int _smallerIndexHeap(DynArr *heap, int i, int j)
{
	assert(i<sizeDynArr(heap)&&j<sizeDynArr(heap));
	if(compare(heap->data[i],heap->data[j])==-1){
		return i;
	}
	return j;
}

/*	Get the first node, which has the min priority, from the heap

	param: 	heap	pointer to the heap
	pre:	heap is not empty
	ret:	value of first node
*/
TYPE getMinHeap(DynArr *heap)
{
	assert(sizeDynArr(heap)>0);
	return getDynArr(heap,0);
}

/*	Add a node to the heap

	param: 	heap	pointer to the heap
	param: 	node	node to be added to the heap
	pre:	heap is not null
	post:	node is added to the heap
*/
void addHeap(DynArr *heap, TYPE val)
{
	int parent;
	int pos = sizeDynArr(heap);
	addDynArr(heap,val);
	assert(sizeDynArr(heap)>0);
	while(pos!=0){
		parent = (pos-1)/2;
		if(compare(getDynArr(heap,pos),getDynArr(heap,parent))==-1){
			swapDynArr(heap,parent,pos);
			pos = parent;
		}
		else return;
	}
	_adjustHeap(heap, sizeDynArr(heap),pos);

}

/*	Adjust heap to maintain heap property

	param: 	heap	pointer to the heap
	param: 	max		index to adjust up to  (but not included)
	param: 	pos		position index where the adjustment starts
	pre:	max <= size
	post:	heap property is maintained for nodes from index pos to index max-1  (ie. up to, but not including max)
*/
void _adjustHeap(DynArr *heap, int max, int pos)
{

	int leftIdx = pos*2+1;
	int rghtIdx = pos*2+2;
	if(rghtIdx<max){
		int minIdx = _smallerIndexHeap(heap,leftIdx,rghtIdx);
		if(minIdx<pos){
			swapDynArr(heap,rghtIdx,minIdx);
		}
		_adjustHeap(heap,max,minIdx);
	}
	else if(leftIdx<max){
		if(compare(heap->data[leftIdx],heap->data[pos])==-1){
			swapDynArr(heap,leftIdx,pos);
		}
		_adjustHeap(heap,max,leftIdx);
	}
	return;
}

/*	Remove the first node, which has the min priority, from the heap

	param: 	heap	pointer to the heap
	pre:	heap is not empty
	post:	the first node is removed from the heap
*/
void removeMinHeap(DynArr *heap)
{
	assert(sizeDynArr(heap)>0);
	int last = sizeDynArr(heap)-1;
	assert(last!=0);
	putDynArr(heap,0,getDynArr(heap,last));
	removeAtDynArr(heap,last);
	_adjustHeap(heap,last,0);
}

/* builds a heap from an arbitrary dynArray

    param: v    dynamicArray
    pre:    v is not empty
    post: v is a proper heap
*/

void _buildHeap(DynArr *heap)
{
	assert(sizeDynArr(heap)>0);
    int	max = sizeDynArr(heap);
    int i;
    for(i=max/2-1;i>=0;i--){
    	_adjustHeap(heap,max,i);
    }
}
/*
    In-place sort of the heap

    param: heap     pointer to the heap
    pre: heap is not empty
    post: the dynArr is in reverse sorted order
*/

void sortHeap(DynArr *heap)
{
	assert(sizeDynArr(heap)>0);
	int i;
   _buildHeap(heap);
   for(i=sizeDynArr(heap)-1;i>0;i--){
	   swapDynArr(heap,0,i);
	   _adjustHeap(heap,i,0);
   }

}


