#include <string>
#include <iostream>
#include <assert.h>

#include "../../src/gcore.h"


int main(){

	auto g = create_graph<string, int, int, GraphAM>();

	auto n1 = create_node<string, int>("A", nullptr);
	auto n2 = create_node<string, int>("B", nullptr);
	
	add_node(g, n1);
	add_node(g, n2);


	auto nodes = get_nodes(g);
	assert(nodes[0] == n1);
	assert(nodes[1] == n2);

	cout << "AM_get_nodes: OK\n";
	return 0;
}