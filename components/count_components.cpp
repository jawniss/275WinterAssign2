/*
# ----------------------------------------------
#   Name: Ricky Au
#   ID: 1529429
#   CMPUT 275, Winter 2018
#
#    Weekly Exercise 5: Graph Concepts
# ----------------------------------------------
*/

#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <list>
#include "digraph.h"
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

// breadthFirstSearch from eclass
unordered_map<int, int> breadthFirstSearch(const Digraph& graph, int startVertex) {
  unordered_map<int, int> searchTree;
  searchTree[startVertex] = -1;
  queue<int> q;
  q.push(startVertex);
  while (!q.empty()) {
    int v = q.front();
    q.pop();
    for (auto iter = graph.neighbours(v); iter != graph.endIterator(v); iter++) {
      if (searchTree.find(*iter) == searchTree.end()) {
        searchTree[*iter] = v;
        q.push(*iter);
      }
    }
  }
  return searchTree;
}

// count components function that takes in single parameter
// while returning an iteger
int count_components(Digraph& graph){
  int counter = 0;
  // obtain the graphs vertices
  vector<int> vertexes = graph.vertices();
  //vector that will take in all values that are connected
  vector<int> alreadyFound;
  // iterate the graphs vertices
  vector<int>::iterator iter = vertexes.begin();
  // loop to iterate through all vertices
  while (iter!=vertexes.end()){
    // if we don't find vertex in the already found vector traverse it and increment counter
    if ( (find(alreadyFound.begin(), alreadyFound.end(), *iter) != alreadyFound.end()) == false){
      // use breadthFirstSearch to find all connecting
      unordered_map<int, int> searchTree = breadthFirstSearch(graph, *iter);
      // for loop that gets all values that connected to each iteration
      for (auto it : searchTree){
        auto eliminate = it.first;
        // add the found connected values to alreadyFound vector
        alreadyFound.push_back(eliminate);
      }
      // increment a counter
      counter++;
    }
    // advance the iterator
    iter++;
  }
  return counter;
}

// reading the city graph function that takes in a string text file
Digraph read_city_graph_undirected(string filename){
  Digraph graph;
  string line;
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
            // convert value to integer from stirng
            vertex = stoi(tokenVert,nullptr,10);
          }
          // don't iterate the rest since we only need the vertex
          if (countVert > 2){
            break;
          }
        }
        // add it to our graph
        graph.addVertex(vertex);
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
        // undirected graph so we need to do both ways
        graph.addEdge(u,v);
        graph.addEdge(v,u);
      }
    }
    // close the file
    infile.close();
  }
  // case to exit if the file doesn't open
  else{
    cout << "Error opening file";
    exit (1);
  }
  // return our graph of class Digraph
  return graph;
}

// main function
int main() {
  // call city graph function to read in text file building the graph
  Digraph g = read_city_graph_undirected("edmonton-roads-2.0.1.txt");
  // with that graph built now count how many connected components
  cout << count_components(g) << endl;
  return 0;
}
