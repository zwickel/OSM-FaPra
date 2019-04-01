#ifndef Node_h
#define Node_h

#include <iostream>
// #include <vector>

struct Node {
  long id; // global ID from OSM
  double lat;
  double lon;

  // double cost;
  // int predecessor;
  // bool visited;

  // friend bool operator== (const Node &n1, const Node &n2) {
  //   return (n1.id == n2.id);
  // }

  // friend bool operator< (const Node &n1, const Node &n2) {
  //   return (n1.cost < n2.cost);
  // };

  Node (long _id);
  Node (long _id, float _lat, float _lon);
};

#endif // Node_h
