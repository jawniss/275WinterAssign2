#ifndef _HEAP_H_
#define _HEAP_H_

#include <vector>
#include <utility> // for pair
#include <math.h>


using namespace std;
// T is the type of the item to be held
// K is the type of the key associated with each item in the heap
// The only requirement is that K is totally ordered and comparable via <
template <class T, class K>
class BinaryHeap {
 public:
    // constructor not required because the only "initialization"
    // is through the constructor of the variable "heap" which is called by default

    // return the minimum element in the heap
    pair<T, K> min() const {
        // return the root node
        return heap[0];
    }

    // insert an item with the given key
    // if the item is already in the heap, will still insert a new copy with this key
    void insert(const T& item, const K& key) {
        // let v be a new vertex at the end of the array/tree
        pair<T, K> v(item, key);
        heap.push_back(v);
        int index = heap.size() - 1;
        sift_up(index);
    }


    // pop the minimum item from the heap
    void popMin() {
        // load the root node
        //pair<T, K> root = heap.front();
        heap.erase(heap.begin());  // remove root node
        /*
        pair<T, K> lastPair = heap.back();
        heap.pop_back();
        heap.insert(heap.begin(), lastPair);
        */
        heap[0] = heap[heap.size() - 1];
        // heapify_down
        sift_down(0);
    }

    // returns the number of items held in the heap
    int size() const {
        // return the number of items in the heap
        return heap.size();
    } 

    private:
    // the array holding the heap
    vector<pair<T, K> > heap;

    // feel free to add appropriate private methods to help implement some functions

    // heapify_up
    // techie delight...
    // https://www.techiedelight.com/min-heap-max-heap-implementation-c/
    void sift_up(int i) {
        if (i && heap[goParent(i)] > heap[i]) {
            swap(heap[i], heap[goParent(i)]);
            sift_up(goParent(i));
        }
    }


    // heapify_down
    void sift_down(int i) {
        unsigned int leftIndex = goLeft(i);
        unsigned int rightIndex = goRight(i);
        int smallVal = i;

        if (leftIndex < heap.size() && heap[leftIndex] < heap[i]) {
            smallVal = leftIndex;
        }
        if (rightIndex < heap.size() && heap[rightIndex] < heap[smallVal]) {
            smallVal = rightIndex;
        }

        if (smallVal != i) {
            swap(heap[i], heap[smallVal]);
            sift_down(smallVal);
        }
    }

    int goLeft(int index) {
        return (2*index + 1);
    }

    int goRight(int index) {
        return(2*index + 2);
    }

    int goParent(int index) {
        return (floor((index - 1) / 2));
    }

};

#endif