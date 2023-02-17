#ifndef UNDERLYINGTREE_H
#define UNDERLYINGTREE_H

#include <vector>


using namespace std;

class LeafNode;

class Edge; 
class Vertex;
class Tree;

class Edge {
    public:
    Edge(Vertex* left, Vertex* right, int weight);
    int weight;
    LeafNode* node;
    Vertex* endpoints[2];
    Edge* prev[2];
    Edge* next[2];
    int vertex_is_right(Vertex* v);
};

class Vertex {
    Edge *first_edge;
    
    public: 
    bool exposed;
    int id;
    
    Vertex(int id) {
        this->id = id;
        first_edge = nullptr;
        exposed = false;
    }
    Edge* get_first_edge() { return first_edge; };
    void set_first_edge(Edge* e) { first_edge = e; };
    bool has_at_most_one_incident_edge();
    bool is_exposed() { return exposed; }
};


class Tree {
    
    vector<Vertex> vertices;

    public:
    Tree(int num_vertices);
    ~Tree();
    Edge* add_edge(int u, int v, int weight);
    void del_edge(Edge* edge);

    void print_tree();

    Vertex* get_vertex(int id) {return &this->vertices[id]; };
    vector<Vertex>* get_vertices() {return &this->vertices; };

    int num_vertices;
    
    private:
    void print_edges(Vertex*);
    void del_edge_inner(Vertex*, Edge* prev, Edge* next);
};

#endif