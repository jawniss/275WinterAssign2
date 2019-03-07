#ifndef _HEAP_H_
#define _HEAP_H_

#include <vector>
#include <utility> // for pair

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

  // pop the minimum item from the heap
  void popMin();
  //1) check the heap isn't empty
  //2)

  //1) check the heap isn't empty
  //2)

  // returns the number of items held in the heap
  int size() const;

private:
  // the array holding the heap
  std::vector< std::pair<T, K> > heap;

  // feel free to add appropriate private methods to help implement some functions
};



pair<long long, long long> complexNum;

void insert(const T& item, const K& key) {
  v =
}

#endif
