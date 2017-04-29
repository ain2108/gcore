#include <string>
#include <iostream>
#include <assert.h>

#include "../../src/gcore.h"


int main(){

	auto n1 = create_node<string, int>("A", nullptr);
	auto n2 = create_node<string, int>("B", nullptr);
	auto n3 = create_node<string, int>("C", nullptr);
	

	cout << "equality_Node: OK\n";

	return 0;	
}