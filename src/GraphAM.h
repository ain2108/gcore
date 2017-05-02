#ifndef GRAPH_AM_H
#define GRAPH_AM_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <memory>
#include <map>
#include <assert.h>
#include <stdexcept>
#include <utility>

//#include "graph_concepts.h"
#include "gcore.h"
#include "SquareMatrix.h"


using namespace std;

template <typename IdType, typename WeightType, typename DataType>
class NodeAM;
template <typename IdType, typename WeightType, typename DataType>
//requires Comparable<IdType>
class GraphAM;


/************************* GraphAM Class ****************************/
/*! This class provides the adjacency matrix implementation for a Graph.
Use this implementation when the graph is expected to be dense. */
template <typename IdType, typename WeightType, typename DataType>
//requires Comparable<IdType>
class GraphAM{
friend class NodeAM<IdType, WeightType, DataType>;
public:

	using id_type = IdType;
	using weight_type = WeightType;
	using data_type = DataType;

	static inline shared_ptr<GraphAM<IdType, WeightType, DataType>> create_graph(){
		shared_ptr<GraphAM<IdType, WeightType, DataType>> p = make_shared<GraphAM<IdType, WeightType, DataType>>();
		return p;
	}

	~GraphAM(){
		for(auto wrap_map_entry : wrapper_map){
			delete wrap_map_entry.second;
		}
	}

	// /* Checks if the node is in the graph */
	inline bool has_node(const shared_ptr<Node<IdType, DataType>> x){
		return node_in_graph(x);
	}


	bool has_edge(const shared_ptr<Node<IdType, DataType>> src, const WeightType w, 
		const shared_ptr<Node<IdType, DataType>> dst){

		/* First we need to check if the nodes are in the graph */
		if(!nodes_in_graph(src, dst)){
			return false;
		}

		/* Get hold of the wrappers */
		auto src_p = get_wrapper_p(src);
		auto dst_p = get_wrapper_p(dst);

		if(!adjacent(src_p, dst_p)){
			return false;
		}

		auto weight = adjacency_matrix.get_entry(src_p->internal_id, dst_p->internal_id);
		if(weight != w)
			return false;

		return true;
	}

	/* Returns all the outgoing edges from a given node */
	vector<shared_ptr<Edge<IdType, WeightType, DataType>>> edges_of_node(const shared_ptr<Node<IdType, DataType>> src){
		
		/* Fill in with edges */
		vector<shared_ptr<Edge<IdType, WeightType, DataType>>> temp;

		auto row = get_wrapper_p(src)->internal_id;

		/* Get the indices of the entries that are not zero in the table */
		auto indices = adjacency_matrix.non_zero_entries(row);

		/* For each of these indices, get hold of the information about the edge,
		construct the edge and add it to the return vector*/
		for(auto column_index : indices){
			temp.push_back(
				create_edge(src, adjacency_matrix.get_entry(row, column_index), 
				wrapper_map[column_index]->user_node_p));
		}

		return temp;
	}

	/* Returns a vector of all eges in the graph */
	vector<shared_ptr<Edge<IdType, WeightType, DataType>>> get_edges(){
		vector<shared_ptr<Edge<IdType, WeightType, DataType>>> temp;
		
		auto nodes = get_nodes();
		for(auto src : nodes){
			auto edges = edges_of_node(src);
			temp.insert(temp.end(), edges.begin(), edges.end());
		}

		return temp;
	}

	/* Returns an edge between two nodes in a graph, if such exists. Throws exp otherwise */
	shared_ptr<Edge<IdType, WeightType, DataType>> get_edge(shared_ptr<Node<IdType, DataType>> src,
		shared_ptr<Node<IdType, DataType>> dst){
		
		if(!this->adjacent(src, dst))
			throw std::invalid_argument("edge does not exist");

		auto src_p = get_wrapper_p(src);
		auto dst_p = get_wrapper_p(dst);

		auto weight = adjacency_matrix.get_entry(src_p->internal_id, dst_p->internal_id);
		return create_edge(src, weight, dst);
	}

