#include <string>
#include <iostream>


#include "../../src/gcore.h"
#include "../../src/algo.h"
#include "../../src/utility.h"


int main(){

	auto g = create_graph<string, int, int, GraphAL>();

	auto n1 = create_node<string, int>("A", nullptr);
	auto n2 = create_node<string, int>("B", nullptr);
	auto n3 = create_node<string, int>("C", nullptr);
	auto n4 = create_node<string, int>("D", nullptr);
	auto n5 = create_node<string, int>("E", nullptr);

	add_node(g, n1);
	add_node(g, n2);
	add_node(g, n3);
	add_node(g, n4);
	add_node(g, n5);

	add_edge(g, n1, 1, n2);
	add_edge(g, n1, 1, n3);
	add_edge(g, n1, 1, n4);
	add_edge(g, n1, 1, n5);

	add_edge(g, n2, 1, n1);
	add_edge(g, n2, 1, n3);
	add_edge(g, n2, 1, n4);
	add_edge(g, n2, 1, n5);

	add_edge(g, n3, 1, n2);
	add_edge(g, n3, 1, n1);
	add_edge(g, n3, 1, n4);
	add_edge(g, n3, 1, n5);

	add_edge(g, n4, 1, n2);
	add_edge(g, n4, 1, n3);
	add_edge(g, n4, 1, n1);
	add_edge(g, n4, 1, n5);

	add_edge(g, n5, 1, n2);
	add_edge(g, n5, 1, n3);
	add_edge(g, n5, 1, n4);
	add_edge(g, n5, 1, n1);


	cout << "Original Graph:\n";
	g->print_graph();


	auto n1dfs = bfs(g, n1);
	auto n2dfs = bfs(g, n2);
	auto n3dfs = bfs(g, n3);
	auto n4dfs = bfs(g, n4);
	auto n5dfs = bfs(g, n5);


	cout << "Rooted at A\n";
	n1dfs->print_graph();

	cout << "Rooted at B\n";
	n2dfs->print_graph();

	cout << "Rooted at C\n";
	n3dfs->print_graph();

	cout << "Rooted at D\n";
	n4dfs->print_graph();

	cout << "Rooted at E\n";
	n5dfs->print_graph();

	return 0;
}