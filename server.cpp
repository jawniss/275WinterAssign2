#include <string>
#include <iostream>
#include <fstream>
#include "dijkstra.h"
#include "digraph.h"
#include "wdigraph.h"
#include <stack>

using namespace std;

struct Point {
    long long lat;
    long long lon;
};

long long manhattan(const Point& pt1, const Point& pt2) {
    // calculate man dist
    long long dist = abs(pt1.lat - pt2.lat) + abs(pt1.lon - pt2.lon);
    return dist;
}


void readGraph(string filename, WDigraph& graph, unordered_map<int, Point>& points) {
    /* method to open and read from files found from:
    cplusplus.com/doc/tutorial/files/             */
    ifstream file;  // file object
    string token;
    char delim = ',';  // the delimeter of choice
    int ID;
    long double coord;
    Point p, p1, p2;
    file.open(filename);  // opening the file
    if (file.is_open()) {  // if the file is open...
        while (getline(file, token, delim)) {  // while there is stilong long a token
                                               // to get...
            if (token == "V") {
                getline(file, token, delim);
                ID = stoi(token);
                getline(file, token, delim);
                coord = stold(token) * 100000;
                coord = static_cast<long long>(coord);
                p.lat = coord;  // saving the coordinate to the points
                getline(file, token);
                coord = stold(token) * 100000;
                coord = static_cast<long long>(coord);
                p.lon = coord;  // saving the coordinate to the points
                points[ID] = p;  // insert into the map
            } else if (token == "E") {
                getline(file, token, delim);
                int id1 = stoi(token);
                getline(file, token, delim);
                int id2 = stoi(token);
                getline(file, token);
                string name = token;
                p1 = points[id1];  // get respective point
                p2 = points[id2];  // get respective point
                // calc man dist, pass in saved points
                long long weight = manhattan(p1, p2);  // calculate distance
                graph.addEdge(id1, id2, weight);  // add the weighted edge
            }
        }
    } else {
        /*Error message in case the file is not found. */
        cout << "ERROR. Unable to open the file " << filename << "." << endl;
    }
    file.close();  // closing the file
}


int closestVert(const long long& lat, const long long& lon, unordered_map<int, Point>& p) {
    Point point;
    long long dist, temp;
    point.lat = lat;  // assign lat parameter to struct
    point.lon = lon;  // assign lon parameter to struct
    int vert = p.begin()->first;  // set starting vertex to the first one
    dist = manhattan(p[vert], point);  // initialize the distance
    for (auto i: p) {
        temp = manhattan(i.second, point);  // calculate temp distance
        if (temp < dist) {  // if that distance is less than previous...
            vert = i.first;  // set the vertex to the current iteration
            dist = temp;  // set the distance to the new lower value
        }
    }
    return vert;  // return resulting vertex
}


void printWaypoints(unordered_map<int, Point>& p, unordered_map<int, PLI>& tree,
                    int& startVert, int& endVert) {
    stack<int> route;
    int vert = endVert;  // start at the end point
    char ack;

    //cout << p[vert].lon << endl;
    while (route.top() != startVert) {  // while we have not reached the start
        route.push(vert);  // push the vertex onto the stack
        vert = tree[vert].second;  // set the vertex to the parent of current
    }
    cout << "N " << route.size() << endl;  // print out number of waypoints
    int size = route.size();
    for (int i = 0; i < size; i++) {
        cin >> ack;  // receive acknowledgement
        if (ack == 'A') {
            /*print out the waypoint coordinates*/
            cout << "W " << p[route.top()].lat << " ";
            cout << p[route.top()].lon << endl;
            route.pop();  // removing the element from the stack
        }
    }
    cin >> ack;
    cout << "E" << endl;  // indicating end of request
}


int main() {
    WDigraph graph;
    unordered_map<int, Point> points;
    readGraph("edmonton-roads-2.0.1.txt", graph, points);
    char r, ack;
    cin >> r;  // read in R character
    int startLat, startLon, endLat, endLon;
    cin >> startLat >> startLon >> endLat >> endLon;  // read in the coordinates
    int start = closestVert(startLat, startLon, points);  // map to vertex
    int end = closestVert(endLat, endLon, points);  // map to vertex
    unordered_map<int, PLI> heapTree;
    dijkstra(graph, start, heapTree);
    if (heapTree.find(end) == heapTree.end()) {
        cout << "N 0" << endl;
        cin >> ack;
        cout << "E" << endl;
    } else {
        printWaypoints(points, heapTree, start, end);
    }
    return 0;
}
