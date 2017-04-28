#ifndef NODE_H
#define NODE_H


template <typename IdType, typename WeightType, typename DataType>
//requires Comparable<IdType> && Numeric<WeightType>
class Edge;


/*! The Node class represents a Node. Create Nodes by calling the appropriate creating funtion.
After Nodes are created, one has to add them to the graph object (track them if you will). */
template <typename IdType, typename DataType>
//requires Comparable<IdType>
class Node{

private:
	/*! This id is seen by a Graph as unique. If the user tries to add two different instances of 
	Node with the same id to a graph, an exception will be caused */
	IdType id;
	
	/*! A pointer to the user owned object associated with this node */
	DataType* data;

public:

	using id_type = IdType;
	using data_type = DataType;

	Node(IdType id, DataType* data){
		//cout << id << " created\n";
		this->id = id;
		this->data = data;
	}

	/* We want to enforce existance of these objects in singular form,
	if somebody tries to copy them, we shall not let them do so!*/
	Node(Node<IdType, DataType> const&) = delete;
	void operator=(Node<IdType, DataType> const&) = delete;

	/* We can add other bookeeping field here to boost
	performace of some algorithms */

	/*! Function returns the id of a Node */
	inline IdType get_id() const {
		return this->id;
	}

	/*! Function returns the pointer to the object associated with the given node */
	inline DataType* get_data(){
		return this->data;
	}

	/*! Sets a pointer to a user owned object associated with the data */
	inline void set_data(DataType * data){
		this->data = data;
	}

	inline bool operator==(const Node& rhs){
		return (this->id==rhs.id);
	}

	inline void print_node() const {
		cout << get_id();
	}

	inline static shared_ptr<Node<IdType, DataType>> create_node(IdType id, DataType* data){
		shared_ptr<Node<IdType, DataType>> p = make_shared<Node<IdType, DataType>>(id, data);
		return p;
	}

};



#endif
