#include <iostream>
#include <unordered_map>
#include <utility>

#include "heap.h"
#include "dijkstra.h"

using namespace std;

// void dijkstra(const WDigraph& graph, int startVertex,unordered_map<int, PLL>& searchTree) {
void dijkstra(const WDigraph& graph, int startVertex, unordered_map<int, PLI>& tree){
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
  unordered_map< int, int> reached;


  BinaryHeap< std::pair<int, int>, int>  events;
  // inserting the starting vertex as a pair
  pair<int, int>startingVertexPair(startVertex,startVertex);
  // insert the pair into events where their initial cost is 0
  events.insert(startingVertexPair,initialCost);
  // while size is not empty
  while (events.size()!= 0) {
    std::pair< std::pair<int, int>, int> start = events.min();
    // for (auto tracker = events.begin(); tracker != events.end(); ++tracker) {
    //   if (tracker->second.second < start->second.second) {
    //     start = tracker;
    //   }
    // }
    events.popMin();
    std::pair<int, int> startEdge = start.first;
    int startKey = start.second;
    int u = startEdge.first;
    int v = startEdge.second;

    if (tree.find(v) != tree.end()) {
      continue;
    }
    else {

      bool insert = true;
      /*
      int cost = events.second;
      int utemp = events.first;
      int u = utemp.first;
      int vtemp = events.first;
      int v = vtemp.second;
      */

      //tree[v] = pair<int, int> (u, d);

      for (auto itr = reached.begin(); itr != reached.end(); ++itr) {
        if (itr->first == v) {
          insert = false;
        }
      }
      // if v not in reached insert is true
      if (insert == true) {
        reached[v] = u;
        std::pair<int,int> treePair;
        treePair.first = startKey;
        treePair.second = u;
        tree[v] = treePair;

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

    /*
    // int v = start->first, u = start->second.first, d = start->second.second;
    // events.erase(start);
    if (tree.find(v) != tree.end()) {
      continue;
    }
    tree[v] = pair<int, int> (u, d);
    */

  }
}

int main() {
  return 0;
}
