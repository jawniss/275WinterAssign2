#ifndef _DIJKSTRA_H_
#define _DIJKSTRA_H_

#include <unordered_set>
#include <unordered_map>
#include "wdigraph.h"

typedef long long ll;
typedef pair<ll, int> PLI;


void dijkstra(const WDigraph& graph, int startVertex, unordered_map<int, PLI>& tree);



#endif /* DIJSKTRA_H */