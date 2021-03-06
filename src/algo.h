#ifndef ALGO_H
#define ALGO_H

#include "gcore.h"
#include <list>

/*! \file */

template <typename I, typename W, typename D>
using EdgeSP = shared_ptr<Edge<I, W, D>>;
template <typename I, typename D>
using NodeSP = shared_ptr<Node<I, D>>;
template <typename I, typename W, typename D, template <typename, typename, typename> typename GraphType>
using GraphSP = shared_ptr<GraphType<I, W, D>>;

/*! The DFS routine. Returns an instance of Graph that represnets a tree 
created by the dfs from Node root to every other Node in Graph graph*/
template <typename I, typename W, typename D, template <typename, typename, typename> typename GraphType>
GraphSP<I, W, D, GraphType> dfs(GraphSP<I, W, D, GraphType> graph, 
	NodeSP<I, D> root){

	/* Initilization */
	list<NodeSP<I, D>> temp;
	map<I, bool> discovered_map;
	map<I, EdgeSP<I, W, D>> predecessor;
	auto tree = create_graph<I, W, D, GraphType>();


	/* Push the root */
	temp.push_front(root);

	while(!temp.empty()){
		auto x = temp.front();
		temp.pop_front();

		if (discovered_map.find(x->get_id()) == discovered_map.end()){
			
			/* Mark x as discovered */
			discovered_map[x->get_id()] = true;

			/* Add the node to the tree we are building */
			add_node(tree, x);
				
			/* Add the edge representig how we got there */
			auto pred_p = predecessor.find(x->get_id());

			/* If prefeccessor exists, add the edge to the graph */
			if(pred_p != predecessor.end()){
				add_edge(tree, (*pred_p).second );
			}
		}

		/* Add all the neighbours to the que */
		for(auto y : graph->neighbours(x)){
			if (discovered_map.find(y->get_id()) == discovered_map.end()){
				temp.push_front(y);
				predecessor[y->get_id()] = get_edge(graph, x, y);
			}
		}
		//}
	}
	return tree;
}

/*! The BFS routine. Returns an instance of Graph that represnets a tree 
created by the dfs from Node root to every other Node in Graph graph*/
template <typename I, typename W, typename D, template <typename, typename, typename> typename GraphType>
GraphSP<I, W, D, GraphType> bfs(GraphSP<I, W, D, GraphType> graph, NodeSP<I, D> root){

	/* Initilization */
	list<NodeSP<I, D>> q;
	map<I, bool> discovered_map;
	map<I, EdgeSP<I, W, D>> predecessor;
	auto tree = create_graph<I, W, D, GraphType>();

	/* Push the root */
	q.push_front(root);

	while(!q.empty()){
		auto x = q.back();
		q.pop_back();
			
		/* Mark x as discovered */
		discovered_map[x->get_id()] = true;

		/* Add the node to the tree we are building */
		add_node(tree, x);
			
		/* Add the edge representig how we got there */
		auto pred_p = predecessor.find(x->get_id());

		/* If prefeccessor exists, add the edge to the graph */
		if(pred_p != predecessor.end()){
			add_edge(tree, (*pred_p).second );
		}

		/* Add all the neighbours to the que */
		for(auto y : graph->neighbours(x)){
			if (discovered_map.find(y->get_id()) == discovered_map.end()){
				q.push_front(y);
				discovered_map[y->get_id()] = true;
				predecessor[y->get_id()] = get_edge(graph, x, y);
			}
		}
	}
	cout << endl;
	return tree;
}


#endif