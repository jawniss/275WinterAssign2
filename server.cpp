#include <string>
#include <iostream>
#include <fstream>
#include "dijkstra.h"
#include "digraph.h"
#include "wdigraph.h"

struct Point {
    ll lat;
    ll lon;
};

ll manhattan(const Point& pt1, const Point& pt2) {
    // calculate man dist
    ll dist = abs(pt1.lat - pt2.lat) + abs(pt1.lon - pt2.lon);
    return dist;
}


void readGraph(string filename, WDigraph& graph, unordered_map<int, Point>& points) {
    /* method to open and read from files found from:
    cplusplus.com/doc/tutorial/files/             */
    ifstream file;  // file object
    string token;
    char delim = ',';  // the delimeter of choice
    int ID;
    float coord;
    Point p, p1, p2;
    file.open(filename);  // opening the file
    if (file.is_open()) {  // if the file is open...
        while (getline(file, token, delim)) {  // while there is still a token
                                               // to get...
            if (token == "V") {
                // add a vertex
                getline(file, token, delim);
                ID = stoi(token);
                getline(file, token, delim);  // check order
                coord = stof(token);
                coord *= 100000;
                p.lat = static_cast<ll>(coord);
                getline(file, token);  // not needed for the exercise
                coord = stof(token);
                coord *= 100000;
                p.lon = static_cast<ll>(coord);
                // put point in the map
                points[ID] = p;
                //graph.addVertex(ID);
            } else if (token == "E") {
                // add an edge
                getline(file, token, delim);
                int id1 = stoi(token);
                getline(file, token, delim);
                int id2 = stoi(token);
                getline(file, token);
                string name = token;
                // search the map for given id's
                p1 = points[id1];
                p2 = points[id2];
                // calc man dist, pass in saved points
                ll weight = manhattan(p1, p2);
                graph.addEdge(id1, id2, weight);
            }
        }
    } else {
        /*Error message in case the file is not found. */
        cout << "ERROR. Unable to open the file " << filename << "." << endl;
    }
    file.close();  // closing the file
}


int getInput(unordered_map<int, Point> points) {
    string letter;
    ll startLon, startLat;//, endLon, endLat;
    cin >> letter;

    if (letter == "R") {
        cin >> startLat >> startLon; //>> endLat >> endLon;
        Point start;//, end;
        start.lat = startLat;
        start.lon = startLon;
        //end.lat = endLat;
        //end.lon = endLon;
        //Point startPoint = points[30198550];
        //ll startWeight = manhattan(start, startPoint);
        ll startWeight = 1000000;
        ll lat = 0;
        ll lon = 0;
        for (auto p: points) {
            ll newWeight = manhattan(start, p.second);
            if (newWeight < startWeight) {
                startWeight = newWeight;
                lat = p.second.lat;
                lon = p.second.lon;
                //auto startPoint = p;
            }
        }
        for (auto i: points) {
            if (i.second.lat == lat && i.second.lon == lon) {
                return i.first;
            }
        }
    } else if (letter == "A") {
        // continue
        return -1;
    }


}


int main() {
    WDigraph graph;
    unordered_map<int, Point> points;
    readGraph("edmonton-roads-2.0.1.txt", graph, points);
    // find the start and ending point IDs
    // pass start vertex into dijkstra
    // find the ending point in the search tree???
    // track the path down the tree

    // int -> current vertex
    // PLI -> ll: weight of next vertex, int: next vertex
    int startVertex = getInput(points);
    unordered_map<int, PLI> heapTree;
    dijkstra(graph, startVertex , heapTree);
    for (auto i: heapTree) {
        cout << i.second.second << endl;
    }
    //bool processed = false;
    return 0;
}