#ifndef Graph_h
#define Graph_h

#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>
#include <map>
#include <unordered_map>
#include <math.h>

// #include <GeographicLib/Geodesic.hpp>

#include "Node.h"
#include "Edge.h"

struct Graph {
  std::unordered_map<std::int64_t, int> osmNodeIdVectorIndexMap;
  std::vector<Node> nodes;
  std::vector<Edge> edges;
  // im offset array steht das erste Vorkommen eines Knotens im edges array
  // der lokale Index (im nodes array) wird über die Postition im offset array angegeben, das erste Auftreten im edges array steht an der Stelle im offset array
  std::vector<int> offset;
  int nodesCounter;
  int edgesCounter;
  void genOffset();
  void sortEdges();
  Node getNearestNode(double lon, double lat);
  Edge getNearestEdge(double lon, double lat);
  // void calcEdgeDistance(Graph graph, Edge edge);
};

#endif // Graph_h
