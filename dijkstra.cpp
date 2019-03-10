#include "dijkstra.h"
#include "heap.h"

void dijkstra(const WDigraph& graph, int startVertex, unordered_map<int, PLI>& tree) {
    // empty events heap
    BinaryHeap<pair<int, int> ,int> events;
    // insert first element of graph into heap
    pair<int, int> startPair(startVertex, startVertex);
    events.insert(startPair, 0);

    while (events.size() > 0) {
        // popping the min item
        pair<pair<int, int>, int> temp = events.min();
        events.popMin();
        pair<int, int> edge = temp.first;
        int key = temp.second;
        int u = edge.first;
        int v = edge.second;

        if (tree.find(v) == tree.end()) {
            pair<ll, int> interPair((ll)key, u);
            tree[v] = interPair;
            for (unordered_set<int>::const_iterator iter = graph.neighbours(v);
                iter != graph.endIterator(v); iter++) {
                pair<int, int> tempPair(v, *iter);
                ll cost = graph.getCost(v, *iter);
                events.insert(tempPair, key + (int)cost);
            }
        }
    }
}