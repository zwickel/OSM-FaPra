#include "Graph.h"

struct compStruct {
  bool operator() (Edge e1, Edge e2) {
    return (e1.srcNodeId < e2.srcNodeId);
  }
} edgesComparer;

void Graph::sortEdges() {
  std::sort(edges.begin(), edges.end(), edgesComparer);
}

/**
 * generates the offset for the graph
 */
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

/**
 * returns nearest node in vector graph.nodes to the input coords (lon, lat)
 */
Node Graph::getNearestNode(double lon, double lat) {
  std::cout << "lon: " << lon << ", lat: " << lat << std::endl;
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

/**
 * returns the local index of a node in the graph.nodes-array
 */
long Graph::getNodePosition(Node &node) {
  long nodeId = node.id;
  for (std::vector<Node>::iterator it = nodes.begin(); it != nodes.end(); it++) {
    if (it->id == nodeId) {
      return std::distance(nodes.begin(), it);
    }
  }
  return -1;
};

/**
 *  returns nearest edge in vector graph.edges to the input coords (lon, lat)
 */
// Edge Graph::getNearestEdge(double lon, double lat) {
//   std::vector<Edge>::iterator it = edges.begin();
//   Edge result = *it;
//   Node src = nodes[it->srcNodeId];
//   Node tgt = nodes[it->tgtNodeId];
//   double distance = abs((src.lat - src.lat) * lon - (src.lon - src.lon) * lat + src.lon * src.lat - tgt.lat * src.lon) / sqrt(pow(tgt.lat - src.lat, 2) + pow(tgt.lon - src.lon, 2));
//   double currentDistance;

//   it++;
//   for (; it != edges.end(); it++) {
//     src = nodes[it->srcNodeId];
//     tgt = nodes[it->tgtNodeId];
//     currentDistance = abs((tgt.lat - src.lat) * lon - (tgt.lon - src.lon) * lat + tgt.lon * src.lat - tgt.lat * src.lon) / sqrt(pow(tgt.lat - src.lat, 2) + pow(tgt.lon - src.lon, 2));
//     if (currentDistance < distance) {
//       std::cout << "currentDistance: " << currentDistance << std::endl;
//       std::cout << "distance: " << distance << std::endl;
//       distance = currentDistance;
//       result = *it;
//     }
//   }
//   std::cout << "result: " << result.srcNodeId << " " << result.tgtNodeId << std::endl;
//   return result;
// }

/**
 * calculates the distance of an edge
 * and writes it to its distance member variable
 */
double Graph::calcDistance(double lat1, double lon1, double lat2, double lon2) {
  double dist = 6378388 * std::acos(std::sin(lat1) * std::sin(lat2) + std::cos(lat1) * std::cos(lat2) * std::cos(lon2 - lon1));

  return dist;
}

// void Graph::foreignCalcEdgeDistance(Edge edge) {
//   GeographicLib::Geodesic::WGS84().Inverse(nodes[edge.srcNodeId].lat, nodes[edge.srcNodeId].lon, nodes[edge.tgtNodeId].lat, nodes[edge.tgtNodeId].lon, edge.distance);
// }

/**
 * 
 */
DijkstraVertex::DijkstraVertex(int nodeId) : nodeId(nodeId), predNodeId(-1), cost(std::numeric_limits<double>::max()), visited(false) {

}

/**
 * 
 */
bool DijkstraVertex::operator< (const DijkstraVertex &vertex) const {
  if (visited) {
    return false;
  } else if (vertex.visited) {
    return true;
  } else {
    return cost < vertex.cost;
  }
};

/**
 * 
 */
DijkstraPriorityQueueVertex::DijkstraPriorityQueueVertex(DijkstraVertex &vertex) : vertex(vertex) {

}

/**
 * 
 */
bool DijkstraPriorityQueueVertex::operator< (const DijkstraPriorityQueueVertex &queueVertex) const {
  return !(vertex < queueVertex.vertex);
};

/**
 * 
 */
void Graph::dijkstraCalcPaths(DijkstraStructure &dijkstraStruct, int sourceNodeId) {
  std::cout << std::endl << "in Dijkstra" << std::endl; // output

  int currentNodeId = sourceNodeId;
  int currentEdgeId = offset[sourceNodeId];
  int currentNeighborNodeId = edges[currentEdgeId].tgtNodeId;

  double currentCost;

  dijkstraStructInit(sourceNodeId, dijkstraStruct);

  // std::cout << "before while" << std::endl; // output

  while (!dijkstraStruct.priorityQueue.empty()) {

    currentNodeId = dijkstraStruct.priorityQueue.top().vertex.nodeId;
    dijkstraStruct.priorityQueue.pop();

    if (dijkstraStruct.vertices[currentNodeId].visited) {
      continue;
    // } else if (currentNodeId == endNodeId) {
    //   break;
    } else {
      dijkstraStruct.vertices[currentNodeId].visited = true;
    }

    currentEdgeId = offset[currentNodeId];

    if (currentEdgeId == -1) {
      continue;
    }

    currentNeighborNodeId = edges[currentEdgeId].tgtNodeId;

    // std::cout << "before inner while" << std::endl; // output
    while (currentEdgeId < edges.size() && edges[currentEdgeId].srcNodeId == currentNodeId) {
      if (!dijkstraStruct.vertices[currentNeighborNodeId].visited) {
        currentCost = dijkstraStruct.vertices[currentNodeId].cost + edges[currentEdgeId].cost;

        if (currentCost < dijkstraStruct.vertices[currentNeighborNodeId].cost) {
          dijkstraStruct.vertices[currentNeighborNodeId].cost = currentCost;
          dijkstraStruct.vertices[currentNeighborNodeId].predNodeId = currentNodeId;
          dijkstraStruct.priorityQueue.push(dijkstraStruct.vertices[currentNeighborNodeId]);
        }
      }

      currentEdgeId++;
      currentNeighborNodeId = edges[currentEdgeId].tgtNodeId;
    }
  }

  std::cout << "leaving Dijkstra" << std::endl; // output
}

void Graph::dijkstraStructInit(int sourceNodeId, DijkstraStructure &dijkstraStruct) {
  for (int i = 0; i < offset.size(); i++) {
    dijkstraStruct.vertices.push_back(DijkstraVertex(i));
  }
  dijkstraStruct.vertices[sourceNodeId].cost = 0.0;
  dijkstraStruct.priorityQueue.push(dijkstraStruct.vertices[sourceNodeId]);
};

std::vector<int> Graph::dijkstraPath(int start, int end, DijkstraStructure &dijkstraStruct) {
  std::vector<int> path;
  int currentNodeId = start;
  // std::cout << "start: " << start << std::endl;

  while (currentNodeId != -1) {
    path.push_back(currentNodeId);
    currentNodeId = dijkstraStruct.vertices[currentNodeId].predNodeId;
    // std::cout << "currentNodeId: " << currentNodeId << std::endl;
  }
  // path.push_back(end);

  return path;
};
