#include "include/Node.h"

Node::Node(long _id) {
  id = _id;
}

Node::Node(long _id, float _lat, float _lon) {
  id = _id;
  lat = _lat;
  lon = _lon;
}
