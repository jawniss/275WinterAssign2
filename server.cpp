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
  //float scale = 100000;
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
            latFloat = stof(tokenVert);
            // convert to long long
            latitude = static_cast <long long>(latFloat*100000);
            //cout << "latitude: " << latitude << endl;
          }
          if (countVert == 4){
            // convert value to float from string
            lonFloat = stof(tokenVert);
            // convert to long long
            longitude = static_cast <long long>(lonFloat*100000);
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
  long long lat1,lon1,lat2,lon2;
  long long startVertex = 0;
  long long endVertex = 0;
  long long seekingStart = 200000;
  long long seekingEnd = 200000;
  readGraph("edmonton-roads-2.0.1.txt", graph, points);
  //readGraph("test.txt", graph, points);
  cin >> code;
  if (code == "R"){
    cin >> lat1 >> lon1 >> lat2 >> lon2;
    begin.lat = lat1;
    begin.lon = lon1;
    end.lat = lat2;
    end.lon = lon2;
    cout << "begin.lat " << begin.lat<< endl;
    cout << "begin.lon " << begin.lon<< endl;
    cout << "end.lat " << end.lat<< endl;
    cout << "end.lon " << end.lon<< endl;
    //long long threshold = manhattan(begin,end);// calculate the distance between start and end
    for (auto iter: points) { // iterate through all vertices
      long long newStartPoint = manhattan(begin,iter.second);
      long long newEndPoint = manhattan(end,iter.second);
      // find starting and end vertex
      if (newStartPoint <= seekingStart){
        seekingStart = newStartPoint;
        startVertex = iter.first;
      }
      if (newEndPoint <= seekingEnd){
        seekingEnd = newEndPoint;
        endVertex = iter.first;
      }
    }
  }
  cout << "startVertex: " << startVertex << endl;
  cout << "endVertex: " << endVertex << endl;

  unordered_map<int, PLI> searchTree;
  dijkstra(graph, startVertex, searchTree);




  //unordered_map<int,Point> pointSet;
  stack<pair<int,Point>>path;
  pair<int,Point >vertex;


  /*
  if (searchTree.find(endVertex) == searchTree.end()) {
    cout << "Vertex " << endVertex << " not reachable from " << startVertex << endl;
  }

  else {
    vertex.first = endVertex;
    vertex.second.lat = lat2;
    vertex.second.lon = lon2;

    while (vertex.first != startVertex) {

      path.push(vertex);

      // crawl up the search tree one step
      vertex.first = searchTree[vertex.first].first;
    }
    vertex.first = startVertex;
    vertex.second.lat = lat1;
    vertex.second.lon = lon1;
    path.push(vertex);
    cout << "size of stack: " << path.size() << endl;

    // for actaul program
    cout << path.size() << endl;
    while (true) {
      cin >> code;
      if (code == "A"){
        if (!path.empty()){
          vertex = path.top();
          path.pop();
          cout << "W " << vertex.second.lat <<" "<< vertex.second.lon << endl;
        }
        else{
          cout << "N  0" << endl;
        }
      }
    }
  }
  */
  cout << "compiled" << endl;
  return 0;
}
