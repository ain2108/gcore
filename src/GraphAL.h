#ifndef GRAPH_AL_H
#define GRAPH_AL_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <memory>
#include <map>
#include <assert.h>
#include <stdexcept>
#include <utility>

#include "graph_concepts.h"
#include "gcore.h"


using namespace std;

template <typename IdType, typename WeightType, typename DataType>
requires Comparable<IdType> && Numeric<WeightType>
class NodeAL;
template <typename IdType, typename WeightType, typename DataType>
requires Comparable<IdType> && Numeric<WeightType>
class GraphAL;


/************************* GraphAL Class ****************************/
/*! This class provides the adjacency list implementation of a Graph.
Use this implementation when the number of Edges is expected to be proportional
to the number of vertices. In other words, for sparse graphs. */
template <typename IdType, typename WeightType, typename DataType>
requires Comparable<IdType> && Numeric<WeightType>
class GraphAL{
friend class NodeAL<IdType, WeightType, DataType>;
public:

	using id_type = IdType;
	using weight_type = WeightType;
	using data_type = DataType;

	static inline shared_ptr<GraphAL<IdType, WeightType, DataType>> create_graph(){
		shared_ptr<GraphAL<IdType, WeightType, DataType>> p = make_shared<GraphAL<IdType, WeightType, DataType>>();
		return p;
	}

	~GraphAL(){
		for(auto x : adjacency_list){
			if(x == nullptr) continue;
			delete x;
		}
	}

	/* Checks if the node is in the graph */
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
		NodeAL<IdType, WeightType, DataType> * src_p = get_wrapper_p(src);
		NodeAL<IdType, WeightType, DataType> * dst_p = get_wrapper_p(dst);


		if(!adjacent(src_p, dst_p)){
			return false;
		}

		auto it = 
		find_if(src_p->neighbours.begin(), 
		src_p->neighbours.end(),
    	[&](const pair<NodeAL<IdType, WeightType, DataType>*, WeightType>& element)
    		{return element.first == dst_p;});

		if(it == src_p->neighbours.end()){
			return false;
		}

		if(it->second != w){
			return false;
		}
		
