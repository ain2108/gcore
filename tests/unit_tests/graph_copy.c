#include <string>
#include <iostream>
#include <assert.h>

#include "../../src/gcore.h"


int main(){

	auto g = create_graph<string, int, int, GraphAL>();

	auto n1 = create_node<string, int>("A", nullptr);
	auto n2 = create_node<string, int>("B", nullptr);
	auto n3 = create_node<string, int>("C", nullptr);
	auto n4 = create_node<string, int>("D", nullptr);

	auto e1 = create_edge<string, int, int>(n1, 1, n2);
	auto e2 = create_edge<string, int, int>(n1, 3, n3);
	auto e3 = create_edge<string, int, int>(n2, 10, n4);
	auto e4 = create_edge<string, int, int>(n4, 5, n3);

	add_node(g, n1);
	add_node(g, n2);
	add_node(g, n3);
	add_node(g, n4);

	add_edge(g, e1);
	add_edge(g, e2);
	add_edge(g, e3);
	add_edge(g, e4);

	auto g_copy = copy_graph<string, int, int, GraphAL>(g);
	assert((g_copy == g) && "Graph equality failes");

	cout << "Graph_copy: OK\n";

	return 0;
}