	/* Returns the nodes of the graph */
	vector<shared_ptr<Node<IdType, DataType>>> get_nodes(){
		vector<shared_ptr<Node<IdType, DataType>>> temp;
		for(auto wrap_map_entry : wrapper_map){
			temp.push_back(wrap_map_entry.second->user_node_p);
		}
		return temp;
	}

	/* Function return the neighbours of the node */
	vector<shared_ptr<Node<IdType, DataType>>> neighbours(const shared_ptr<Node<IdType, DataType>> src){

		/* Get the indices of the entries that are not zero in the table */
		auto indices = adjacency_matrix.non_zero_entries(get_wrapper_p(src)->internal_id);
		vector<shared_ptr<Node<IdType, DataType>>> temp;

		/* For each of these indices, get hold of the Node associated with them */
		for(auto column_index : indices){
			temp.push_back(wrapper_map[column_index]->user_node_p);
		}

		return temp;
	}

	/* Checks if exists a directed edge from src to dst */
	bool adjacent(const shared_ptr<Node<IdType, DataType>> src, const shared_ptr<Node<IdType, DataType>> dst){

		/* First we need to check if the nodes are in the graph */
		if(!nodes_in_graph(src, dst)){
			throw std::invalid_argument("src or dst of the edge not in the graph");
		}

		/* Get hold of the wrappers */
		auto src_p = get_wrapper_p(src);
		auto dst_p = get_wrapper_p(dst);

		return adjacent(src_p, dst_p);
	}

	/* Adds a node to the graph */
	bool add_node(const shared_ptr<Node<IdType, DataType>> x){

		/* Check if the vertex is already in the graph */
		if(node_in_graph(x)){
			throw std::invalid_argument("node already added");
		}

		/* Create the wrapper and add it to adjacency list */
		NodeAM<IdType, WeightType, DataType>* vertex_p = 
			new NodeAM<IdType, WeightType, DataType>(this, x);
		int internal_id = vertex_p->internal_id;

		/* Add the entry to the wrapper map */
		wrapper_map[internal_id] = vertex_p;

		/* Update the knowledge about highest active id */
		if(internal_id > highest_active_id){
			highest_active_id = internal_id;
			adjacency_matrix.inc_used();
		}

		/* Check if our adjacency matrix needs resizing */
		if(adjacency_matrix.full())
			adjacency_matrix.resize();

		/* Add the new mapping into the map */
		id_map[x->get_id()] = vertex_p;
		assert(id_map.find(x->get_id())->second == vertex_p); //ASSERT
		return true;

	}

	/* Removes a node from a graph */
	bool remove_node(const shared_ptr<Node<IdType, DataType>> x){

		/* Check if the vertex is already in the graph */
		if(!node_in_graph(x)){
			throw std::invalid_argument("node not in the graph");
		}

		auto wrapper_p = get_wrapper_p(x);
		int internal_id = wrapper_p->internal_id;

		/* Erase the row an the column */
		adjacency_matrix.zero_row(internal_id);
		adjacency_matrix.zero_column(internal_id);

		/* Delete the entry from the wrapper map */
		wrapper_map.erase(wrapper_p->internal_id);
		/* Delete the wrapper */
		delete wrapper_p;

		/* Get the unique id back for id recycling */
		return_id(internal_id);

		/* Erase the vertex from the wrapper map */
		id_map.erase(x->get_id());

		return true;
	}

	/* Adds an edge to the graph */
	inline bool add_edge(shared_ptr<Edge<IdType, WeightType, DataType>> e){
		return add_edge(e->get_src(), e->get_weight(), e->get_dst());
	}

	bool add_edge(const shared_ptr<Node<IdType, DataType>> src, const WeightType w, 
		const shared_ptr<Node<IdType, DataType>> dst){

		/* First we need to check if the nodes are in the graph */
		if(!nodes_in_graph(src, dst)){
			throw std::invalid_argument("src or dst of the edge not in the graph");
		}

		/* Get hold of the wrappers */
		auto src_p = get_wrapper_p(src);
		auto dst_p = get_wrapper_p(dst);

		/* Get the entry of the adjacency matrix to access */
		int row = src_p->internal_id;
		int column = dst_p->internal_id;

		/* Check if the edge already exists, if it does, 
		throw an exception */
		if(adjacent(src_p, dst_p)){
			throw std::invalid_argument("edge already exists");
		}

		/* If does not exist, lets add it by adding the weight */
		adjacency_matrix.set_entry(row, column, w);

		return true;
	}

