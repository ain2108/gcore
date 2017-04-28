#include <string>
#include <iostream>
#include <assert.h>

#include "../../src/Graph.h"

int main(){
/* Create few nodes. nullptr is the data pointer */
	auto n1 = create_node<string, int>("A", nullptr);
	auto n2 = create_node<string, int>("B", nullptr);
	auto n3 = create_node<string, int>("C", nullptr);
	auto n4 = create_node<string, int>("D", nullptr);
	auto extra = create_node<string, int>("Z", nullptr);

	auto gAM1 = create_graph<string, int, int, GraphAL>();
	add_node(gAM1, n1);
	add_node(gAM1, n2);
	add_node(gAM1, n3);
	add_node(gAM1, n4);

	auto nodes = get_nodes(gAM1);
	assert((nodes[0] == n1) && (nodes[1] == n2) && (nodes[2] == n3) && (nodes[3] == n4) && "Node addition is broken");
	cout << "AL_add_node: OK" << endl;
}