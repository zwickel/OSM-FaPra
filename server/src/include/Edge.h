#ifndef Edge_h
#define Edge_h

#include <iostream>

struct Edge {
  int srcNodeId; // a local ID in graph.nodes vector
  int tgtNodeId; // a local ID in graph.nodes vector
  // std::map<std::string> tags;
  std::string highwayTag; // value of the according highway tag
  // int costs; // costs according to the length of the edge (distance between source and target node)

  Edge(int _srcNodeId, int _tgtNodeId);
  Edge(int _srcNodeId, int _tgtNodeId, std::string _highwayTag);
};

#endif // Edge_h
