#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "server_http.hpp"
#include "client_http.hpp"
#include <boost/filesystem.hpp>

using HttpServer = SimpleWeb::Server<SimpleWeb::HTTP>;
using HttpClient = SimpleWeb::Client<SimpleWeb::HTTP>;

class WebServer {
  public:
    void start();
};

#endif // WEBSERVER_H