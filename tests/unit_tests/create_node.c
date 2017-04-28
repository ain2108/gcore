#include <string>
#include <iostream>
#include <assert.h>

#include "../../src/gcore.h"

int main(){

	/* Create few nodes. nullptr is the data pointer */
	auto n1 = create_node<string, int>("A", nullptr);
	auto n2 = create_node<string, int>("B", nullptr);

	assert((n1->get_id() == "A") && "n1 must have id A");
	assert((n2->get_id() == "B") && "n1 must have id B");
	assert((n2->get_data() == nullptr) && "n2 must have data equal to nullptr");

	cout << "create_node: OK" << endl;

	return 0;

}