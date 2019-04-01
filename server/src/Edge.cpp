#include "include/Edge.h"

Edge::Edge(int _srcNodeId, int _tgtNodeId) {
  srcNodeId = _srcNodeId;
  tgtNodeId = _tgtNodeId;
}

Edge::Edge(int _srcNodeId, int _tgtNodeId, std::string _highwayTag) {
  srcNodeId = _srcNodeId;
  tgtNodeId = _tgtNodeId;
  highwayTag = _highwayTag;
}

// bool operator==(const Edge &e1, const Edge &e2) {
//   return (e1.srcNodeId== e2.srcNodeId && e1.tgtNodeId== e2.tgtNodeId);
// }

double Edge::getDistance() {
  return distance;
}
void Edge::setDistance(double _distance) {
  distance = _distance;
};

void Edge::calcCost() {
  cost = distance;
}
