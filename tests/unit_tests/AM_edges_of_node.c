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

	auto edges_of_n1 = edges_of_node(g, n1);
	auto edges_of_n2 = edges_of_node(g, n2);
	auto edges_of_n3 = edges_of_node(g, n3);

	assert(edges_of_n1[0] == e1 && edges_of_n1[1] == e2);
	assert(edges_of_n2[0] == e3);
	assert(edges_of_n3.empty());

	cout << "AM_edges_of_node: OK\n";
	return 0;
}