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
	bool operator< (const Node *n) {
		return x < n->x;
	}
	float x;
};

int main() {
	int n = 10;
	
	MisterHeapy<Node> heapy(n);		// Create our MisterHeapy instance
	std::vector<Node> nodes;			// Allocate nodes
	nodes.resize(n);
	
	for (int i=0; i < n; i++) {
		nodes[i].x = i;				// Populate heapy
		heapy.fast_push(&nodes[i]);			// Since we're using fast_push, will be disordered
	}

	std::cout << "Popping heap: " << heapy.pop()->x << ", reheapifying..." << std::endl;
	heapy.reheapify();
	std::cout << "Popping heap: " << heapy.pop()->x << std::endl;
	std::cout << "Popping heap: " << heapy.pop()->x << std::endl;

	return 0;
}
