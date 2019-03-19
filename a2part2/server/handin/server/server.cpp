#include "wdigraph.h"
#include "dijkstra.h"
#include "digraph.h"
#include "serialport.h"

#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>
#include <utility> // for pair
#include <stack> // std :: stack lives here
#include <cstdlib>
#include <cassert>

using namespace std;

// struct given to us that stores latitude and longitude
struct Point {
  long long lat; // latitude of the point
  long long lon; // longitude of the point

};

// Return the Manhattan distance between the two given points from part 1 solutions given to us
long long manhattan(const Point& pt1, const Point& pt2) {
  long long latitude1 = pt1.lat;
  long long latitude2 = pt2.lat;
  long long longitude1 = pt1.lon;
  long long longitude2 = pt2.lon;
  long long distance = abs(latitude1  - latitude2) + abs(longitude1 - longitude2);
  return distance;
}

// finds the id of the point that is closest to the given point "pt" taken from part 1 solutions
int findClosest(const Point& pt, const unordered_map<int, Point>& points) {
  pair<int, Point> best = *points.begin();

  // just scan each vertex, linear time is acceptable
  for (const auto& check : points) {
    if (manhattan(pt, check.second) < manhattan(pt, best.second)) {
      best = check;
    }
  }
  return best.first;
}


// reading the city graph function that takes in a string text file from part 1 solutions given to us
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
  string inputcoord;
  string inputAck;
  string inputEnd;
  string ln,lt;
  string p[5];
  bool timeout = false;
  bool pathisdone = false;
  readGraph("edmonton-roads-2.0.1.txt", graph, points);

  // while loop to read in inputs
  while (true){
    // case to wait until we read a R
    cout << "start server " << endl;
    timeout = false;
    pathisdone = false;
    while ((timeout == false) && (pathisdone == false)){
      int counter = 0;
      do {
        // timeout for readline increased to 2 seconds since it seemed more consistent
      inputcoord = Serial.readline(2000);
    } while (inputcoord.find("R") == (string::npos));
        // split string taken from readgraph above
        int at = 0;
        for (auto c: inputcoord){
          if (c == ' '){
            at++;
          }
          else {
            p[at] += c;
          }
          assert(at < 5);
        }
        // case for if an R is read
        if (p[0] == "R"){
          // set the broken up string and take the values from string to long longs
          begin.lat = stoll(p[1]);
          begin.lon = stoll(p[2]);
          end.lat = stoll(p[3]);
          end.lon = stoll(p[4]);
        }
        // find closest vertex for both starting point and end point
        int start = findClosest(begin,points);
        int last = findClosest(end,points);

        unordered_map<int, PLI> searchTree;
        // call dijkstra function
        dijkstra(graph, start, searchTree);
        // initialize a stack
        stack<int>path;
        finalPoint = last;
        // while loop that pushes the path until we reach the end vertex
        while (path.top() != start) {
          path.push(finalPoint);
          // crawl up the searchtree one step at a time
          finalPoint = searchTree[finalPoint].second;
        }
        length = path.size();
        string lengthstr = to_string(length);
        // case for if the number of vertices is 0 and if vertices is greater than 500
        if ((length > 500) || (length == 0)){
          assert(Serial.writeline("N 0\\n"));
        }
        // output the number of vertices for the client
        else {
          cout << "N " << path.size() << endl;
          assert(Serial.writeline("N "));
          assert(Serial.writeline(lengthstr));
          assert(Serial.writeline("\n"));

          // start a while loop that will loop for the size of the stack
          while (counter != length){
            do {
            inputAck = Serial.readline(1000);
            // timeout if it takes too long to recieve an acknowledgement
            if (inputAck == ""){
              timeout = true;
              break;
            }
          } while (inputAck.find("A") == (string::npos));

            // if an A was found
            if (inputAck.find("A") != (string::npos) ){
              // output the latitude and longitude of the point we are taking off the stack for client to store
              lt = to_string( points[path.top()].lat);
              ln = to_string(points[path.top()].lon);
              assert(Serial.writeline("W "));
              assert(Serial.writeline(lt));
              assert(Serial.writeline(" "));
              assert(Serial.writeline(ln));
              assert(Serial.writeline("\n"));
              // pop what we outputed
              path.pop();
              // increment counter
              counter++;
            }
            else {
              cout << "didnt get an A " << endl;
            }
          }
          // last input that we will take in
          do {
          inputEnd = Serial.readline(1000);
        } while (inputEnd=="");
          // output E since we are done
          assert(Serial.writeline("E"));
          assert(Serial.writeline("\n"));
          pathisdone = true;
          // output the coordinates just to show they are different if you try to draw again
          cout <<"this was the input coordinates: " << inputcoord << endl;
        }
      }
    }
  return 0;
}
