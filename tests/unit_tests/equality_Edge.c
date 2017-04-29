#include <iostream>
#include <assert.h>

#include "../../src/gcore.h"


int main(){

	int a = 10;
	auto n1 = create_node<string, int>("A", nullptr);
	auto n1copy = create_node<string, int>("A", &a);
	auto n2 = create_node<string, int>("B", nullptr);
	auto n3 = create_node<string, int>("C", nullptr);

	auto e1 = create_edge<string, int, int>(n1, 1, n2);
	auto e1copy = create_edge<string, int, int>(n1copy, 1, n2);
	auto e2 = create_edge<string, int, int>(n1copy, 1, n3);
	auto e3 = create_edge<string, int, int>(n2, 1, n3);
	auto e3reverse = create_edge<string, int, int>(n3, 1, n2);
	auto e3difweight = create_edge<string, int, int>(n3, 2, n2);

	assert((e1 == e1copy) && (e1copy == e1copy) && (e1copy == e1) 
		&& (!(e1copy == e2)) && (!(e1copy == e3)) && (!(e3reverse == e3))
		&& (!(e3difweight == e3)));

	cout << "equality_Edge: OK\n";

	return 0;	
}