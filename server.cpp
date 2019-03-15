#include "wdigraph.h"
#include "dijkstra.h"
#include "digraph.h"


#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>
#include <utility> // for pair
#include <stack> // std :: stack lives here


#include "serialport.h"


using namespace std;

// struct given to us that stores latitude and longitude
struct Point {
  long long lat; // latitude of the point
  long long lon; // longitude of the point

};

// Return the Manhattan distance between the two given points
long long manhattan(const Point& pt1, const Point& pt2) {
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
            // convert value to long double from string
            latDouble = stold(tokenVert);
            // convert to long long
            latitude = static_cast <long long>(latDouble*100000);
            //cout << "latitude: " << latitude << endl;
          }
          if (countVert == 4){
            // convert value to long double from string
            lonDouble = stold(tokenVert);
            // convert to long long
            longitude = static_cast <long long>(lonDouble*100000);
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
        // find distance between them
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

// main function that takes the job of communicating with stdin and stdout
int main() {
  SerialPort Serial("/dev/ttyACM0");

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
  //bool readR == false;
  readGraph("edmonton-roads-2.0.1.txt", graph, points);


  string input;



  // while loop to read in inputs
  while (true){
    // case to wait until we read a R
    /*
    do {
      input = Serial.readline();
    } while (); // not sure if while loop needed
    */
    int word = 0;

    input = Serial.readline();
    cout << input << endl;

    if (input.find("R")){
      istringstream ss(input);
      string token;
      while (getline(ss,token,' ')){
        cout << "inside get line" << endl;
        word++;
        if (word == 2){
          // convert it from string to integer
          lat1 = stoi(token,nullptr,10);
          cout << "lat1" << lat1 << endl;
        }
        if (word == 3){
          lon1 = stoi(token,nullptr,10);
          cout << "lon1" << lon1 << endl;
        }
        if (word == 4 ){
          lat2 = stoi(token,nullptr,10);
          cout << "lat2" << lat2 << endl;
        }
        if (word == 5) {
          lon2 = stoi(token,nullptr,10);
          cout << "lon2" << lon2 << endl;
        }
        if (word > 5){
          break;
        }
      }
    }
    // generate a random starting point for both start and end point
    seekingStart = manhattan(begin,points[0]);
    seekingEnd = manhattan(end,points[0]);
    // iterate through all vertices
    for (auto iter: points) {
      // check every point iterating through
      long long newStartPoint = manhattan(begin,iter.second);
      long long newEndPoint = manhattan(end,iter.second);
      // if cases new point found is closer than replace the previous point
      if (newStartPoint < seekingStart){
        startVertex = iter.first;
        seekingStart = newStartPoint;
      }
      if (newEndPoint < seekingEnd){
        endVertex = iter.first;
        seekingEnd = newEndPoint;
      }
    }
    cout << "compiled" << endl;
    cout << seekingStart << endl;
    cout << seekingEnd << endl;





    /*
    while (readR == false){
      cin >> code;
      // case if we read "R" as first letter
      if (code == "R"){
        cin >> lat1 >> lon1 >> lat2 >> lon2;
        begin.lat = lat1;
        begin.lon = lon1;
        end.lat = lat2;
        end.lon = lon2;
        // generate a random starting point for both start and end point
        seekingStart = manhattan(begin,points[0]);
        seekingEnd = manhattan(end,points[0]);
        // iterate through all vertices
        for (auto iter: points) {
          // check every point iterating through
          long long newStartPoint = manhattan(begin,iter.second);
          long long newEndPoint = manhattan(end,iter.second);
          // if cases new point found is closer than replace the previous point
          if (newStartPoint < seekingStart){
            startVertex = iter.first;
            seekingStart = newStartPoint;
          }
          if (newEndPoint < seekingEnd){
            endVertex = iter.first;
            seekingEnd = newEndPoint;
          }
        }
        readR = true;
      }
    }
*/
  /*
    unordered_map<int, PLI> searchTree;
    // call dijkstra function
    dijkstra(graph, startVertex, searchTree);
    // initialize a stack
    stack<int>path;
    finalPoint = endVertex;
    // while loop that pushes the path until we reach the end vertex
    while (path.top() != startVertex) {
      path.push(finalPoint);
      // crawl up the searchtree one step at a time
      finalPoint = searchTree[finalPoint].second;
    }
    // output the length
    cout << "N " << path.size() << endl;
    length = path.size();
    // start a while loop that will loop for the size of the stack
    while (counter != length){
      counter++;
      // read in a letter
      cin >> code;
      // if case for if we read a "A"
      if (code == "A"){
        // output the latitude and longitude of the point we are taking off the stack
        cout << "W " << points[path.top()].lat << " " << points[path.top()].lon << endl;
        // pop what we outputed
        path.pop();
      }
    }
    // last input that we will take in
    cin >> code;
    // output E since we are done
    cout << "E" << endl;
    break;
    */

  }

  return 0;
}
