#include "include/Node.h"
#include "include/Edge.h"
#include "include/Graph.h"
#include "include/GraphReader.h"
#include "include/WebServer.h"
#include <chrono>
#include <iostream>
#include <string>
#include <osmpbf/parsehelpers.h>
#include <osmpbf/inode.h>
#include <osmpbf/iway.h>
#include <osmpbf/irelation.h>
#include <osmpbf/filter.h>
#include <fstream>

void readMapData(std::string fileName, Graph * graph) {
  
  graph->nodesCounter = 0;
  graph->edgesCounter = 0;
  GraphReader gr;

  auto graphInputStartTime = std::chrono::high_resolution_clock::now();
  
  auto edgeStartTime = std::chrono::high_resolution_clock::now();
  gr.readEdges(fileName, graph);
  graph->sortEdges();
  graph->genOffset();
  auto edgeEndTime = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> edgeDuration = edgeEndTime - edgeStartTime;
  std::cout << "--> reading/writing edges took " << edgeDuration.count() << " ms" << std::endl;
  
  auto nodeStartTime = std::chrono::high_resolution_clock::now();
  gr.fillNodes(fileName, graph);
  auto nodeEndTime = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> nodeDuration = nodeEndTime - nodeStartTime;
  std::cout << "--> writing nodes took " << nodeDuration.count() << " ms" << std::endl;

  auto graphInputEndTime = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> graphInputDuration = graphInputEndTime - graphInputStartTime;
  std::cout << "--> graph input took " << graphInputDuration.count() << " ms" << std::endl;

  std::cout << std::endl << "getting amount of edge types..." << std::endl;

  std::map<std::string, int> edgeTypeCounter;
  for (std::vector<Edge>::iterator it = graph->edges.begin(); it != graph->edges.end(); it++) {
    if (!edgeTypeCounter.insert(std::pair<std::string, int>(it->highwayTag, 1)).second) {
      edgeTypeCounter.find(it->highwayTag)->second++;
    }
    // graph->calcEdgeDistance(it);
  }

  // table output
  std::cout << std::endl << std::setw(15) << "type" << " | " << "amount" << std::endl;  // output
  std::cout << std::setw(15)<< "--------------" << " | " << "--------------" << std::endl;  // output
  std::cout << std::setw(15) << "nodes" << " | " << graph->nodesCounter << std::endl;  // output
  std::cout << std::setw(15) << "edges" << " | " << graph->edgesCounter << std::endl;  // output
  for (std::map<std::string, int>::iterator it = edgeTypeCounter.begin(); it != edgeTypeCounter.end(); it++) {
    std::cout << std::setw(15) << it->first << " | " << it->second << std::endl;
  }
}

int main(int argc, char *argv[]) {
  std::string fileName;
  std::cout << "#args: " << argc << std::endl;
  if (argc > 1) {
    std::cout << "OSM map filename: " << argv[1] << std::endl;
    fileName = argv[1];
    Graph graph;

    // reading osmpbf
    readMapData(fileName, &graph);

    // calculate distances for each edge in graph
    for (std::vector<Edge>::iterator it = graph.edges.begin(); it != graph.edges.end(); it++) {
      it->distance = graph.calcDistance(graph.nodes[it->srcNodeId].lat, graph.nodes[it->srcNodeId].lon, graph.nodes[it->tgtNodeId].lat, graph.nodes[it->tgtNodeId].lon);
      it->calcCost();
    }
    std::cout << std::endl << "--> distances calculated" << std::endl;  // output

    // std::ofstream outputfile;
    // outputfile.open("outputfile.txt");
    // for (auto edge : graph.edges) {
    //   outputfile << "edge-distance: " << edge.getDistance() << std::endl;
    // }
    // outputfile.close();

    // webserver
    WebServer ws;
    std::cout << std::endl << "starting server at http://localhost:8091" << std::endl;
    ws.start(&graph);

    // std::string input;
    
    // do
    // {
    //   std::cout << std::endl << "MAIN MENU:" << std::endl;
    //   std::cout << "# press -r- to read the graph data" << std::endl;
    //   std::cout << "# press -w- to start the web server" << std::endl;
    //   std::cout << "# press -s- to stop the web server" << std::endl;
    //   std::cout << "# press -q- to quit" << std::endl;
      
    //   std::cin >> input;
      
    //   if (input == "r") {
    //     std::cout << "reading data..." << std::endl;
    //     readMapData(fileName, &graph);
    //   }
    //   else if(input == "w") {
    //     std::cout << "starting web server..." << std::endl;
    //     WebServer ws;
    //     ws.start(&graph);
    //   }

    // } while (input != "q");

    

  } else {
    std::cout << "OSM map file needed!" << std::endl;
    return -1;
  }

  return 0;
}
