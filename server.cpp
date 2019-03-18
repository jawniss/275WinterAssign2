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
#include <cstdlib>
#include <cassert>

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

// finds the id of the point that is closest to the given point "pt"
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
  string inputcoord;
  string inputAck;
  string inputEnd;
  string ln,lt;
  string p[5];
  bool timeout = false;
  bool pathisdone = false;
  //bool readR == false;
  readGraph("edmonton-roads-2.0.1.txt", graph, points);



  // while loop to read in inputs
  while (true){
    // case to wait until we read a R
    cout << "start server " << endl;
    timeout = false;
    pathisdone = false;

    while ((timeout == false) && (pathisdone == false)){
      do {
        // timeout for readline
      inputcoord = Serial.readline(1000);
      //if (inputcoord == ""){
        //timeout = true;
        //cout << "timeout" << endl;
        //break;
      //}
    } while (inputcoord.find("R") == (string::npos));

      cout << " inputcoord " << endl;
      cout << inputcoord << endl;

        cout << "getting line " << endl;
        // split string
        int at = 0;
        for (auto c: inputcoord){
          if (c == ' '){
            cout << "in at" << endl;
            at++;
          }
          else {
            p[at] += c;
          }
          assert(at < 5);
        }
      //}
        if (p[0] == "R"){
          begin.lat = stoll(p[1]);
          begin.lon = stoll(p[2]);
          end.lat = stoll(p[3]);
          end.lon = stoll(p[4]);
          cout << "lat1" << begin.lat << endl;
          cout << "lon1" << begin.lon << endl;
          cout << "lat2" << end.lat << endl;
          cout << "lon2" << end.lon << endl;
        }

        cout << "find closest" << endl;
        int start = findClosest(begin,points);
        int last = findClosest(end,points);

        cout << "compiled" << endl;
        cout << "start: " << start << endl;
        cout << "last: " <<last << endl;



        unordered_map<int, PLI> searchTree;
        cout << "after search tree" << endl;
        // call dijkstra function
        // seg fault here
        dijkstra(graph, start, searchTree);
        cout << "after dijkstra" << endl;
        // initialize a stack
        stack<int>path;
        finalPoint = last;
        cout << "before looking for path" << endl;
        // while loop that pushes the path until we reach the end vertex
        while (path.top() != start) {
          path.push(finalPoint);
          // crawl up the searchtree one step at a time
          finalPoint = searchTree[finalPoint].second;
        }
        cout << "found path" << endl;
        length = path.size();
        string lengthstr = to_string(length);
        if ((length > 500) || (length == 0)){
          assert(Serial.writeline("N 0\\n"));
        }
        // output the length
        else {
          cout << "N " << path.size() << endl;
          assert(Serial.writeline("N "));
          assert(Serial.writeline(lengthstr));
          assert(Serial.writeline("\n"));


          // start a while loop that will loop for the size of the stack
          while (counter != length){

            // read in a letter
            //cin >> code;
            cout << "before reading A" << endl;
            do {
            inputAck = Serial.readline(1000);
            if (inputAck == ""){
              cout << "took too long: breaking" << endl;
              timeout = true;
              break;
            }
            //cout << "inside looking for A loop" << endl;
          } while (inputAck.find("A") == (string::npos));
            cout << "passed getting a value" << endl;
            cout << "inputAck: " << inputAck << endl;
            // if case for if we read a "A"

            if (inputAck.find("A") != (string::npos) ){
              cout << "read A" << endl;
              // output the latitude and longitude of the point we are taking off the stack
              //cout << "W " << points[path.top()].lat << " " << points[path.top()].lon << endl;
              lt = to_string( points[path.top()].lat );
              cout << "got lat" << endl;
              ln = to_string(points[path.top()].lon);
              cout << "got lon" << endl;
              assert(Serial.writeline("W "));
              cout << "sent W" << endl;
              assert(Serial.writeline(lt));
              cout << "sent lat: " << lt << endl;
              assert(Serial.writeline(" "));
              assert(Serial.writeline(ln));
              cout << "sent lon: " << ln << endl;
              assert(Serial.writeline("\n"));
              cout << "newline" << endl;
              // pop what we outputed
              path.pop();
              counter++;
            }
            else {
              cout << "didnt get an A " << endl;
            }
          }
          // last input that we will take in
          //cin >> code;
          do {
          inputEnd = Serial.readline(1000);
        } while (inputEnd=="");
          // output E since we are done
          cout << "E" << endl;
          assert(Serial.writeline("E"));
          assert(Serial.writeline("\n"));
          pathisdone = true;
          //timeout == true;
          cout <<" this was the input coordinates: " <<inputcoord << endl;
        }

      }

    }

  return 0;
}
