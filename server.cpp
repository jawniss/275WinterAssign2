#include "wdigraph.h"
#include "dijkstra.h"
#include "digraph.h"

#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <list>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>
#include <utility> // for pair
# include <stack> // std :: stack lives here

using namespace std;

struct Point {
  long long lat; // latitude of the point
  long long lon; // longitude of the point

};

long long manhattan(const Point& pt1, const Point& pt2) {
// Return the Manhattan distance between the two given points
  long long latitude1 = pt1.lat;
  long long latitude2 = pt2.lat;
  long long longitude1 = pt1.lon;
  long long longitude2 = pt2.lon;

  long long distance = abs(latitude1  - latitude2) + abs(longitude1 - longitude2);
  return distance;
}


// reading the city graph function that takes in a string text file
void readGraph(string filename, WDigraph& graph, unordered_map<int, Point>& points){
  Point coordinates;
  Point point1;
  Point point2;
  string line;
  long double latDouble;
  long double latitude = 0;
  long double lonDouble;
  //float scale = 100000;
  long double longitude = 0;
  ifstream infile (filename);
  // open the text file
  if (infile.is_open()){
    // while loop that reads each line of the text file
    while (getline(infile,line)){
      // If in the line we find "V," we know its a Vertex
      if(line.find("V,") != (string::npos)){
        int vertex = 0;
        istringstream ss(line);
        string tokenVert;
        int countVert = 0;
        while (getline(ss,tokenVert,',')){
          countVert++;
          // take value after first comma
          if (countVert == 2){
            // convert value to integer from string
            vertex = stoi(tokenVert,nullptr,10);
          }
          if (countVert == 3){
            // convert value to float from string
            latDouble = stold(tokenVert);
            // convert to long long
            latitude = static_cast <long long>(latDouble*100000);
            //cout << "latitude: " << latitude << endl;
          }
          if (countVert == 4){
            // convert value to float from string
            lonDouble = stold(tokenVert);
            // convert to long long
            longitude = static_cast <long long>(lonDouble*100000);
            //cout << "llongie: " << longitude << endl;
          }
          // don't iterate the rest since we only need the vertex, longitude and latitude
          if (countVert > 4){
            break;
          }
        }

        coordinates.lat = latitude;
        coordinates.lon = longitude;
        // to insert into a unordered map
        pair<int,Point>vertCord(vertex,coordinates);
        points.insert(vertCord);
      }
      // If in the line we find "E," we know its an Edge
      else if(line.find("E,") != (string::npos)){
        int u = 0;
        int v = 0;
        istringstream ss(line);
        string tokenEdge;
        int countEdge = 0;
        // obtain the u and v edges
        while (getline(ss,tokenEdge,',')){
          countEdge++;
          // take value after first comma
          if (countEdge == 2){
            // convert it from string to integer
            u = stoi(tokenEdge,nullptr,10);
          }
          // take value after second comma
          if (countEdge == 3){
            // convirt it from string to integer
            v = stoi(tokenEdge,nullptr,10);
          }
          // don't bother iterating the rest since we only need edges
          if (countEdge > 3){
            break;
          }
        }
        // let it be struct
        point1 = points[u];
        point2 = points[v];
        long long dist = manhattan(point1,point2);
        // directed graph so we need the given way
        graph.addEdge(u,v,dist);
      }
    }
    // close the file
    infile.close();
  }
  // case to exit if the file doesn't open
  else{
    cout << "Error opening file"<< endl;
    exit (1);
  }
}


int main() {
  // get start vertex first find it by looking through all and seeing which is closest (in case cursor is in buildings or something)
  // iterate through all vertices and and get the vertex ID with the smallest Manhattan dist
  // make the sorting tree using the vertex returned
  WDigraph graph;
  unordered_map<int,Point> points;
  Point begin;
  Point end;
  string code;
  int finalPoint;
  int length;
  int counter = 0;
  long long lat1,lon1,lat2,lon2;
  long long startVertex = 0;
  long long endVertex = 0;
  long long seekingStart;
  long long seekingEnd;
  readGraph("edmonton-roads-2.0.1.txt", graph, points);
  //cout << " we done reading graph" << endl;
  //readGraph("test.txt", graph, points);
  cin >> code;
  if (code == "R"){
    cin >> lat1 >> lon1 >> lat2 >> lon2;
    begin.lat = lat1;
    begin.lon = lon1;
    end.lat = lat2;
    end.lon = lon2;
    seekingStart = manhattan(begin,points[0]);
    seekingEnd = manhattan(end,points[0]);
    //long long threshold = manhattan(begin,end);// calculate the distance between start and end
    for (auto iter: points) { // iterate through all vertices
      long long newStartPoint = manhattan(begin,iter.second);
      long long newEndPoint = manhattan(end,iter.second);
      // find starting and end vertex
      if (newStartPoint < seekingStart){
        startVertex = iter.first;
        seekingStart = newStartPoint;
      }
      if (newEndPoint < seekingEnd){
        endVertex = iter.first;
        seekingEnd = newEndPoint;
      }
    }
  }

  unordered_map<int, PLI> searchTree;
  dijkstra(graph, startVertex, searchTree);
  //cout <<"passed dike" << endl;
  //unordered_map<int,Point> pointSet;
  stack<int>path;
  finalPoint = endVertex;
  while (path.top() != startVertex) {

    path.push(finalPoint);

    // crawl up the search tree one step
    finalPoint = searchTree[finalPoint].second;
  }
  cout << "N " << path.size() << endl;

  length = path.size();
  while (counter != length){
    counter++;
    cin >> code;
    if (code == "A"){
      cout << "W " << points[path.top()].lat << " " << points[path.top()].lon << endl;
      path.pop();
    }
  }
  cin >> code;
  cout << "E" << endl;

  //cout << "compiel " << endl;
  return 0;
}
