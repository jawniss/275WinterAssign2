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
    //return heap[0]; // return minimum pair
    return heap.at(0);
  }
  void print() {
    for (int i = 0; i < heap.size(); i++) {
      std::pair<T, K> show;
      show = heap.at(i);
      cout << show.first << ' '<< show.second << endl;
    }
  }

  // insert an item with the given key
  // if the item is already in the heap, will still insert a new copy with this key
  void insert(const T& item, const K& key){
  // steps on how to do this
  //1) Pass item of key value where v is new vertex in the tree ie let v be a new vertex in the tree
  //2) v is node that contains the pair(item,key) ie give v the pair(item,key)
  //3) While loop (v is not the root and key(v)<key(parent(v)) )
  //4) swap items and key between v and its parent if its true
  //5) go up the tree and keep doing the same by letting v = parent(v)
    std::pair<T,K > v;// this is v
    v.first = item; // two vertexes
    v.second = key; //manhatten distance;


    heap.push_back(v);// pushing v into vector
    int childIndex = size()-1; // subratact one since vectors are indexed from zero
    // need to find the position of v we know initialy the position of v is the size
    int parentIndex = parentindex(childIndex); // position of the parent

    // this one probably doesn't work
    //int keyParent = heap.second[parentIndex]

    std::pair<T,K > pairParent = heap.at(parentIndex);// key of parent
    int keyParent = pairParent.second;

    std::pair<T,K > firstPosition = heap.at(0);
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
  // pop the minimum item from the heap ie the very first index item in vector using min() first
  void popMin(){
    /*
    if (size() == 0){
      cout << "heap is empty" <<endl;
    }else{*/
      //std::pair<T,K > root = min();

      int lastIndex = size()-1; // last index
      //cout << "(inside)last index: " << lastIndex << endl;
      int rootIndex = 0;
      int vIndex;
      int rightIndex;
      int leftIndex;
      int vCost;
      int rightCost;
      int leftCost;
      //bool heapProperty = false;
      int difference;
      iter_swap(heap.begin() + rootIndex, heap.begin() + lastIndex);// swap the root with the first and last
      heap.pop_back(); // pop the last item from the vector (originally the root)
      //std::pair<T,K > v = min(); // set v is the root vertex
      vIndex = rootIndex;
      //cout << "(inside)vindex: " << vIndex << endl;
      std::pair<T, K> v = heap.at(vIndex);
      vCost = v.second;

      rightIndex = rightChildIndex(vIndex);
      //cout << "(inside)rightindex: " << rightIndex << endl;
      std::pair<T, K> right = heap.at(rightIndex);
      rightCost = right.second;

      leftIndex = leftChildIndex(vIndex);
      //cout << "(inside)leftindex: " << leftIndex << endl;
      std::pair<T, K> left = heap.at(leftIndex);
      leftCost = left.second;

      // since the heap vector we bring in is already all in order except root we move down the tree
      // while heapProperty not satisfied
      //while(heapProperty == false){
        while ((vCost > leftCost) || (vCost > rightCost)){
          /*
          cout << "went in whiel" << endl;

          cout << "(notsatisfied)vCOst: " << vCost<< endl;
          cout << "(notsatisfied)rightcost: " << rightCost << endl;
          cout << "(notsatisfied)leftcost: " << leftCost << endl;
          */

          difference = leftCost - rightCost;
          if (difference <= 0){ // case where left side is smaller
            // traverse the left side
            lastIndex = size()-1;

            //std::pair< std::pair<long long, long long>, long long> u = heap.at(leftIndex); // child of v with smallest key
            iter_swap(heap.begin() + vIndex, heap.begin() + leftIndex);// swap item and key between u and v
            vIndex = leftIndex;
            v = heap.at(vIndex);
            vCost = v.second;


            rightIndex = rightChildIndex(vIndex);
            if (rightIndex > lastIndex){
              break;
            }else{
              right = heap.at(rightIndex);
              rightCost = right.second;
            }

            leftIndex = leftChildIndex(vIndex);
            if (leftIndex > lastIndex){
              break;
            }else{
              left = heap.at(leftIndex);
              leftCost = left.second;
            }
            /*
            cout << "after swap to left rightindex: " << rightIndex << endl;
            cout << "after swap to left leftindex: " << leftIndex << endl;
            */
            /*
            cout << "(leftcase)vCOst: " << vCost<< endl;
            cout << "(leftcase)rightcost: " << rightCost << endl;
            cout << "(leftcase)leftcost: " << leftCost << endl;
            */

          } else if (difference > 0){ // case where right side is smaller
            lastIndex = size()-1;

            iter_swap(heap.begin() + vIndex, heap.begin() + rightIndex);// swap item and key between u and v
            vIndex = rightIndex;
            v = heap.at(vIndex);
            vCost = v.second;

            rightIndex = rightChildIndex(vIndex);
            if (rightIndex > lastIndex){
              break;
            }else{
              right = heap.at(rightIndex);
              rightCost = right.second;
            }

            leftIndex = leftChildIndex(vIndex);
            if (leftIndex > lastIndex){
              break;
            }else{
              left = heap.at(leftIndex);
              leftCost = left.second;
            }
            /*
            cout << "(rightcase)vCOst: " << vCost<< endl;
            cout << "(rightcase)rightcost: " << rightCost << endl;
            cout << "(rightcase)leftcost: " << leftCost << endl;
            */
            /*
            cout << "after swap to right rightindex: " << rightIndex << endl;
            cout << "after swap to right leftindex: " << leftIndex << endl;
            */
          }
        }
      //}
    //}
    //cout <<"done" << endl;
  }


  // returns the number of items held in the heap
  int size() const{
    // get the size of current instance of graph
    int size = heap.size();
    return size;
  }

  /*
  int parentindex(int i);

  int leftChildIndex(int i);

  int rightChildIndex(int i);
  */

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

};





#endif
