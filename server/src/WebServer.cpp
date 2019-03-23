#include "WebServer.h"

using namespace boost::property_tree; // for requests with .json

void WebServer::start(Graph * graph) {
  HttpServer server;
  server.config.port = 8091;

  // Default GET-example. If no other matches, this anonymous function will be called.
  // Will respond with content in the web/-directory, and its subdirectories.
  // Default file: index.html
  // Can for instance be used to retrieve an HTML 5 client that uses REST-resources on this server
  server.default_resource["GET"] = [](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request) {
    try {
      // for debugging
      auto web_root_path = boost::filesystem::canonical("client");
      // without debugging
      // auto web_root_path = boost::filesystem::canonical("../../client");

      auto path = boost::filesystem::canonical(web_root_path / request->path);
      // Check if path is within web_root_path
      if(std::distance(web_root_path.begin(), web_root_path.end()) > std::distance(path.begin(), path.end()) ||
         !std::equal(web_root_path.begin(), web_root_path.end(), path.begin()))
        throw std::invalid_argument("path must be within root path");
      if(boost::filesystem::is_directory(path))
        path /= "index.html";

      SimpleWeb::CaseInsensitiveMultimap header;

      auto ifs = std::make_shared<std::ifstream>();
      ifs->open(path.string(), std::ifstream::in | std::ios::binary | std::ios::ate);

      if(*ifs) {
        auto length = ifs->tellg();
        ifs->seekg(0, std::ios::beg);

        header.emplace("Content-Length", to_string(length));
        response->write(header);

        // Trick to define a recursive function within this scope (for example purposes)
        class FileServer {
        public:
          static void read_and_send(const std::shared_ptr<HttpServer::Response> &response, const std::shared_ptr<std::ifstream> &ifs) {
            // Read and send 128 KB at a time
            static std::vector<char> buffer(131072); // Safe when server is running on one thread
            std::streamsize read_length;
            if((read_length = ifs->read(&buffer[0], static_cast<std::streamsize>(buffer.size())).gcount()) > 0) {
              response->write(&buffer[0], read_length);
              if(read_length == static_cast<std::streamsize>(buffer.size())) {
                response->send([response, ifs](const SimpleWeb::error_code &ec) {
                  if(!ec)
                    read_and_send(response, ifs);
                  else
                    std::cerr << "Connection interrupted" << std::endl;
                });
              }
            }
          }
        };
        FileServer::read_and_send(response, ifs);
      }
      else
        throw std::invalid_argument("could not read file");
    }
    catch(const std::exception &e) {
      response->write(SimpleWeb::StatusCode::client_error_bad_request, "Could not open path " + request->path + ": " + e.what());
    }
  };

  server.on_error = [](std::shared_ptr<HttpServer::Request> /*request*/, const SimpleWeb::error_code & /*ec*/) {
    // Handle errors here
    // Note that connection timeouts will also call this handle with ec set to SimpleWeb::errc::operation_canceled
  };

  std::thread server_thread([&server]() {
    // Start server
    server.start();
  });

  // Wait for server to start so that the client can connect
  std::this_thread::sleep_for(std::chrono::seconds(1));
  
  // GET-example for the path /match/[number], responds with the matched string in path (number)
  // For instance a request GET /match/123 will receive: 123
  server.resource["^/match/([0-9]+)$"]["GET"] = [](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request) {
    response->write(request->path_match[1]);
  };

  /**
   * POST-request
   * input coords (lon, lat) where the user clicked
   * return sends back the coords of the nearest node to the coords from input
   */  
  server.resource["^/getnearestnode$"]["POST"] = [&](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request) {
    
    try
    {
      ptree pt;
      read_json(request->content, pt);

      double lon = pt.get<double>("lon");
      double lat = pt.get<double>("lat");

      Node nearestNode = graph->getNearestNode(lon, lat);

      std::string jsonResponse = "{\"coords\": {\"lon\":" + std::to_string(nearestNode.lon) + ", \"lat\":" + std::to_string(nearestNode.lat) + "}}";

      *response << "HTTP/1.1 200 OK\r\n" << "Content-length: " << jsonResponse.length() << "\r\n\r\n" << jsonResponse;
    }
    catch(const std::exception& e)
    {
      std::cerr << e.what() << '\n';
      *response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << strlen(e.what()) << "\r\n\r\n" << e.what();
    }
  };

  /**
   * POST-request
   * input coords (lon, lat) where the user clicked
   * return nearest edge (src, tgt) to the coords from input
   */
  server.resource["^/getnearestedge$"]["POST"] = [&](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request) {
    
    try
    {
      ptree pt;
      read_json(request->content, pt);

      double lon = pt.get<double>("lon");
      double lat = pt.get<double>("lat");

      Edge nearestEdge = graph->getNearestEdge(lon, lat);
      Node src = graph->nodes[nearestEdge.srcNodeId];
      Node tgt = graph->nodes[nearestEdge.tgtNodeId];

      std::string jsonResponse = "{\"edge\":{\"src\":{\"lon\":" + std::to_string(src.lon) + ", \"lat\":" + std::to_string(src.lat) + "}, \"tgt\": {\"lon\":" + std::to_string(tgt.lon) + ", \"lat\":" + std::to_string(tgt.lat) + "}}}";

      *response << "HTTP/1.1 200 OK\r\n" << "Content-length: " << jsonResponse.length() << "\r\n\r\n" << jsonResponse;
    }
    catch(const std::exception& e)
    {
      std::cerr << e.what() << '\n';
      *response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << strlen(e.what()) << "\r\n\r\n" << e.what();
    }
    
  };

  // GET-request to get info of nearest tag
  // server.resource["^/getnearesttaginfo/[0-9]+([\\.\\,][0-9])?/[0-9]+([\\.\\,][0-9])?$"]["GET"] = [](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
  //   stringstream stream;
  //   double lon;
  //   double lat;
  //   auto query_fields = request->parse_query_string();
  //   for(auto &field : query_fields) {
  //     std::cout << field.first << ": " << field.second << "<br>";
  //     if (field.first == "lon") {
  //       lon = stod(field.second);
  //     } else if (field.first == "lat") {
  //       lat = stod(field.second);
  //     }
  //   }


  //   // request->query_string

  //   response->write(request->path_match[1]);
  // };

  server_thread.join();
}