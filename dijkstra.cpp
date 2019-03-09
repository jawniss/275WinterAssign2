#include <iostream>
#include <unordered_map>
#include <utility>

#include "heap.h"
#include "wdigraph.h"

using namespace std;

long long manhattan(const Point& pt1, const Point& pt2) {
// Return the Manhattan distance between the two given points
  long long latitude1 = pt1.lat;
  long long latitude2 = pt2.lat;
  long long longitude1 = pt1.lon;
  long long longitude2 = pt2.lon;

  distance = abs(latitude1  - latitude2) + abs(longitude1 - longitude2);
  return distance;
}

// void dijkstra(const WDigraph& graph, long long startVertex,unordered_map<long long, PLL>& searchTree) {
void dijkstra(const WDigraph& graph, long long startVertex, unordered_map<int, pair<int, long long>& tree) {
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
  unordered_map< long long, long long> reached;
  std::vector< pair< std::pair<long long, long long>, long long> > events;
  events.insert(startVertex);
  while (!events.empty()) {
    std::pair< std::pair<long long, long long>, long long> start = events.popmin();
    // for (auto tracker = events.begin(); tracker != events.end(); ++tracker) {
    //   if (tracker->second.second < start->second.second) {
    //     start = tracker;
    //   }
    // }

    bool insert = true;
    long long cost = events.second;
    long long utemp = events.first;
    long long u = utemp.first;
    long long vtemp = events.first;
    long long v = vtemp.second;
    for (auto itr = reached.begin(); itr != reached.end(); ++itr) {
      if (itr->first == v) {
        insert = false;
      }
    }
    if (insert == true) {
      reached[v] = u;
      for (auto iter = graph.neighbours(v); iter != graph.endIterator(v); iter++) {
        long long node = *iter;
        long long cost = cost + graph.manhattan(v, node);
        // events.insert(pair <long long, pair<long long, long long> >(node, pair <long long, long long> (v, cost)));
        //pair < pair<long long, long long>(u, v), long long> (cost)
        pair<long long, long long> uv;
        uv.first = u;
        uv.second = v;
        events.insert(uv, cost);
      }
    }

    // long long v = start->first, u = start->second.first, d = start->second.second;
    // events.erase(start);
    if (tree.find(v) != tree.end()) {
      continue;
    }
    tree[v] = pair<long long, long long> (u, d);

  }
}

int main() {
  return 0;
}
