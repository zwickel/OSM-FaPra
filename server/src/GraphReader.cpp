#include "GraphReader.h"

void GraphReader::parseBlock(osmpbf::PrimitiveBlockInputAdaptor & pbi, Graph * graph) {
  // filter for roads
  osmpbf::AndTagFilter roadFilter({
    // new osmpbf::KeyOnlyTagFilter("name"),
    new osmpbf::KeyMultiValueTagFilter("highway", {"motorway", "motorway_link", "trunk", "trunk_link", "primary", "primary_link", "secondary", "secondary_link", "tertiary", "tertiary_link", "unclassified", "residential", "service"})
  });
  // filter for oneway roads
  osmpbf::KeyMultiValueTagFilter oneWayFilter("oneway", {"yes"});

  roadFilter.assignInputAdaptor(&pbi);
  roadFilter.rebuildCache();

  // creating map that maps the OSM node ID with the node vector index
  // graph->osmNodeIdVectorIndexMap;
  long srcNodeId;

  if (pbi.waysSize()) {
    for (osmpbf::IWayStream way = pbi.getWayStream(); !way.isNull(); way.next()) {
      if (roadFilter.matches(way)) {
        // std::cout << "id: " << way.id() << std::endl; // output
        // std::cout << "#tags: " << way.tagsSize() << std::endl; // output

        std::string highwayValue;
        for(uint32_t i = 0, s = way.tagsSize();  i < s; ++i) {
          // std::cout << "\t<tag k=" << way.key(i) << " v=" << way.value(i) << ">" << std::endl;  // output
          if (way.key(i) == "highway") {
            highwayValue = way.value(i);
          }
        }
        // std::cout << "</way>" << std::endl;

        if (way.refsSize()) {
          osmpbf::RefIterator refIter(way.refBegin()), refEnd(way.refEnd());
          // std::cout << typeid(*refIter).name() << std::endl; // output

          srcNodeId = *refIter;
          // std::cout << typeid(srcNodeId).name() << std::endl; // output
          if (graph->osmNodeIdVectorIndexMap.insert(std::pair<int64_t, int>(srcNodeId, graph->nodesCounter)).second) {
            graph->nodes.push_back(Node(srcNodeId));
            graph->nodesCounter++;
          }


          // std::cout << srcNodeId << std::endl; // output
          ++refIter;
          for (; refIter != refEnd; ++refIter) {
            // check if node is already in graph.nodes vector, else add
            // map.insert returns true or false as second parameter
            if(graph->osmNodeIdVectorIndexMap.insert(std::pair<int64_t, int>(*refIter, graph->nodesCounter)).second) {
              // push node
              graph->nodes.push_back(Node(*refIter));
              graph->nodesCounter++;
              // std::cout << graph->nodesCounter << std::endl; // output
            }

            // create new edge to add
            Edge newEdge = Edge(graph->osmNodeIdVectorIndexMap.find(srcNodeId)->second, graph->osmNodeIdVectorIndexMap.find(*refIter)->second, highwayValue);

            // check if edge is already in edges else add it
            // if (std::find(graph->edges.begin(), graph->edges.end(), newEdge) == graph->edges.end()) {
              
              // push edge to edgess
              graph->edges.push_back(newEdge);
              graph->edgesCounter++;

              // check if edge is one-way else add backward edge
              if(!oneWayFilter.matches(way)) {
                graph->edges.push_back(Edge(graph->osmNodeIdVectorIndexMap.find(*refIter)->second, graph->osmNodeIdVectorIndexMap.find(srcNodeId)->second, highwayValue));
                graph->edgesCounter++;
              }
            // }

            // std::cout << graph->edgesCounter << std::endl; // output

            srcNodeId = *refIter;
          }
        }
      }
    }
  }
}

int GraphReader::readEdges(std::string fileName, Graph * graph) {
  osmpbf::OSMFileIn inFile(fileName);
  if (!inFile.open()) {
    return -1;
  }

  osmpbf::PrimitiveBlockInputAdaptor pbi;
  while (inFile.parseNextBlock(pbi)) {
    if (pbi.isNull()) {
      continue;
    }
    parseBlock(pbi, graph);
  }

  inFile.close();
  return 0;
}

// writes lat and lon values to nodes
int GraphReader::fillNodes(std::string fileName, Graph * graph) {
  osmpbf::OSMFileIn inFile(fileName);
  if (!inFile.open()) {
    return -1;
  }

  osmpbf::PrimitiveBlockInputAdaptor pbi;
  // int nodeIndex;

  while(inFile.parseNextBlock(pbi)) {
    if (pbi.isNull()) {
      continue;
    }
    if (pbi.nodesSize()) {
      for (osmpbf::INodeStream node = pbi.getNodeStream(); !node.isNull(); node.next()) {

        // takes too long !!!
        // try
        // {
        //   nodeIndex = graph->osmNodeIdVectorIndexMap.at(node.id());
        // }
        // catch(const std::out_of_range& oor)
        // {
        //   // std::cerr << e.what() << '\n';
        //   continue;
        // }
        // graph->nodes[nodeIndex].lat = node.latd;
        // graph->nodes[nodeIndex].lon = node.lond;

        // if(graph->osmNodeIdVectorIndexMap.count(node.id())>0) {
          const auto& it = graph->osmNodeIdVectorIndexMap.find(node.id());
          if (it != graph->osmNodeIdVectorIndexMap.end()) {
            graph->nodes[it->second].lat = node.latd();
            graph->nodes[it->second].lon = node.lond();
          }
        //   graph->nodes[graph->osmNodeIdVectorIndexMap.find(node.id())->second].lon = node.lond();
        // }
      }
    }
  }

  inFile.close();
  return 0;
}
