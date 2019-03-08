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

  // return the minimum v in the heap
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


  int parentindex(int i);

  int leftChildIndex(int i);

  int rightChildIndex(int i);

private:
  // the array holding the heap
  std::vector< std::pair<T, K> > heap; // holds current instance of the graph we are considering

  // feel free to add appropriate private methods to help implement some functions
};

std::pair<T, K> min() const{
  return heap.at(0); // return minimum pair
}

int parentindex(int i) {
  return floor((i-1)/2);
}

int leftChildIndex(int i){
  return ((2*i) + 1);
}

int rightChildIndex(int i){
  return ((2*i) + 2);
}

void popMin(){
  if (size() == 0){
    cout << "heap is empty" <<endl;
  }else{
    std::pair< std::pair<long long, long long>, long long> root = min();
    int lastIndex = size()-1; // last index
    int rootIndex = 0;
    int vIndex;
    int rightIndex;
    int leftIndex;
    //bool heapProperty = false;
    int difference;
    iter_swap(heap.begin() + rootIndex, heap.begin() + lastIndex);// swap the root with the
    heap.pop_back(); // pop the last item from the vector
    std::pair< std::pair<long long, long long>, long long> v = min(); // set v is the root vertex
    vIndex = v.second;
    rightIndex = rightChildIndex(vIndex);
    leftIndex = leftChildIndex(vIndex);
    // since the heap vector we bring in is already all in order except root we move down the tree
    // while heapProperty not satisfied
    //while(heapProperty == false){
      while ((vIndex > leftIndex) && (vIndex > rightIndex)){
        difference = leftIndex - rightIndex;
        if (difference < 0){ // case where left side is smaller
          // traverse the left side
          //std::pair< std::pair<long long, long long>, long long> u = heap.at(leftIndex); // child of v with smallest key
          iter_swap(heap.begin() + vIndex, heap.begin() + leftIndex);// swap item and key between u and v
          vIndex = leftIndex;
          rightIndex = rightChildIndex(vIndex);
          leftIndex = leftChildIndex(vIndex);
        } else if (difference > 0){ // case where right side is smaller
          iter_swap(heap.begin() + vIndex, heap.begin() + rightIndex);// swap item and key between u and v
          vIndex = rightIndex;
          rightIndex = rightChildIndex(vIndex);
          leftIndex = leftChildIndex(vIndex);
        }
      }
    //}
  }
}

int size() const{
  // get the size of current instance of graph
  int size = heap.size();
  return size;
}


void insert(const T& item, const K& key) {
  /*
  // probably set item stuff elsewhere
  std::pair<long long, long long> item;
  item.first = vertex1; //first vertex key when we add later
  item.second = vectex2; //second vertex key when we add later
  */

  std::pair< std::pair<long long, long long>, long long> v;// this is v
  v.first = item; // two vertexes
  v.second = key; //manhatten distance;


  heap.push_back(v);// pushing v into vector
  int childIndex = size()-1; // subratact one since vectors are indexed from zero
  // need to find the position of v we know initialy the position of v is the size
  int parentIndex = parentindex(childIndex); // position of the parent

  // this one probably doesn't work
  //int keyParent = heap.second[parentIndex]

  std::pair< std::pair<long long, long long>, long long> pairParent = heap.at(parentIndex);// key of parent
  int keyParent = pairParent.second;

  std::pair< std::pair<long long, long long>, long long> firstPosition = heap.at(0);
  //std::pair< std::pair<long long, long long>, long long> firstPosition = min(); // could be better
  int rootCost = firstPosition.second;

  //while ((v.second != rootCost) && (v.second < keyParent )){
  while ((key != rootCost) && (key < keyParent)){
    // swap the items and keys between v and parent(v)
    iter_swap(heap.begin() + parentIndex, heap.begin() + childIndex);
    // setting the indexes and setting v to be the parent(v) locations
    childIndex = parentIndex;
    parentIndex = parentindex(childIndex); // index of new parent
    pairParent = heap.at(parentIndex);// key of new parent
    keyParent = pairParent.second;

    // rechecking who is the root
    firstPosition = heap.at(0);
    //firstPosition = min(); // could be better
    rootCost = firstPosition.second;
  }
}


#endif
