#include <string>
#include <iostream>
#include <assert.h>

#include "../../src/gcore.h"


int main(){

	int a = 10;
	auto n1 = create_node<string, int>("A", nullptr);
	auto n1copy = create_node<string, int>("A", &a);
	auto n2 = create_node<string, int>("B", nullptr);
	

	bool should_be_true = (n1 == n1copy);
	bool should_be_false = (n1 == n2);

	assert(should_be_true && "Equality comparison fails");
	assert((!should_be_false) && "Equality comparison fails");

	cout << "equality_Node: OK\n";

	return 0;	
}