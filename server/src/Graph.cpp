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
    
    // if (edges[i].srcNodeId != c) {
    //   while (edges[i].srcNodeId > c) {
    //     offset.push_back(-1);
    //     c++;
    //   }

    // }

    // if(edges[i].srcNodeId != j) {
    //   while (edges[i].srcNodeId != j) {
    //     offset.push_back(-1);
    //     j++;
    //   }
    //   // j++;
    //   offset.push_back(i);
    // }
  }
}

Node Graph::getNearestNode(double lon, double lat) {
  std::vector<Node>::iterator it = nodes.begin();
  Node result = *it;
  double distance = sqrt(pow((lon - it->lon), 2.0) + pow((lat - it->lat), 2.0));
  double currentDistance;
  for (; it != nodes.end(); it++) {
    currentDistance = sqrt(pow((lon - it->lon), 2.0) + pow((lat - it->lat), 2.0));
    if (currentDistance < distance) {
      distance = currentDistance;
      result = *it;
    }
  }

  return result;
}
