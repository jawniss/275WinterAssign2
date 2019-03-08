#include <iostream>
#include <unordered_map>
#include <utility>

void dijkstra(const WDigraph& graph, long long startVertex,unordered_map<long long, PLL>& searchTree) {
/*

reached ← empty hash table (i.e., unordered map)
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

unordered_map< pair <long long, long long>, long long> reached;
std::vector< std::pair<T, K> > events;
events.insert(startVertex);
while (!events.empty()) {
  auto start = events.begin();
  for (auto tracker = events.begin(); tracker != events.end(); ++tracker) {
    if (tracker->second.second < start->second.second) {
      start = tracker;
    }
  }




  long long u = events.first.first; v = events.first.second; cost = events.second;
  for (auto itr = reached.begin(); itr != reached.end(); ++itr) {
    if (itr->first.second == v) {
      bool insert = false;
    }
  }
  if (insert == true) {
    reached[v] = u;
  }



  long long v = start->first, u = start->second.first, d = start->second.second;
  events.erase(start);
  if (searchTree.find(v) != searchTree.end()) {
    continue;
  }
  searchTree[v] = pair< long long, long long> (u, d);
  for (auto iter = graph.neighbours(v); iter != graph.endIterator(v); iter++) {
    long long node = *iter;
    long long cost = d + graph.getCost(v, node);
    events.push_back(pair <long long, pair<long long, long long> >(node, pair <long long, long long> (v, cost)));
  }
}
}
