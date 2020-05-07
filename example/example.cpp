#include <vector>
#include <iostream>

#define _MH_NO_INDEX_TRACKING
#include "../MisterHeapy.hpp"

class Node {
public:
  Node() { }
  bool operator< (const Node *n) {
    return x < n->x;
  }
  float x;
};

int main() {
  int n = 10;

  MisterHeapy<Node> heapy(n);   // Create our MisterHeapy instance
  std::vector<Node> nodes;      // Allocate nodes
  nodes.resize(n);

  for (int i=0; i < n; i++) {
    nodes[i].x = i;               // Populate
    heapy.fast_push(&nodes[i]);   // Since we're using fast_push, will be disordered
  }
  heapy.reheapify();

  nodes[n-1].x = -1;
#ifdef _MH_NO_INDEX_TRACKING      // In either mode we should see node n-1 being re-sorted
  heapy.update_at(0);             // from the top down to the bottom of the heap
#else                             //
  heapy.update(&nodes[n-1]);      //
#endif                            //

  for (int i=0; i < n; ++i) {
    std::cout << "Popping heap: " << heapy.pop()->x << std::endl;
  }

  return 0;
}

