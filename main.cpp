/*
 * Example use of MisterHeapy
 *
 */

#include "MisterHeapy.hpp"
#include <vector>
#include <iostream>

class Node {
public:
	Node() { }
	bool operator< (Node *n) {
		return comparand < n->comparand;
	}
	float comparand;
};

int main() {
	int n = 10;
	
	MisterHeapy<Node*, float> heapy(n);		// Create our MisterHeapy instance
	std::vector<Node> nodes;				// Allocate nodes
	nodes.resize(n);
	
	for (int i=0; i < n; i++) {
		nodes[i].comparand = i;				// Populate heapy
		heapy.fast_push(&nodes[i]);			// Since we're using fast_push, will be disordered
	}

	std::cout << "Last in heap: " << heapy.pop()->comparand << ", reheapifying..." << std::endl;
	heapy.reheapify();
	std::cout << "Last in heap: " << heapy.pop()->comparand << std::endl;
	std::cout << "Last in heap: " << heapy.pop()->comparand << std::endl;

	return 0;
}
