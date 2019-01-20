#ifndef Edge_h
#define Edge_h

struct Edge {
  int srcNodeId; // a local ID in graph.nodes vector
  int tgtNodeId; // a local ID in graph.nodes vector

  Edge(int _srcNodeId, int _tgtNodeId);
};

#endif // Edge_h