		return true;

	}

	/* Returns all the outgoing edges from a given node */
	vector<shared_ptr<Edge<IdType, WeightType, DataType>>> edges_of_node(const shared_ptr<Node<IdType, DataType>> x){
		
		vector<shared_ptr<Edge<IdType, WeightType, DataType>>> temp;
		
		if(!node_in_graph(x))
			throw std::invalid_argument("node not in the graph");

		auto wrapper_p = get_wrapper_p(x);

		/* Go through the list of pairs constructing Edge objects */
		for(auto edge : wrapper_p->neighbours){
			temp.push_back(create_edge(wrapper_p->user_node_p, edge.second, (edge.first)->user_node_p));
		}

		/* SVO here */
		return temp;
	}

	/* Returns a vector of all eges in the graph */
	vector<shared_ptr<Edge<IdType, WeightType, DataType>>> get_edges(){
		
		vector<shared_ptr<Edge<IdType, WeightType, DataType>>> temp;
		for(auto wrapper_p : adjacency_list){
			if(wrapper_p == nullptr) continue;
			
			/* For each edge build an edge object and add it to the temp */
			for(auto edge : wrapper_p->neighbours){
				temp.push_back(create_edge(wrapper_p->user_node_p, edge.second, (edge.first)->user_node_p));
			}
		}

		/* RVO here again */
		return temp;
	}

	/* Returns an edge between two nodes in a graph, if such exists. Throws exp otherwise */
	shared_ptr<Edge<IdType, WeightType, DataType>> get_edge(shared_ptr<Node<IdType, DataType>> src,
		shared_ptr<Node<IdType, DataType>> dst){

		if(!this->adjacent(src, dst))
			throw std::invalid_argument("edge does not exist");

		auto src_wp = get_wrapper_p(src);
		auto dst_wp = get_wrapper_p(dst);

		auto edge_p = get_edge(src_wp, dst_wp);
		return create_edge(src, (*edge_p).second, dst);
	}

	/* Returns the nodes of the graph */
	vector<shared_ptr<Node<IdType, DataType>>> get_nodes(){
		vector<shared_ptr<Node<IdType, DataType>>> temp;
		
		/* Walk through adjacency list and extract node pointers */
		for(auto wrapper_p : adjacency_list){
			if(wrapper_p == nullptr) continue;
			temp.push_back(wrapper_p->user_node_p);
		}

		/* RVO here */
		return temp;
	}	

	/* Function return the neighbours of the node */
	vector<shared_ptr<Node<IdType, DataType>>> neighbours(const shared_ptr<Node<IdType, DataType>> src){

		/* Check if the node is in the graph */
		if(!node_in_graph(src))
			throw std::invalid_argument("node not in the graph");

		/* Get the vector of neighbours that is stored in the wrapper */
		auto edges = get_wrapper_p(src)->neighbours;
		vector<shared_ptr<Node<IdType, DataType>>> temp;

		/* Get the pointers to all the edges */
		for(auto edge : edges){
			temp.push_back((edge.first)->user_node_p);
		}

		/* RVO should be performed her */
		return temp;
	}

	/* Checks if exists a directed edge from src to dst */
	bool adjacent(const shared_ptr<Node<IdType, DataType>> src, const shared_ptr<Node<IdType, DataType>> dst){

		if(!nodes_in_graph(src, dst)){
			throw std::invalid_argument("node not in the graph");
		}

		if(adjacent(get_wrapper_p(src), get_wrapper_p(dst))){
			return true;
		}

		return false;
	}

	/* Adds a node to the graph */
	bool add_node(const shared_ptr<Node<IdType, DataType>> x){
		
		/* Check if the vertex is already in the graph */
		if(node_in_graph(x)){
			throw std::invalid_argument("node already added");
		}

		/* Create the wrapper and add it to adjacency list */
		NodeAL<IdType, WeightType, DataType>* vertex_p = 
			new NodeAL<IdType, WeightType, DataType>(this, x);
		int internal_id = vertex_p->internal_id;

		//TODO: factor this out into a helper?
		if(internal_id == next_unique_id - 1){
			adjacency_list.push_back(vertex_p);
		}else{
			adjacency_list[internal_id] = vertex_p;
		}
		
		/* Add the new mapping into the map */
		id_map[x->get_id()] = vertex_p;
		assert(id_map.find(x->get_id())->second == vertex_p); //ASSERT
		return true;
	}

	/* Removes a node from a graph */
	bool remove_node(const shared_ptr<Node<IdType, DataType>> x){

		/* Check if the vertex is not in the graph */
		if(!node_in_graph(x)){
			throw std::invalid_argument("node not in the graph");
		}

		/* Get the internal id of the wrapper of x */
		int internal_id = id_map.find(x->get_id())->second->internal_id;

		/* Remove all outgoing endes from the vertex */
		adjacency_list[internal_id]->neighbours.clear();

		/* Destroy incoming edges */
		for(auto node_p : adjacency_list){
			if(node_p == nullptr) continue;
			for(int i = 0; i < node_p->neighbours.size(); ++i){
				if((node_p->neighbours[i]).first == get_wrapper_p(x))
					node_p->neighbours.erase(node_p->neighbours.begin() + i);
			}
		}

		/* Delete the wrapper */
		//TODO: discuss the naked delete and smart pointers and etc
		delete adjacency_list[internal_id];

		/* This slot is going t be resused when id recycling kicks in*/
		adjacency_list[internal_id] = nullptr;


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
		/* All we need to do here is to add a pointer
		to n2 to the neighbours of n1*/

		/* First we need to check if the nodes are in the graph */
		if(!nodes_in_graph(src, dst)){
			throw std::invalid_argument("src or dst of the edge not in the graph");
		}

		/* Get hold of the wrappers */
		NodeAL<IdType, WeightType, DataType> * src_p = get_wrapper_p(src);
		NodeAL<IdType, WeightType, DataType> * dst_p = get_wrapper_p(dst);

		/* Check if the edge already exists, if it does, 
		throw an exception */
		if(adjacent(src_p, dst_p)){
			throw std::invalid_argument("edge already exists");
		}

		/* Now we are sure the edge is not already represented,
		so lets just add it to the back of the vector */
		src_p->neighbours.push_back(make_pair(dst_p, w));

		return true;
	}

	/* Removes an edge from the graph */
	bool remove_edge(const shared_ptr<Node<IdType, DataType>> src,
		const shared_ptr<Node<IdType, DataType>> dst){

		if(!nodes_in_graph(src, dst)){
			throw std::invalid_argument("src or dst of the edge not in the graph");
		}

		/* Get hold of the implementation wrappers */
		NodeAL<IdType, WeightType, DataType> * src_p = get_wrapper_p(src);
		NodeAL<IdType, WeightType, DataType> * dst_p = get_wrapper_p(dst);

		/* If they are already not adjacent, nothing to remove*/
		if(!adjacent(src_p, dst_p)){
			throw std::invalid_argument("edge does not exist");
		}

		/* Erase the neighbour element. Should not fail
		since we know they are adjacent. */
		/* #readability */
		src_p->neighbours.erase(find_if(src_p->neighbours.begin(), 
		src_p->neighbours.end(),
    	[&](const pair<NodeAL<IdType, WeightType, DataType>*, WeightType>& element)
    		{return element.first == dst_p;}));

		return true;
	}

	void print_graph() {
		for(auto node_p : this->adjacency_list){
			if(node_p == nullptr) continue;
			cout << (node_p->user_node_p->get_id()) << "-> ";
			for(auto edge : node_p->neighbours){
				cout << "(" << (edge.first)->user_node_p->get_id() << 
				":" << edge.second << "), ";
			}
			cout << endl;
		}
	}

	/* Need to add more constructors such as list initialization here*/
	GraphAL(){
		next_unique_id = 0;
	}
