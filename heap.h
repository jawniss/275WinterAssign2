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
    //return heap.at(0);
  }


  // insert an item with the given key
  // if the item is already in the heap, will still insert a new copy with this key
  void insert(const T& item, const K& key){
    std::pair<T,K > v;// this is v
    v.first = item; // two vertexes
    v.second = key; //manhatten distance;


    heap.push_back(v);// pushing v into vector
    int childIndex = size()-1; // subratact one since vectors are indexed from zero
    // need to find the position of v we know initialy the position of v is the size
    int parentIndex = parentindex(childIndex); // position of the parent

    std::pair<T,K > pairParent = heap[parentIndex];// key of parent
    int keyParent = pairParent.second;

    std::pair<T,K > firstPosition = heap[0];
    //std::pair< std::pair<long long, long long>, long long> firstPosition = min(); // could be better
    int rootCost = firstPosition.second;

    //while ((v.second != rootCost) && (v.second < keyParent )){
    while ((key != rootCost) && (key < keyParent)){
      // swap the items and keys between v and parent(v)
      iter_swap(heap.begin() + parentIndex, heap.begin() + childIndex);
      // setting the indexes and setting v to be the parent(v) locations
      childIndex = parentIndex;
      parentIndex = parentindex(childIndex); // index of new parent
      //pairParent = heap.at(parentIndex);// key of new parent
      pairParent = heap[parentIndex];// key of new parent
      keyParent = pairParent.second;

      // rechecking who is the root
      //firstPosition = heap.at(0);
      firstPosition = heap[0];
      //firstPosition = min(); // could be better
      rootCost = firstPosition.second;
      }
  }
  // pop the minimum item from the heap ie the very first index item in vector using min() first
  void popMin(){


  int lastIndex = size()-1; // last index
  int minIndex = 0;

  iter_swap(heap.begin() + minIndex, heap.begin() + lastIndex);// swap the root with the first and last
  heap.pop_back(); // pop the last item from the vector (originally the root)
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
  std::vector< std::pair<T, K> > heap; // holds current instance of the graph we are considering

  // feel free to add appropriate private methods to help implement some functions
  int parentindex(int i) {
    return (floor((i-1)/2));
  }

  int leftChildIndex(int i){
    return ((2*i) + 1);
  }

  int rightChildIndex(int i){
    return ((2*i) + 2);
  }

  void minHeapify(int index){
    int lChild = 0;
    int rChild = 0;
    int min = index;
    int length = heap.size();
    lChild = leftChildIndex(index);
    rChild = rightChildIndex(index);
    if (lChild < length && heap[lChild].second < heap[index].second) {
        min = lChild;
    }
    if (rChild < length && heap[rChild].second < heap[min].second) {
        min = rChild;
    }
    if (min != index) {
        swap(heap[index], heap[min]);
        minHeapify(min);
    }
  }

};

#endif
