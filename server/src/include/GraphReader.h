#ifndef GraphReader_h
#define GraphReader_h

#include "Node.h"
#include "Edge.h"
#include "Graph.h"
#include <osmpbf/parsehelpers.h>
#include <osmpbf/inode.h>
#include <osmpbf/iway.h>
#include <osmpbf/irelation.h>
#include <osmpbf/filter.h>

class GraphReader {
public:
  void parseBlock(osmpbf::PrimitiveBlockInputAdaptor & pbi, Graph * graph);
  int readEdges(std::string fileName, Graph * graph);
  int fillNodes(std::string fileName, Graph * graph);
};

#endif // GraphReader_h
