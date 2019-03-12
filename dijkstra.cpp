#include "dijkstra.h"
#include "heap.h"
#include <iostream>

using namespace std;

// Global variable for whether or not to insert a new vertice from the heap
bool insert = true;


// Dijkstra algorithm by following the pseudocode on the eClass slides
void dijkstra(const WDigraph& graph, int startVertex, unordered_map<int, PLI>& reached) {
  // Construct the '(s, s)' initial vertex pair that is to be inserted to the
  // events heap
  pair<long long, long long> startingvertex(startVertex, startVertex);
  // Initialize the events heap
  BinaryHeap<pair<long long, long long>, long long> events;
  int startingcost = 0;
  // Insert the initial vertex pair and the cost of 0
  events.insert(startingvertex, startingcost);
  // Initialize the 'vertexes reached' map as empty since so far no vertexes
  // were traversed
  reached = {};
  while (events.size() != 0) {
    // Temporary object of the minimum cost object in the heap
    pair< pair<long long, long long>, long long> uvtime;
    uvtime = events.min();
    events.popMin();
    // The second characteristic of the object is the cost, the first is the
    // vertices pair. Break up the pair into first and second vertexes
    long long verticescost = uvtime.second;
    pair<long long, long long> twovertices = uvtime.first;
    long long vertex1 = twovertices.first;
    long long vertex2 = twovertices.second;
    // Do not insert by default
    insert = false;
    // If the vertex is not found in the 'reached' map, then it should be
    // inserted
    if (reached.find(vertex2) == reached.end())
      insert = true;
    if (insert == true) {
      // Another temporary object of the vertices pair
      pair<long long, long long> putinreached;
      putinreached.first = verticescost;
      putinreached.second = vertex1;
      // Now that the vertex has been traversed, put it in the 'reached' map
      reached[vertex2] = putinreached;
      // Using the eClass provided digraph code, add the neighbours of the
      // current vertex
      for (auto itr = graph.neighbours(vertex2); itr != graph.endIterator(vertex2); ++itr) {
        // Temporary vertices pair
        pair<long long, long long> putinevents;
        putinevents.first = vertex2;
        putinevents.second = *itr;
        long long mostrecentcost = graph.getCost(vertex2, *itr);
        // Insert the new object with the new cost into events to be iterated
        // through on the next loop run
        events.insert(putinevents, verticescost + mostrecentcost);
      }
    }
  }
}
