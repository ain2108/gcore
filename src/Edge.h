#ifndef EDGE_H
#define EDGE_H

template <typename IdType, typename DataType>
//requires Comparable<IdType> 
class Node;



/*! This class represents an Edge in the graph. And Edge has a src, a dst and a weight
	associated with it.
*/
template <typename IdType, typename WeightType, typename DataType>
//requires Comparable<IdType> && Numeric<WeightType>
class Edge{
public:

	/* Same as for edges, we don't want these to be copied by accident */
	Edge(Edge<IdType, WeightType, DataType> const&) = delete;
	void operator=(Edge<IdType, WeightType, DataType> const&) = delete;

	/*! The function returns the shared pinter to a source node */
	inline const shared_ptr<Node<IdType, DataType>> get_src() const {
		return src;
	}

	/*! The function returns the shared pinter to a destination node */
	inline const shared_ptr<Node<IdType, DataType>> get_dst() const {
		return dst;
	}

	/*! The function returns the weight of the Edge by value*/
	inline WeightType get_weight() const {
		return w;
	}

	/*! The function sets the weight of the Edge by value*/
	inline WeightType set_weight(WeightType new_w){
		w = new_w;
	}

	/*! The function creates an Edge object, and returns a smart pointer to it. Function used by the
	user creation function */
	inline static shared_ptr<Edge<IdType, WeightType, DataType>> create_edge(
		const shared_ptr<Node<IdType, DataType>> src,
		const WeightType w,
		const shared_ptr<Node<IdType, DataType>> dst){
		shared_ptr<Edge<IdType, WeightType, DataType>> p  = make_shared<Edge<IdType, WeightType, DataType>>(src, w, dst);
		return p;
	}

	/*! This is a simple printed provided only for debugging purposes */
	void print_edge() const {
		cout << "(" << this->src->get_id() << ", " << w << ", " << this->dst->get_id() << ")";
	}

	Edge(const shared_ptr<Node<IdType, DataType>> src, WeightType w,
	 const shared_ptr<Node<IdType, DataType>> dst){
		this->src = src;
		this->dst = dst;
		this->w = w;
	}

private:
	shared_ptr<Node<IdType, DataType>> src;
	shared_ptr<Node<IdType, DataType>> dst;
	WeightType w;
};

#endif