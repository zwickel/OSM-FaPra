#ifndef Edge_h
#define Edge_h

#include <iostream>
// #include <math.h>
// #include "Graph.h"

struct Edge {
  int srcNodeId; // a local ID in graph.nodes vector
  int tgtNodeId; // a local ID in graph.nodes vector
  double distance;
  double cost;
  // std::map<std::string> tags;
  std::string highwayTag; // value of the according highway tag

  // friend bool operator== (const Edge &e1, const Edge &e2);

  Edge(int _srcNodeId, int _tgtNodeId);
  Edge(int _srcNodeId, int _tgtNodeId, std::string _highwayTag);
  double getDistance();
  void setDistance(double _distance);
  void calcCost();
};

#endif // Edge_h
