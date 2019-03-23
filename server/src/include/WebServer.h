#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "server_http.hpp"
#include "client_http.hpp"
#include <boost/filesystem.hpp>
#define BOOST_SPIRIT_THREADSAFE
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "Graph.h"

using HttpServer = SimpleWeb::Server<SimpleWeb::HTTP>;

class WebServer {
  public:
    void start(Graph * graph);
};

#endif // WEBSERVER_H