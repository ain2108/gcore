#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include "graph_concepts.h"
#include "Edge.h"
#include "Node.h"
#include "GraphAL.h"
#include "GraphAM.h"

using namespace std;

/* THE TODO LIST 
1) Fix the move, assignment adn copy constructors of Node.
Remember, it should not be possible to make a copy of a Node.
User must go through create_node function*

/*! \file */

/* CREATION functions for user types */
/*! Function creates an instance of a Node. Returns a shared_ptr back to the user. User should
use auto keyword to catch this pointer */
template <typename IdType, typename DataType>
requires Comparable<IdType>
inline shared_ptr<Node<IdType, DataType>> create_node(IdType id, DataType* data){
	return Node<IdType, DataType>::create_node(id, data);
}

/*! Function creates an instance of a Edge. Returns a shared_ptr back to the user. User should
use auto keyword to catch this pointer */
template <typename IdType, typename WeightType, typename DataType>
requires Comparable<IdType> && Numeric<WeightType>
inline shared_ptr<Edge<IdType, WeightType, DataType>> create_edge(
	const shared_ptr<Node<IdType, DataType>> src,
	const WeightType w,
	const shared_ptr<Node<IdType, DataType>> dst){
	return Edge<IdType, WeightType, DataType>::create_edge(src, w, dst); 
}

//TODO: Add the concepts for GraphType, IdType and WeightType
/*! Function creates an instance of a Graph. Returns a shared_ptr back to the user. User should
use auto keyword to catch this pointer */
template <typename I, typename W, typename D, template <typename, typename, typename> typename GraphType>
requires Comparable<I> && Numeric<W> && IsGraph<I, W, D, GraphType>
inline shared_ptr<GraphType<I, W, D>> create_graph(){
	return GraphType<I, W, D>::create_graph();
}


/*! Function compares two edges by the (<) if the Weight */
template <typename IdType, typename Weight, typename DataType>
requires Comparable<IdType> && Numeric<Weight>
inline bool compare_edges(const shared_ptr<Edge<IdType, Weight, DataType>> lhs,
	const shared_ptr<Edge<IdType, Weight, DataType>> rhs){
	return lhs->get_weight() < rhs->get_weight();
}

/* PRINTERS */
/*! A simple printer to print a vector of Nodes */
template <typename IdType, typename DataType>
void print_nodes(vector<shared_ptr<Node<IdType, DataType>>>& node_ps){
	for(auto node_p : node_ps){
		node_p->print_node();
		cout << " ";
	}
	cout << endl;
}

/*! A simple printer to print a vector of Edges */
template <typename IdType, typename WeightType, typename DataType>
requires Comparable<IdType> && Numeric<WeightType>
void print_edges(vector<shared_ptr<Edge<IdType, WeightType, DataType>>>& edges){
	for(auto edge : edges){
		edge->print_edge();
		cout << " ";
	}
	cout << endl;
}

/*! Utility function that adds all Nodes in the input vector to the given graph */
template <typename I, typename W, typename D, template <typename, typename, typename> typename GraphType>
requires Comparable<I> && Numeric<W>
inline void add_nodes(shared_ptr<GraphType<I, W, D>> g, const vector<shared_ptr<Node<I, D>>>& node_ps){

	for(auto node_p : node_ps){
		add_node(g, node_p);
	}

	return;
}

/*! Utility function that adds all Edges in the input vector to the given graph */
template <typename I, typename W, typename D, template <typename, typename, typename> typename GraphType>
requires Comparable<I> && Numeric<W>
inline void add_edges(shared_ptr<GraphType<I, W, D>> g, const vector<shared_ptr<Edge<I, W, D>>>& edge_ps){

	for(auto edge_p : edge_ps){
		add_edge(g, edge_p);
	}

	return;
}

/*                     IMPLEMENTATION WRAPPERS                          */
/* These are the wrappers around the implementation dependent function. */

template <typename I, typename W, typename D>
using EdgeSP = shared_ptr<Edge<I, W, D>>;
template <typename I, typename D>
using NodeSP = shared_ptr<Node<I, D>>;
template <typename I, typename W, typename D, template <typename, typename, typename> typename GraphType>
using GraphSP = shared_ptr<GraphType<I, W, D>>;


/*! Implementation independent function checks if the Node x is a member of the Graph graph. */ 
template <typename I, typename W, typename D, template <typename, typename, typename> typename GraphType>
requires Comparable<I> && Numeric<W>
inline bool has_node(const GraphSP<I, W, D, GraphType> graph, const NodeSP<I, D> x){
	return graph->has_node(x);
}

/*! Implementation independent function checks if the Edge e is a member of the Graph graph. */ 
template <typename I, typename W, typename D, template <typename, typename, typename> typename GraphType>
requires Comparable<I> && Numeric<W>
inline bool has_edge(const GraphSP<I, W, D, GraphType> graph, EdgeSP<I, W, D> e){
	return graph->has_edge(e->get_src(), e->get_weight(), e->get_dst());
}

/*! Implementation independent function checks if the Edge e is a member of the Graph graph. */ 
template <typename I, typename W, typename D, template <typename, typename, typename> typename GraphType>
requires Comparable<I> && Numeric<W>
inline bool has_edge(const GraphSP<I, W, D, GraphType> graph, const NodeSP<I, D> src, const W w, 
		const NodeSP<I, D> dst){
	return graph->has_edge(src, w, dst);
}

/*! Implementation independent function returns a vector of Edges of Node x in graph. */ 
template <typename I, typename W, typename D, template <typename, typename, typename> typename GraphType>
requires Comparable<I> && Numeric<W>
inline vector<EdgeSP<I, W, D>> edges_of_node(const GraphSP<I, W, D, GraphType> graph,
	const NodeSP<I, D> x){
	return graph->edges_of_node(x);
}

