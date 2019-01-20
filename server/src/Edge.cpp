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
