#ifndef Graph_h
#define Graph_h

#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>
#include <map>

#include "Node.h"
#include "Edge.h"

struct Graph {
  std::map<std::int64_t, int> osmNodeIdVectorIndexMap;
  std::vector<Node> nodes;
  std::vector<Edge> edges;
  // im offset array steht das erste Vorkommen eines Knotens im edges array
  // der lokale Index (im nodes array) wird über die Postition im offset array angegeben, das erste Auftreten im edges array steht an der Stelle im offset array
  std::vector<int> offset;
  int nodesCounter;
  int edgesCounter;
  void genOffset();
  void sortEdges();
};

#endif // Graph_h