	/* Removes an edge from the graph */
	bool remove_edge(const shared_ptr<Node<IdType, DataType>> src,
		const shared_ptr<Node<IdType, DataType>> dst){

		/* First we need to check if the nodes are in the graph */
		if(!nodes_in_graph(src, dst)){
			throw std::invalid_argument("src or dst of the edge not in the graph");
		}

		/* Get hold of the wrappers */
		auto src_p = get_wrapper_p(src);
		auto dst_p = get_wrapper_p(dst);

		/* Get the entry of the adjacency matrix to access */
		int row = src_p->internal_id;
		int column = dst_p->internal_id;

		/* Check if the edge already exists, if it does, 
		throw an exception */
		if(!adjacent(src_p, dst_p)){
			throw std::invalid_argument("nodes not adjacent");
		}

		/* If does not exist, lets add it by adding the weight */
		adjacency_matrix.zero_entry(row, column);

	}

	void print_graph(){
		adjacency_matrix.print_matrix();
	}

	/* Need to add more constructors such as list initialization here*/
	GraphAM(){
		next_unique_id = 0;
		//adjacency_matrix = SquareMatrix<WeightType>();
	}

private:
	
	/* Vector of smart pointers to wrappers. This allows direct access
	to the neighbours of the node from its wrapper. The penalty – vertex
	removal */
	SquareMatrix<WeightType> adjacency_matrix;

	/* Node id to the wrapper */
	map<IdType, NodeAM<IdType, WeightType, DataType>*> id_map;

	/* From internal id to the wrapper */
	map<int, NodeAM<IdType, WeightType, DataType>*> wrapper_map;

	/* Same idea as for GraphAl here */
	long next_unique_id;
	vector<int> free_ids;

	int highest_active_id;

	/* Function hands out the new id when a vertex is added*/
	inline long get_new_id(){
		if(free_ids.empty()){
			return next_unique_id++;
		}else{
			int temp = free_ids.back();
			free_ids.pop_back();
			return temp;
		}
	}

	inline bool return_id(int internal_id){
		free_ids.push_back(internal_id);
	}

	inline bool node_in_graph(const shared_ptr<Node<IdType, DataType>> x){
		if(id_map.find(x->get_id()) != id_map.end()){
			return true;
		}
		return false;
	}

	inline bool nodes_in_graph(const shared_ptr<Node<IdType, DataType>> x,
		const shared_ptr<Node<IdType, DataType>> y){
		return (node_in_graph(x) && node_in_graph(y));
	}

	/* NOTE: Assumes x is in the graph */
	NodeAM<IdType, WeightType, DataType>* get_wrapper_p(const shared_ptr<Node<IdType, DataType>> x){
		return id_map.find(x->get_id())->second;
	}

	bool adjacent(const NodeAM<IdType, WeightType, DataType> * src_p, 
		const NodeAM<IdType, WeightType, DataType> * dst_p){

		if(adjacency_matrix.get_entry(src_p->internal_id, dst_p->internal_id) == 0)
			return false;
		
		return true;
	}
};

/************************* NodeAM Class ****************************/

/* Adjacency list implementation node wrapper */
/*! NodeAL is an implementation dependent wrapper around Node object. These wrappers are actually
what represents a Node in a given graph.  */
template <typename IdType, typename WeightType, typename DataType>
class NodeAM{
friend class GraphAM<IdType, WeightType, DataType>;
public:

	/* Need to think about other constructors here a little */
	/* Existance of NodeAL only maks sense in the context of a graph */
	NodeAM(GraphAM<IdType, WeightType, DataType>* graph,
		const shared_ptr<Node<IdType, DataType>> user_node){
		/* Get the new internal id */
		internal_id = graph->get_new_id();

		user_node_p = user_node;
	}

private:
	/* Used to boost the performance of implementation graph*/
	long internal_id;
	
	/* Pointer to the user created node */
	shared_ptr<Node<IdType, DataType>> user_node_p;

	bool operator==(const NodeAM& rhs){
		return (*(this->user_node_p))==(*(rhs.user_node_p));
	}
};

#endif