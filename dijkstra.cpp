#include <iostream>
#include <unordered_map>
#include <utility>

#include "heap.h"
#include "dijkstra.h"

using namespace std;

// void dijkstra(const WDigraph& graph, int startVertex,unordered_map<int, PLL>& searchTree) {
void dijkstra(const WDigraph& graph, int startVertex, unordered_map<int, std::pair<long long, int> >& tree){
/*
reached ← empty hash table (i.e., unordered map) X
events ← empty heap
events.insert((s,s),0)
while len(events) > 0 do
   (u, v), time ← events.popmin()
   if v not in reached then
    reached[v] ← u
    for each neighbour w of v do       # burn vertex v, record predecessor u
      #new event: edge (v,w) started burning
      events .insert ((v , w ), time + cost (v , w ))
return reached
*/

// unordered_map< pair <int, int>, int> reached;
  // initial cost is 0 since on same vertex
  int initialCost = 0;
  unordered_map< int, int> reached = {};
  reached.insert({1,1});
  cout << reached.size() << endl;

  BinaryHeap< std::pair<int, int>, int> events;

  // inserting the starting vertex as a pair
  std::pair<int, int>startingVertexPair(startVertex,startVertex);
  // insert the pair into events where their initial cost is 0
  events.insert(startingVertexPair,initialCost);
  reached.insert({2,2});
  cout << reached.size() << endl;
  // while size is not empty
  cout << "before while loop "<< endl;
  while (events.size()!= 0) {
    std::pair< std::pair<int, int>, int> start = events.min();
    cout << start.first.first << " " << start.first.second << " " << start.second << endl;
    // for (auto tracker = events.begin(); tracker != events.end(); ++tracker) {
    //   if (tracker->second.second < start->second.second) {
    //     start = tracker;
    //   }
    // }
    reached.insert({4,4});
    cout << reached.size() << endl;
    events.popMin();
    std::pair<int, int> startEdge = start.first;
    int startKey = start.second;
    int u = startEdge.first;
    int v = startEdge.second;
    cout << "U and v: " << endl;
    cout << u << ' ' << v << endl;
    cout << "inside while loop "<< endl;
    reached.insert({3,3});
    cout << reached.size() << endl;
    if (tree.find(v) != tree.end()) {

      continue;
    }
    else {
      cout << "tree found "<< endl;
      bool insert = true;
      // reached.insert({6,6});
      cout << reached.size() << endl;
      cout << "seg fault" << endl;
      /*
      int cost = events.second;
      int utemp = events.first;
      int u = utemp.first;
      int vtemp = events.first;
      int v = vtemp.second;
      */

      //tree[v] = pair<int, int> (u, d);

      cout << "one" << endl;
      // for (auto itr = reached.begin(); itr != reached.end(); ++itr) {
      //   cout << "for loop" << endl;
      //   if (itr->first == v) {
      //     cout << "faulso" << endl;
      //     insert = false;
      //     break;
      //   }
      // }
      cout << "two" << endl;
      cout << "finished iterating" << endl;
      // if v not in reached insert is true
      if (insert == true) {
        cout << "test" << endl;
    //     using namespace std::this_thread; // sleep_for, sleep_until
    // using namespace std::chrono; // nanoseconds, system_clock, seconds
    //     sleep_for(seconds(5));
    //     cout << "go into insert" << endl;
    cout << reached.size() << endl;
        reached.insert({v,u});
        // cout << "returrning" << endl;
        // return;
        cout << "hai" << endl;
        std::pair<int,int> treePair;
        cout << "past insert" << endl;
        treePair.first = startKey;
        treePair.second = u;
        cout << treePair.first << " " << treePair.second << endl;
        // tree[v] = treePair;
        cout << "after tree" << endl;

        // create one for each of its neighbors
        for (auto iter = graph.neighbours(v); iter != graph.endIterator(v); iter++) {
          int node = *iter;
          int cost = graph.getCost(v,node);
          std::pair<long long,int> insertPair;
          insertPair.first = (long long) v; // typecast v into long long
          insertPair.second = node;
          int newCost = startKey + cost;
          events.insert(insertPair, newCost);
        }
      }
    }
  }
}