/*	TODO: Not sure how these factor into our library
 *	get_vertex_value(G, x): returns the value associated with the vertex x;
 *	set_vertex_value(G, x, v): sets the value associated with the vertex x to v. */
private:
	
	/* Vector of smart pointers to wrappers. This allows direct access
	to the neighbours of the node from its wrapper. The penalty – vertex
	removal */
	vector<NodeAL<IdType, WeightType, DataType>*> adjacency_list;
	// Need this map to go from Node -> NodeAL
	map<IdType, NodeAL<IdType, WeightType, DataType>*> id_map;

	/* Same idea as for GraphAM here */
	long next_unique_id;
	vector<int> free_ids;

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
	NodeAL<IdType, WeightType, DataType>* get_wrapper_p(const shared_ptr<Node<IdType, DataType>> x){
		return id_map.find(x->get_id())->second;
	}

	bool adjacent(const NodeAL<IdType, WeightType, DataType> * src_p, 
		const NodeAL<IdType, WeightType, DataType> * dst_p){

		/*Lets findout if dst_p in in neghbours of src_p */
		auto it = 
		find_if(src_p->neighbours.begin(), 
		src_p->neighbours.end(),
    	[&](const pair<NodeAL<IdType, WeightType, DataType>*, WeightType>& element)
    		{return element.first == dst_p;});

		if(it != src_p->neighbours.end())
			return true;
		
		return false;
	}

	auto get_edge(const NodeAL<IdType, WeightType, DataType> * src_p, 
		const NodeAL<IdType, WeightType, DataType> * dst_p){

		/*Lets findout if dst_p in in neghbours of src_p */
		auto it = 
		find_if(src_p->neighbours.begin(), 
		src_p->neighbours.end(),
    	[&](const pair<NodeAL<IdType, WeightType, DataType>*, WeightType>& element)
    		{return element.first == dst_p;});

		if(it != src_p->neighbours.end())
			return it;
		
		//TODO: FIX
		return it;
	}
};

/************************* NodeAL Class ****************************/

/* Adjacency list implementation node wrapper */
/*! NodeAL is an implementation dependent wrapper around Node object. These wrappers are actually
what represents a Node in a given graph.  */
template <typename IdType, typename WeightType, typename DataType>
requires Comparable<IdType> && Numeric<WeightType>
class NodeAL{
friend class GraphAL<IdType, WeightType, DataType>;
public:

using id_type = IdType;
using weight_type = WeightType;
using data_type = DataType;
/* Need to think about other constructors here a little */
	/* Existance of NodeAL only makes sense in the context of a graph */
	NodeAL(GraphAL<IdType, WeightType, DataType>* graph,
		const shared_ptr<Node<IdType, DataType>> user_node){
		/* Get the new internal id */
		internal_id = graph->get_new_id();

		user_node_p = user_node;
	}

	int heuristic_function() 
	{
		return 10;
	}

private:
	/* Used to boost the performance of implementation graph*/
	long internal_id;

	/* This way, we avoid indexing into the adjacency list */
	/* For now, lets store the Weight by value */
	vector<pair<NodeAL<IdType, WeightType, DataType>*, WeightType>> neighbours;
	
	/* Pointer to the user created node */
	shared_ptr<Node<IdType, DataType>> user_node_p;


	bool operator==(const NodeAL& rhs){
		return (*(this->user_node_p))==(*(rhs.user_node_p));
	}
};

#endif
