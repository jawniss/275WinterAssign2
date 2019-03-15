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
  string ln,lt;
  string p[5];
  //bool readR == false;
  readGraph("edmonton-roads-2.0.1.txt", graph, points);


  string input;
  //char str[];
  //int count = 0;

  // while loop to read in inputs
  while (true){
    // case to wait until we read a R
    /*
    do {
      input = Serial.readline();
    } while (); // not sure if while loop needed
    */

    do {
    input = Serial.readline();
  } while (input=="");
    cout << " input " << endl;
    cout << input << endl;

    /*
    string parsed;
    istringstream ss(input);
    int count = 0;
    // obtain the u and v edges
    while (getline(ss,parsed,' ')){
      count++;
      // take value after first comma
      if (count == 2){
        // convert it from string to integer
        begin.lat = stoll(parsed);
      }
      // take value after second comma
      if (count == 3){
        // convirt it from string to integer
        begin.lon = stoll(parsed);
      }
      if (count == 4){
        // convirt it from string to integer
        end.lat = stoll(parsed);
      }
      if (count == 5){
        // convirt it from string to integer
        end.lon = stoll(parsed);
      }
      // don't bother iterating the rest since we only need edges
      if (count > 5){
        break;
      }
    }
    */

    cout << "before stream" << endl;
    ifstream fin(input);
    string line;
    cout << "string line stuff" << endl;
    while (getline(fin,line)){
      cout << "getting line " << endl;
      // split string
      int at = 0;
      for (auto c: line){
        if (c == ' '){
          cout << "in at" << endl;
          at++;
        }
        else {
          p[at] += c;
        }
        assert(at < 5);
      }
    }
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


    /*
    int word = 0;
    if (input.find("R")){
      cout << "found R" << endl;
      istringstream ss(input);
      string token;
      while (getline(ss,token,' ')){
        cout << "inside get line" << endl;
        word = word + 1;
        cout << "word: " << word << endl;
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
    */
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
    // while loop that pushes the path until we reach the end vertex
    while (path.top() != start) {
      path.push(finalPoint);
      // crawl up the searchtree one step at a time
      finalPoint = searchTree[finalPoint].second;
    }

    length = path.size();

    if (length > 500){
      assert(Serial.writeline("N 0\\n"));
    }
    // output the length
    else {
      cout << "N " << path.size() << endl;
      assert(Serial.writeline("N "));
      assert(Serial.writeline(path.size()+"\n"));


      // start a while loop that will loop for the size of the stack
      while (counter != length){

        counter++;
        // read in a letter
        //cin >> code;
        do {
        input = Serial.readline();
      } while (input=="");

        // if case for if we read a "A"
        if (input == "A"){
          // output the latitude and longitude of the point we are taking off the stack
          //cout << "W " << points[path.top()].lat << " " << points[path.top()].lon << endl;
          lt = to_string( points[path.top()].lat );
          ln = to_string(points[path.top()].lon);
          assert(Serial.writeline("W "));
          assert(Serial.writeline(lt));
          assert(Serial.writeline(" "));
          assert(Serial.writeline(ln));
          assert(Serial.writeline("\n"));
          // pop what we outputed
          path.pop();
        }
      }
      // last input that we will take in
      //cin >> code;
      do {
      input = Serial.readline();
    } while (input=="");
      // output E since we are done
      cout << "E" << endl;
      assert(Serial.writeline("E \\n"));
    }
  }

  return 0;
}
