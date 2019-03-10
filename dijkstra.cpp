#include <iostream>
#include <unordered_map>
#include <utility>

#include "heap.h"
#include "dijkstra.h"

using namespace std;

// void dijkstra(const WDigraph& graph, long long startVertex,unordered_map<long long, PLL>& searchTree) {
void dijkstra(const WDigraph& graph, long long startVertex,  unordered_map<int, PLI>& tree){
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

// unordered_map< pair <long long, long long>, long long> reached;
  // initial cost is 0 since on same vertex
  long long initialCost = 0;
  unordered_map< long long, long long> reached;


  BinaryHeap< std::pair<long long, long long>, long long>  events;
  // inserting the starting vertex as a pair
  pair<long long, long long>startingVertexPair(startVertex,startVertex);
  // insert the pair into events where their initial cost is 0
  events.insert(startingVertexPair,initialCost);
  // while size is not empty
  while (events.size()!= 0) {
    std::pair< std::pair<long long, long long>, long long> start = events.popMin();
    // for (auto tracker = events.begin(); tracker != events.end(); ++tracker) {
    //   if (tracker->second.second < start->second.second) {
    //     start = tracker;
    //   }
    // }
    std::pair<long long, long long> startEdge = start.first;
    long long startKey = start.second;
    long long u = startEdge.first;
    long long v = startEdge.second;

    if (tree.find(v) != tree.end()) {
      continue;
    }
    else {

      bool insert = true;
      /*
      long long cost = events.second;
      long long utemp = events.first;
      long long u = utemp.first;
      long long vtemp = events.first;
      long long v = vtemp.second;
      */

      //tree[v] = pair<long long, long long> (u, d);

      for (auto itr = reached.begin(); itr != reached.end(); ++itr) {
        if (itr->first == v) {
          insert = false;
        }
      }
      // if v not in reached insert is true
      if (insert == true) {
        reached[v] = u;
        std::pair<long long,long long> treePair;
        treePair.first = startKey;
        treePair.second = u;
        tree[v] = treePair;

        // create one for each of its neighbors
        for (auto iter = graph.neighbours(v); iter != graph.endIterator(v); iter++) {
          long long node = *iter;
          long long cost = graph.getCost(v,node);
          std::pair<long long,long long> insertPair;
          insertPair.first = v;
          insertPair.second = node;
          long long newCost = startKey + cost;
          events.insert(insertPair, newCost);
        }
      }
    }

    /*
    // long long v = start->first, u = start->second.first, d = start->second.second;
    // events.erase(start);
    if (tree.find(v) != tree.end()) {
      continue;
    }
    tree[v] = pair<long long, long long> (u, d);
    */

  }
}
/*
int main() {
  return 0;
}
*/
