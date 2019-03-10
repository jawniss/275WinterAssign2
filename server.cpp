#include "wdigraph.h"
#include "dijkstra.h"
#include "digraph.h" // do we need?

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

// from hash example
#include <iomanip>
#include <cassert>

using namespace std;

struct Point {
  long long lat; // latitude of the point
  long long lon; // longitude of the point

  /*
  // constructor that takes in latitude and longitude
	Point(long long latitude,long long longitude):
		lat(latitude), lon(longitude)
	{
	}
  // two records with the same longitude and latitude
  bool operator==(const Point& lt,const Point& ln) const {
    return ((lat == lt.lat) && (lon == ln.lon));
  }
  bool operator!=(const Point& lt,const Point& ln) const {
    return ((lat == lt.lat) && (lon == ln.lon));
  }
  */
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

/*
// hash function to overide operator() while calculating the hash value
namespace std{
	template <>
	struct hash<StudentRecord>{
	  std::size_t operator()(const StudentRecord& record) const{
	    using std::size_t;
	    using std::hash;
	    return (hash<int>()(record.mId));
	  }
	};
}
*/

// reading the city graph function that takes in a string text file
void readGraph(string filename, WDigraph& graph, unordered_map<int, Point>& points){
  Point coordinates;
  Point point1;
  Point point2;
  string line;
  float latFloat;
  long long latitude;
  float lonFloat;
  long long longitude;
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
            latFloat = stof(tokenVert)*100000;
            // convert to long long
            latitude = static_cast <long long>(latFloat);
          }
          if (countVert == 4){
            // convert value to float from string
            lonFloat = stof(tokenVert)*100000;
            // convert to long long
            longitude = static_cast <long long>(lonFloat);
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
  WDigraph graph;
  unordered_map<int, Point> points;
  readGraph("edmonton-roads-2.0.1.txt", graph, points);


    cout << "compiled" << endl;

return 0;
}
