#ifndef Graph_h
#define Graph_h

#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>
#include <map>
#include <unordered_map>
#include <math.h>
#include <set>
#include <utility>
#include <queue>

// #include <GeographicLib/Geodesic.hpp>

#include "Node.h"
#include "Edge.h"

struct DijkstraVertex {
  DijkstraVertex(int nodeId);

  int nodeId;
  int predNodeId;
  double cost;
  bool visited;

  bool operator< (const DijkstraVertex &vertex) const;
};

/**
 * 
 */
struct DijkstraPriorityQueueVertex {
  DijkstraPriorityQueueVertex(DijkstraVertex &vertex);

  DijkstraVertex vertex;

  bool operator< (const DijkstraPriorityQueueVertex &queueVertex) const;
};

/**
 * 
 */
struct DijkstraStructure {
  std::vector<DijkstraVertex> vertices;
  std::priority_queue<DijkstraPriorityQueueVertex> priorityQueue;
};

// struct neighbor {
//     int targetNodeId;
//     double cost;
//     neighbor(int _targetNodeId, double _cost)
//         : targetNodeId(_targetNodeId), cost(_cost) { }
// };

// typedef std::vector<std::vector<neighbor>> neighbors;

/**
 * 
 */
struct Graph {
  std::unordered_map<std::int64_t, int> osmNodeIdVectorIndexMap;
  std::vector<Node> nodes;
  std::vector<Edge> edges;
  // der lokale Index (im nodes array) wird über die Postition im offset array angegeben, das erste Auftreten im edges array steht an der Stelle im offset array
  std::vector<int> offset;  // im offset array steht das erste Vorkommen eines Knotens im edges array (als srcNodeId) // es enthält den Wert '-1' wenn der Knoten keine ausgehenden Kanten besitzt
  int nodesCounter;
  int edgesCounter;
  void genOffset();
  void sortEdges();
  Node getNode(long &id);
  Node getNearestNode(double lon, double lat);
  long getNodePosition(Node &node);
  // Edge getNearestEdge(double lon, double lat);
  double calcDistance(double lat1, double lon1, double lat2, double lon2);
  // void foreignCalcEdgeDistance(Edge edge);
  void dijkstraCalcPaths(DijkstraStructure &dijkstraStruct, int sourceNodeId, std::vector<int> otherPositions);
  void dijkstraStructInit(int sourceNodeId, DijkstraStructure &dijkstraStruct);
  std::vector<int> dijkstraPath(int source, int end, DijkstraStructure &dijkstraStruct);
};

#endif // Graph_h
