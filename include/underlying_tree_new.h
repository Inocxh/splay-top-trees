#ifndef UNDERLYING_TREE 
#define UNDERLYING_TREE 1
#include <vector>
#include <variant>

template<class C, class E, class V> class LeafNode;

template<class C, class E, class V> class Edge;
template<class C, class E, class V> class Vertex;
template<class C, class E, class V> class Tree;

//This is an empty type
//A unit class like None {} would have size one and would take up space for no reason. 
//Void also creates problems as it cannot be instansiated
typedef std::monostate None;

template<class C = None, class E = None, class V = None> 
class Tree {    
    std::vector<Vertex<C, E, V>> vertices;

    public:
    Tree(int num_vertices);
    ~Tree();
    
    Edge<C, E, V>* add_edge(Vertex<C, E, V>*, Vertex<C, E, V>*);
    Edge<C, E, V>* add_edge(int, int);
    Edge<C, E, V>* find_edge(int, int);

    void del_edge(Edge<C, E, V>*);
    void del_edge_inner(Vertex<C, E, V>*, Edge<C, E, V>* prev, Edge<C, E, V>* next);

    void print_tree();
    void print_edges(Vertex<C, E, V>*);

    int get_size();
    std::vector<Vertex<C, E, V>>* get_vertices();
    Vertex<C, E, V>* get_vertex(int id);

};

//Only exists for "empty base class" optimization!
//If V is an empty type like None the V field would still take up one byte
// as per c++ standard. However this does not apply if Vertex inherits the field from another class.
template<class V>
struct VHolder {
    V vertex_data;
};

template<class C = None, class E = None, class V = None>  
class Vertex : VHolder<V> {
    friend class Tree<C,E,V>;

    int id;
    bool exposed;
    Edge<C, E, V>* first_edge;
    
    public:
    Vertex(int id);
    Edge<C, E, V>* get_first_edge();
    void set_first_edge(Edge<C, E, V>*);
    bool has_at_most_one_incident_edge();
    bool is_exposed();

    V get_data();
};

template<class E>
struct EHolder {
    E edge_data;
};

template<class C = None, class E = None, class V = None>  
class Edge : EHolder<E>{
    friend class Tree<C,E,V>;

    Vertex<C,E,V>* endpoints[2];
    Edge<C,E,V>* prev[2];
    Edge<C,E,V>* next[2];

    LeafNode<C,E,V>* node;

    public:
    Edge(Vertex<C,E,V>* left, Vertex<C,E,V>* right);
    int vertex_is_right(Vertex<C,E,V>* v);
    void set_leaf_node(LeafNode<C,E,V>* l);    
    Vertex<C,E,V>* get_endpoints(int);
    Edge<C,E,V>* get_next(int);
    Edge<C,E,V>* get_prev(int);

    E get_data();
};

#include "underlying_tree_new.hpp"
#include "tree.hpp"


#endif