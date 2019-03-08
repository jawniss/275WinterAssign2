#ifndef _HEAP_H_
#define _HEAP_H_

#include <vector>
#include <utility> // for pair
#include <algorithm> // neded for iter swap

// T is the type of the item to be held
// K is the type of the key associated with each item in the heap
// The only requirement is that K is totally ordered and comparable via <
template <class T, class K>
class BinaryHeap {
public:
  // constructor not required because the only "initialization"
  // is through the constructor of the variable "heap" which is called by default

  // return the minimum element in the heap
  std::pair<T, K> min() const;

  // insert an item with the given key
  // if the item is already in the heap, will still insert a new copy with this key
  void insert(const T& item, const K& key);
  // steps on how to do this
  //1) Pass item of key value where v is new vertex in the tree ie let v be a new vertex in the tree
  //2) v is node that contains the pair(item,key) ie give v the pair(item,key)
  //3) While loop (v is not the root and key(v)<key(parent(v)) )
  //4) swap items and key between v and its parent if its true
  //5) go up the tree and keep doing the same by letting v = parent(v)

  // pop the minimum item from the heap ie the very first index item in vector using min() first
  void popMin();
  //1) check the heap isn't empty
  //2)

  // returns the number of items held in the heap
  int size() const;


  int parentindex(int i) {
    return floor((i-1)/2);
  }


private:
  // the array holding the heap
  std::vector< std::pair<T, K> > heap; // holds current instance of the graph we are considering

  // feel free to add appropriate private methods to help implement some functions
};

int size() const{
  // get the size of current instance of graph
  int size = heap.size();
  return size;
}


void insert(const T& item, const K& key) {
  // probably set item stuff elsewhere
  pair<long long, long long> item;
  item.first = vertex1; //first vertex key when we add later
  item.second = vectex2; //second vertex key when we add later


  pair< pair<long long, long long>, long long> element;// this is v
  element.first = item; // two vertexes
  element.second = key; //manhatten distance;


  heap.pushback(element);// pushing v into vector
  int childIndex = size()-1; // subratact one since vectors are indexed from zero
  // need to find the position of v we know initialy the position of v is the size
  int parentIndex = parentindex(childIndex); // position of the parent
  int keyParent = heap.second[parentIndex]// key of parent

  while ((element.second != heap.second[0]) && (element.second < keyParent )){
    // swap the items and keys between v and parent(v)
    iter_swap(heap.begin() + parentIndex, heap.begin() + childIndex;
    // setting the indexes and setting v to be the parent(v) locations
    childIndex = parentIndex;
    parentIndex = parentindex(childIndex);
    keyParent = heap.second[parentIndex]// key of new parent


  }
}


#endif
