#include <string>
#include <iostream>
#include <assert.h>

#include "../../src/gcore.h"


int main(){

	auto g = create_graph<string, int, int, GraphAM>();

	auto n1 = create_node<string, int>("A", nullptr);
	auto n2 = create_node<string, int>("B", nullptr);

	auto e1 = create_edge<string, int, int>(n1, 1, n2);
	auto e2 = create_edge<string, int, int>(n1, 3, n2);
	auto e3 = create_edge<string, int, int>(n2, 10, n1);
	
	add_node(g, n1);
	add_node(g, n2);
	add_edge(g, e1);

	assert(has_edge(g, e1) == true);
	assert(has_edge(g, e2) == false);
	assert(has_edge(g, e3) == false);

	cout << "AM_has_node: OK\n";
	return 0;
}