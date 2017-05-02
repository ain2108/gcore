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
	auto extra = create_node<string, int>("E", nullptr);

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
	auto g_other_node = copy_graph<string, int, int, GraphAL>(g);
	auto g_other_edge = copy_graph<string, int, int, GraphAL>(g);

	add_node(g_other_node, extra);
	add_edge(g_other_edge, n1, 3, n4); 


	assert((g_copy == g) && "Graph equality fails");
	assert((!(g_other_node == g)) && "Graph equality fails");
	assert((!(g_other_edge == g)) && "Graph equality fails");
	
	cout << "equality_Graph: OK\n";

	return 0;	
}