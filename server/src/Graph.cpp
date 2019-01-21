#include "Graph.h"

struct compStruct {
  bool operator() (Edge e1, Edge e2) {
    return (e1.srcNodeId < e2.srcNodeId);
  }
} edgesComparer;

void Graph::sortEdges() {
  std::sort(edges.begin(), edges.end(), edgesComparer);
}

void Graph::genOffset() {
  int i = 0;
  int n = 0;
  for (; i < edgesCounter; i++) {
    if (edges[i].srcNodeId == n) {
      offset.push_back(i);
      n++;
    } else if (edges[i].srcNodeId < n) {
      continue;
    } else /*if (edges[i].srcNodeId > n)*/ {
      offset.push_back(-1);
      n++;
      i--;
    }
  }
}

// returns nearest node in vector graph.nodes to the input coords (lon, lat)
Node Graph::getNearestNode(double lon, double lat) {
  std::vector<Node>::iterator it = nodes.begin();
  Node result = *it;
  double distance = sqrt(pow((lon - it->lon), 2.0) + pow((lat - it->lat), 2.0));
  double currentDistance;
  
  it++;
  for (; it != nodes.end(); it++) {
    currentDistance = sqrt(pow((lon - it->lon), 2.0) + pow((lat - it->lat), 2.0));
    if (currentDistance < distance) {
      distance = currentDistance;
      result = *it;
    }
  }
  return result;
}

// returns nearest edge in vector graph.edges to the input coords (lon, lat)
Edge Graph::getNearestEdge(double lon, double lat) {
  std::vector<Edge>::iterator it = edges.begin();
  Edge result = *it;
  Node src = nodes[it->srcNodeId];
  Node tgt = nodes[it->tgtNodeId];
  double distance = abs((src.lat - src.lat) * lon - (src.lon - src.lon) * lat + src.lon * src.lat - tgt.lat * src.lon) / sqrt(pow(tgt.lat - src.lat, 2) + pow(tgt.lon - src.lon, 2));
  double currentDistance;

  it++;
  for (; it != edges.end(); it++) {
    currentDistance = abs((src.lat - src.lat) * lon - (src.lon - src.lon) * lat + src.lon * src.lat - tgt.lat * src.lon) / sqrt(pow(tgt.lat - src.lat, 2) + pow(tgt.lon - src.lon, 2));
    if (currentDistance < distance) {
      distance = currentDistance;
      result = *it;
    }
  }
  return result;
}
