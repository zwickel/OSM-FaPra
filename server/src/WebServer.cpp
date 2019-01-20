#include "WebServer.h"

using namespace std;
using namespace boost::property_tree; // for requests with .json

void WebServer::start(Graph * graph) {
  HttpServer server;
  server.config.port = 8091;

  // Default GET-example. If no other matches, this anonymous function will be called.
  // Will respond with content in the web/-directory, and its subdirectories.
  // Default file: index.html
  // Can for instance be used to retrieve an HTML 5 client that uses REST-resources on this server
  server.default_resource["GET"] = [](std::shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
    try {
      auto web_root_path = boost::filesystem::canonical("../../client");
      auto path = boost::filesystem::canonical(web_root_path / request->path);
      // Check if path is within web_root_path
      if(distance(web_root_path.begin(), web_root_path.end()) > distance(path.begin(), path.end()) ||
         !equal(web_root_path.begin(), web_root_path.end(), path.begin()))
        throw invalid_argument("path must be within root path");
      if(boost::filesystem::is_directory(path))
        path /= "index.html";

      SimpleWeb::CaseInsensitiveMultimap header;

      auto ifs = make_shared<ifstream>();
      ifs->open(path.string(), ifstream::in | ios::binary | ios::ate);

      if(*ifs) {
        auto length = ifs->tellg();
        ifs->seekg(0, ios::beg);

        header.emplace("Content-Length", to_string(length));
        response->write(header);

        // Trick to define a recursive function within this scope (for example purposes)
        class FileServer {
        public:
          static void read_and_send(const shared_ptr<HttpServer::Response> &response, const shared_ptr<ifstream> &ifs) {
            // Read and send 128 KB at a time
            static vector<char> buffer(131072); // Safe when server is running on one thread
            streamsize read_length;
            if((read_length = ifs->read(&buffer[0], static_cast<streamsize>(buffer.size())).gcount()) > 0) {
              response->write(&buffer[0], read_length);
              if(read_length == static_cast<streamsize>(buffer.size())) {
                response->send([response, ifs](const SimpleWeb::error_code &ec) {
                  if(!ec)
                    read_and_send(response, ifs);
                  else
                    cerr << "Connection interrupted" << endl;
                });
              }
            }
          }
        };
        FileServer::read_and_send(response, ifs);
      }
      else
        throw invalid_argument("could not read file");
    }
    catch(const exception &e) {
      response->write(SimpleWeb::StatusCode::client_error_bad_request, "Could not open path " + request->path + ": " + e.what());
    }
  };

  server.on_error = [](shared_ptr<HttpServer::Request> /*request*/, const SimpleWeb::error_code & /*ec*/) {
    // Handle errors here
    // Note that connection timeouts will also call this handle with ec set to SimpleWeb::errc::operation_canceled
  };

  thread server_thread([&server]() {
    // Start server
    server.start();
  });

  // Wait for server to start so that the client can connect
  this_thread::sleep_for(chrono::seconds(1));
  
  // GET-example for the path /match/[number], responds with the matched string in path (number)
  // For instance a request GET /match/123 will receive: 123
  server.resource["^/match/([0-9]+)$"]["GET"] = [](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
    response->write(request->path_match[1]);
  };

  // POST-request
  server.resource["^/getnearesttaginfo$"]["POST"] = [&](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
    
    try
    {
      ptree pt;
      read_json(request->content, pt);

      double lon = pt.get<double>("lon");
      double lat = pt.get<double>("lat");;

      Node nearestNode = graph->getNearestNode(lon, lat);

      string jsonResponse = "{\"coords\": {\"lon\":" + to_string(nearestNode.lon) + ", \"lat\":" + to_string(nearestNode.lat) + "}}";

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