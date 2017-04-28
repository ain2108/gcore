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

template <typename I, typename W, typename D, template <typename, typename, typename> typename GraphType>
concept bool IsGraph = 
requires (GraphType<I, W, D> g, 
	shared_ptr<Node<I, D>> n1, 
	shared_ptr<Node<I, D>> n2, 
	shared_ptr<Edge<I, W, D>> e,
	W w){

	{ g.has_node(n1) } -> bool;
	{ g.create_graph() } -> shared_ptr<GraphType<I, W, D>>;
	{ g.has_edge(n1, w, n2) } -> bool;
	{ g.edges_of_node(n1) } -> vector<shared_ptr<Edge<I, W, D>>>;
	{ g.get_edges() } -> vector<shared_ptr<Edge<I, W, D>>>;
	{ g.get_edge(n1, n2)} -> shared_ptr<Edge<I, W, D>>;
	{ g.get_nodes()} -> vector<shared_ptr<Node<I, D>>>;
	{ g.neighbours(n1)} -> vector<shared_ptr<Node<I, D>>>;
	{ g.adjacent(n1, n2)} -> bool;
	{ g.add_node(n1)} -> bool;
	{ g.remove_node(n1)} -> bool;
	{ g.add_edge(e)} -> bool;
	{ g.remove_edge(n1, n2)} -> bool;

};

#endif