#include <string>
#include <iostream>
#include <assert.h>

#include "../../src/gcore.h"


int main(){

	auto g = create_graph<string, int, int, GraphAM>();

	auto n1 = create_node<string, int>("A", nullptr);
	auto n2 = create_node<string, int>("B", nullptr);

	auto e1 = create_edge<string, int, int>(n1, 1, n2);
	
	add_node(g, n1);
	add_node(g, n2);
	add_edge(g, e1);

	auto e2 = get_edge(g, n1, n2);
	assert(e2 == e1);

	cout << "AM_get_edge: OK\n";
	return 0;
}