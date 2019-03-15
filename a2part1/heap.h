#ifndef _HEAP_H_
#define _HEAP_H_

#include <vector>
#include <utility> // for pair
#include <algorithm> // neded for iter swap
#include <iostream>
#include <math.h> // needed for floor

// T is the type of the item to be held
// K is the type of the key associated with each item in the heap
// The only requirement is that K is totally ordered and comparable via <
using namespace std;

template <class T, class K>
class BinaryHeap {
public:
  // constructor not required because the only "initialization"
  // is through the constructor of the variable "heap" which is called by default

  // return the minimum v in the heap
  std::pair<T, K> min() const{
    return heap[0]; // return minimum pair
  }

  // insert an item with the given key
  // if the item is already in the heap, will still insert a new copy with this key
  void insert(const T& item, const K& key){
    // v the new vertex in the tree
    std::pair<T,K > v;
    v.first = item;
    v.second = key;
    // pushing v into vector
    heap.push_back(v);
    // subratact one since vectors are indexed from zero
    int childIndex = size()-1;
    // need to find the position of v we know initialy the position of v is the size
    int parentIndex = parentindex(childIndex);
    std::pair<T,K > pairParent = heap[parentIndex];
    int keyParent = pairParent.second;
    std::pair<T,K > firstPosition = heap[0];
    int rootCost = firstPosition.second;
    // while v is not the root and key(v) < key(parent(V))
    while ((key != rootCost) && (key < keyParent)){
      // swap the items and keys between v and parent(v)
      iter_swap(heap.begin() + parentIndex, heap.begin() + childIndex);
      // setting the indexes and setting v to be the parent(v) locations
      childIndex = parentIndex;
      // index of new parent
      parentIndex = parentindex(childIndex);
      // key of new parent
      pairParent = heap[parentIndex];
      keyParent = pairParent.second;
      // rechecking what is the root
      firstPosition = heap[0];
      rootCost = firstPosition.second;
      }
  }
  // pop the minimum item from the heap ie the very first index item in vector using min() first
  void popMin(){
    int lastIndex = size()-1;
    int minIndex = 0;
    // swap the root with the first and last
    iter_swap(heap.begin() + minIndex, heap.begin() + lastIndex);
    // pop the last item from the vector (originally the root)
    heap.pop_back();
    // call heapify on the first index
    minHeapify(0);
  }

  // returns the number of items held in the heap
  int size() const{
    // get the size of current instance of graph
    int size = heap.size();
    return size;
  }

private:
  // the array holding the heap
  std::vector< std::pair<T, K> > heap;

  // function that finds the parent index given an index
  int parentindex(int i) {
    return (floor((i-1)/2));
  }

  // function that finds left child index given an index
  int leftChildIndex(int i){
    return ((2*i) + 1);
  }

  // function that finds the right child index givenan index
  int rightChildIndex(int i){
    return ((2*i) + 2);
  }

  // heapify source: https://www.geeksforgeeks.org/binary-heap/
  // takes in an index then traverse that index until heap condition is satisfied
  void minHeapify(int index){
    int lChild = 0;
    int rChild = 0;
    int min = index;
    int length = heap.size();
    lChild = leftChildIndex(index);
    rChild = rightChildIndex(index);
    // if left child index less than length and value at that childs index is less than value at inputed index
    if (lChild < length && heap[lChild].second < heap[index].second) {
        min = lChild;
    }
    // if right child index less than length and value at that childs index is less than value at inputed index 
    if (rChild < length && heap[rChild].second < heap[min].second) {
        min = rChild;
    }
    //if we changed min index swap the positions of those and heapify again
    if (min != index) {
        swap(heap[index], heap[min]);
        minHeapify(min);
    }
  }

};

#endif
