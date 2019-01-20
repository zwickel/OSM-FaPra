#ifndef Node_h
#define Node_h

#include <iostream>

struct Node {
  long id; // global ID from OSM
  double lat;
  double lon;

  Node (long _id);
  Node (long _id, float _lat, float _lon);
};

#endif // Node_h
