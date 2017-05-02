#include <string>
#include <iostream>
#include <assert.h>

#include "../../src/gcore.h"


int main(){

	auto g = create_graph<string, int, int, GraphAM>();

	auto n1 = create_node<string, int>("A", nullptr);
	auto n2 = create_node<string, int>("B", nullptr);
	auto n3 = create_node<string, int>("C", nullptr);	

	auto e1 = create_edge<string, int, int>(n1, 1, n2);
	auto e2 = create_edge<string, int, int>(n1, 3, n3);
	auto e3 = create_edge<string, int, int>(n2, 10, n1);
	
	add_node(g, n1);
	add_node(g, n2);
	add_node(g, n3);

	add_edge(g, e1);
	add_edge(g, e2);
	add_edge(g, e3);

	auto edges = get_edges(g);

	assert(edges[0] == e1 && edges[1] == e2 && edges[2] == e3);

	cout << "AM_get_edges: OK\n";
	return 0;
}