/*! Implementation independent function returns a vector of all Edges in graph. */ 
template <typename I, typename W, typename D, template <typename, typename, typename> typename GraphType>
requires Comparable<I> && Numeric<W>
inline vector<EdgeSP<I, W, D>> get_edges(const GraphSP<I, W, D, GraphType> graph){
	return graph->get_edges();
}

/*! Implementation independent function returns the Edge object that representing the Edge between src and
dst in graph */ 
template <typename I, typename W, typename D, template <typename, typename, typename> typename GraphType>
requires Comparable<I> && Numeric<W>
inline EdgeSP<I, W, D> get_edge(const GraphSP<I, W, D, GraphType> graph, NodeSP<I, D> src, NodeSP<I, D> dst){
	return graph->get_edge(src, dst);
}

/*! Implementation independent function returns a vector of all Nodes in graph. */ 
template <typename I, typename W, typename D, template <typename, typename, typename> typename GraphType>
requires Comparable<I> && Numeric<W>
inline vector<NodeSP<I, D>> get_nodes(const GraphSP<I, W, D, GraphType> graph){
	return graph->get_nodes();
}

/*! Implementation independent function returns a vector of Nodes adjacent to node src in graph. */ 
template <typename I, typename W, typename D, template <typename, typename, typename> typename GraphType>
requires Comparable<I> && Numeric<W>
inline vector<NodeSP<I, D>> neighbours(const GraphSP<I, W, D, GraphType> graph, const NodeSP<I, D> src){
	return graph->neighbours(src);
}

/*! Implementation independent function returns boolean value reflecting the adjacency of src and dst in graph */ 
template <typename I, typename W, typename D, template <typename, typename, typename> typename GraphType>
requires Comparable<I> && Numeric<W>
inline bool adjacent(const GraphSP<I, W, D, GraphType> graph, 
	const NodeSP<I, D> src, 
	const NodeSP<I, D> dst){
	return graph->adjacent(src, dst);
}

/*! Implementation independent function adds a Node to the graph. Exception if Node already added. */ 
template <typename I, typename W, typename D, template <typename, typename, typename> typename GraphType>
requires Comparable<I> && Numeric<W>
inline bool add_node(const GraphSP<I, W, D, GraphType> graph, const NodeSP<I, D> x){
	return graph->add_node(x);
}

/*! Implementation independent function removes a Node to the graph. Exception if Node already removed. */ 
template <typename I, typename W, typename D, template <typename, typename, typename> typename GraphType>
requires Comparable<I> && Numeric<W>
inline bool remove_node(const GraphSP<I, W, D, GraphType> graph, const NodeSP<I, D> x){
	return graph->remove_node(x);
}


/*! Implementation independent function adds an Edge to the graph. Exception if either src or dst are not 
part of the graph, or if they are already adjacent*/ 
template <typename I, typename W, typename D, template <typename, typename, typename> typename GraphType>
requires Comparable<I> && Numeric<W>
inline bool add_edge(const GraphSP<I, W, D, GraphType> graph, const NodeSP<I, D> src, const W w, const NodeSP<I, D> dst){
	return graph->add_edge(src, w, dst);
}

/*! Implementation independent function adds an Edge to the graph. Exception if either src or dst are not 
part of the graph, or if they are already adjacent*/ 
template <typename I, typename W, typename D, template <typename, typename, typename> typename GraphType>
requires Comparable<I> && Numeric<W>
inline bool add_edge(const GraphSP<I, W, D, GraphType> graph, const EdgeSP<I, W, D> e){
	return graph->add_edge(e->get_src(), e->get_weight(), e->get_dst());
}

/*! Implementation independent function removes an Edge to the graph. Exception if either src or dst are not 
part of the graph, or if the two Nodes are already disjoint */ 
template <typename I, typename W, typename D, template <typename, typename, typename> typename GraphType>
requires Comparable<I> && Numeric<W>
bool remove_edge(const GraphSP<I, W, D, GraphType> graph, const NodeSP<I, D> src, const NodeSP<I, D> dst){
	return graph->remove_edge(src, dst);
}

/* OPERATORS ON NODE SPs */
/*! The operator that compares the shared_pointers to Nodes, so the user does not have to worry about
the exact details. */
template <typename I, typename D>
requires Comparable<I>
inline bool operator==(const NodeSP<I, D>& lhs, const NodeSP<I, D>& rhs){ 
	return (lhs->get_id() == rhs->get_id());
}

/*! Same idea as for (==) operator. */
template <typename I, typename D>
requires Comparable<I>
inline bool operator!=(const NodeSP<I, D>& lhs, const NodeSP<I, D>& rhs){ 
	return !(lhs == rhs);
}

/* OPERATORS EDGE SPs */
/*! The operator that compares the shared_pointers to Edges, so the user does not have to worry about
the exact details. */
template <typename I, typename W, typename D>
requires Comparable<I> && Numeric<W>
inline bool operator==(const EdgeSP<I, W, D> lhs,
	const EdgeSP<I, W, D> rhs){
	return (lhs->get_weight() == rhs->get_weight())
		&& (lhs->get_src() == rhs->get_src())
		&& (lhs->get_dst() == rhs->get_dst());
}

/*! Same idea as for (==) operator. */
template <typename I, typename W, typename D>
requires Comparable<I> && Numeric<W>
inline bool operator!=(const EdgeSP<I, W, D> lhs,
	const EdgeSP<I, W, D> rhs){
	return !(lhs == rhs);
}

#endif
