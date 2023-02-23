#ifndef UNDERLYING_TREE 
#define UNDERLYING_TREE 1
#include <vector>

template<class C, class E, class V> class LeafNode;

template<class C, class E, class V> class Edge;
template<class C, class E, class V> class Vertex;
template<class C, class E, class V> class Tree;

template<class C, class E, class V> 
class Tree {    
    std::vector<Vertex<C, E, V>> vertices;
    int num_vertices;

    Tree(int num_vertices);
    ~Tree();
    
    public:
    Edge<C, E, V>* add_edge(Vertex<C, E, V>*, Vertex<C, E, V>*);
    Edge<C, E, V>* add_edge(int, int);
    Edge<C, E, V>* find_edge(int, int);

    void del_edge(Edge<C, E, V>*);
    void del_edge_inner(Vertex<C, E, V>*, Edge<C, E, V>* prev, Edge<C, E, V>* next);

    void print_tree();
    void print_edges(Vertex<C, E, V>*);

    std::vector<Vertex<C, E, V>>* get_vertices();
    Vertex<C, E, V>* get_vertex(int id);

};

template<class C, class E, class V> 
class Vertex {
    int id;
    bool exposed;
    Edge<C, E, V>* first_edge;
    
    public:
    Vertex(int id);
    Edge<C, E, V>* get_first_edge();
    void set_first_edge(Edge<C, E, V>*);
    bool has_at_most_one_incident_edge();
    bool is_exposed();
    
};

template<class C, class E, class V> 
class Edge {
    LeafNode<C,E,V>* node;
    Vertex<C,E,V>* endpoints[2];
    Edge<C,E,V>* prev[2];
    Edge<C,E,V>* next[2];

    public:
    Edge(Vertex<C,E,V>* left, Vertex<C,E,V>* right);
    int vertex_is_right(Vertex<C,E,V>* v);
    void set_leaf_node(LeafNode<C,E,V>* l);    
};

#include "underlying_tree_new.hpp"
#include "tree.hpp"


#endif