#ifndef UNDERLYINGTREE_H
#define UNDERLYINGTREE_H

#include <vector>


using namespace std;

template<class T> class LeafNode;

template<class T> class Edge; 
template<class T> class Vertex;
template<class T> class Tree;

template<class T>
class Edge {

    public:
    Edge(Vertex<T>* left, Vertex<T>* right);
    LeafNode<T>* node;
    Vertex<T>* endpoints[2];
    Edge* prev[2];
    Edge* next[2];
    int vertex_is_right(Vertex<T>* v);

    void set_leaf_node(LeafNode<T>* l) { this->node = l; };
};

template<class T>
class Vertex {
    Edge<T> *first_edge;
    
    public: 
    bool exposed;
    int id;
    
    Vertex(int id) {
        this->id = id;
        first_edge = nullptr;
        exposed = false;
    }
    Edge<T>* get_first_edge() { return first_edge; };
    void set_first_edge(Edge<T>* e) { first_edge = e; };
    bool has_at_most_one_incident_edge();
    bool is_exposed() { return exposed; }
};

template<class T>
class Tree {
    
    vector<Vertex<T>> vertices;

    public:
    Tree<T>(int num_vertices);
    ~Tree<T>();
    Edge<T>* add_edge(int u, int v);
    Edge<T>* add_edge(Vertex<T>*,Vertex<T>*);
    void del_edge(Edge<T>* edge);

    void print_tree();

    Vertex<T>* get_vertex(int id) {return &this->vertices[id]; };
    vector<Vertex<T>>* get_vertices() {return &this->vertices; };

    Edge<T>* find_edge(int,int);

    int num_vertices;
    
    private:
    void print_edges(Vertex<T>*);
    void del_edge_inner(Vertex<T>*, Edge<T>* prev, Edge<T>* next);
};

#endif