#include "dijkstra.h"
#include "heap.h"
#include <iostream>

using namespace std;

bool insert = true;

void dijkstra(const WDigraph& graph, int startVertex, unordered_map<int, PLI>& reached) {
  pair<long long, long long> startingvertex(startVertex, startVertex);
  BinaryHeap<pair<long long, long long>, long long> events;
  int startingcost = 0;
  events.insert(startingvertex, startingcost);
  reached = {};
  while (events.size() != 0) {
    pair< pair<long long, long long>, long long> uvtime;
    uvtime  = events.min();
    events.popMin();
    long long verticescost = uvtime.second;
    pair<long long, long long> twovertices = uvtime.first;
    long long vertex1 = twovertices.first;
    long long vertex2 = twovertices.second;
    insert = false;
    if (reached.find(vertex2) == reached.end())
      insert = true;
    if (insert == true) {
      pair<long long, long long> putinreached;
      putinreached.first = verticescost;
      putinreached.second = vertex1;
      reached[vertex2] = putinreached;
      for (auto itr = graph.neighbours(vertex2); itr != graph.endIterator(vertex2); ++itr) {
        pair<long long, long long> putinevents;
        putinevents.first = vertex2;
        putinevents.second = *itr;
        long long mostrecentcost = graph.getCost(vertex2, *itr);
        events.insert(putinevents, verticescost + mostrecentcost);
      }
    }
  }
}
