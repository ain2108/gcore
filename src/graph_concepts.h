#ifndef GRAPH_CONCEPTS_H
#define GRAPH_CONCEPTS_H

#include <vector>
#include <memory>

#include <iostream>
#include <algorithm>
#include <vector>
#include <memory>
#include <map>
#include <assert.h>
#include <stdexcept>
#include <utility>

using namespace std;

//#include "graph_concepts.h"


template <typename IdType, typename DataType>
class Node;
template <typename IdType, typename WeightType, typename DataType>
class Edge;


template<typename IdType>
concept bool Comparable =
requires (IdType id1, IdType id2) {
	{ id1 == id2 } -> bool;
	{ id1 != id2 } -> bool;
	{ id1 < id2 } -> bool;
	{ id1 > id2 } -> bool;
	{ id1 <= id2 } -> bool;
	{ id1 >= id2 } -> bool;
};

template<typename WeightType>
concept bool Numeric =
requires (WeightType w1, WeightType w2) {
	{ w1 + w2 } -> WeightType;
	{ w1 - w2 } -> WeightType;
	{ w1 * w2 } -> WeightType;
	{ w1 / w2 } -> WeightType;
	{ w1 % w2 } -> WeightType;
};

#endif