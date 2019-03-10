#include "heap.h"
#include <iostream>
using namespace std;

int main() {
    BinaryHeap<int, int> Bheap;

    cout << "size before insert: " << Bheap.size() << endl;
    Bheap.insert(21, 6);
    cout << "size after insert: "<< Bheap.size() << endl;
    cout << "heap min cost: "<<Bheap.min().second << endl;
    cout << "heap min pair "<<Bheap.min().first << endl;
    Bheap.insert(34, 7);
    cout << "size after insert: "<< Bheap.size() << endl;
    cout << "heap min cost: "<<Bheap.min().second << endl;
    cout << "heap min pair "<<Bheap.min().first << endl;
    Bheap.insert(54, 12);
    cout << "size after insert: "<< Bheap.size() << endl;
    cout << "heap min cost: "<<Bheap.min().second << endl;
    cout << "heap min pair "<<Bheap.min().first << endl;
    Bheap.insert(9, 10);
    cout << "size after insert: "<< Bheap.size() << endl;
    cout << "heap min cost: "<<Bheap.min().second << endl;
    cout << "heap min pair "<<Bheap.min().first << endl;
    Bheap.insert(11, 14);
    cout << "size after insert: "<< Bheap.size() << endl;
    cout << "heap min cost: "<<Bheap.min().second << endl;
    cout << "heap min pair "<<Bheap.min().first << endl;
    Bheap.insert(50, 13);
    cout << "size after insert: "<< Bheap.size() << endl;
    cout << "heap min cost: "<<Bheap.min().second << endl;
    cout << "heap min pair "<<Bheap.min().first << endl;


    Bheap.insert(2, 5);
    cout << "size after insert: "<< Bheap.size() << endl;
    cout << "heap min cost: "<<Bheap.min().second << endl;
    cout << "heap min pair "<<Bheap.min().first << endl;

    Bheap.insert(4,8);
    cout << "size after insert: "<< Bheap.size() << endl;
    cout << "heap min cost: "<<Bheap.min().second << endl;
    cout << "heap min pair "<<Bheap.min().first << endl;

    Bheap.print();

    /*
    Bheap.popMin();
    cout << "size after popmin: "<< Bheap.size() << endl;
    cout << "heap min cost: "<<Bheap.min().second << endl;
    cout << "heap min pair "<<Bheap.min().first << endl;
    Bheap.popMin();
    cout << "size after popmin: "<< Bheap.size() << endl;
    cout << "heap min cost: "<<Bheap.min().second << endl;
    cout << "heap min pair "<<Bheap.min().first << endl;
    Bheap.popMin();
    cout << "size after popmin: "<< Bheap.size() << endl;
    cout << "heap min cost: "<<Bheap.min().second << endl;
    cout << "heap min pair "<<Bheap.min().first << endl;
    Bheap.popMin();
    cout << "size after popmin: "<< Bheap.size() << endl;
    cout << "heap min cost: "<<Bheap.min().second << endl;
    cout << "heap min pair "<<Bheap.min().first << endl;
    Bheap.popMin();
    cout << "size after popmin: "<< Bheap.size() << endl;
    cout << "heap min cost: "<<Bheap.min().second << endl;
    cout << "heap min pair "<<Bheap.min().first << endl;
    Bheap.popMin();
    cout << "size after popmin: "<< Bheap.size() << endl;
    cout << "heap min cost: "<<Bheap.min().second << endl;
    cout << "heap min pair "<<Bheap.min().first << endl;
    Bheap.popMin();
    cout << "size after popmin: "<< Bheap.size() << endl;
    cout << "heap min cost: "<<Bheap.min().second << endl;
    cout << "heap min pair "<<Bheap.min().first << endl;
    */

    return 0;
}
