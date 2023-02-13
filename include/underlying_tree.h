#ifndef UNDERLYINGTREE_H
#define UNDERLYINGTREE_H

#include <vector>
using namespace std;

class Edge; 
class Vertex;
class Tree;
class UserData;

class Edge {
    public:
    Edge(Vertex* left, Vertex* right, int weight);
    int weight;
    UserData* user_data;
    Vertex* endpoints[2];
    Edge* prev[2];
    Edge* next[2];
    
    int vertex_is_right(Vertex* v);


};

class Vertex {
    bool exposed;
    Edge *first_edge;
    
    public: 

    int id;
    
    Vertex(int id) {
        this->id = id;
        first_edge = nullptr;
        exposed = false;
    }
    Edge* get_first_edge() { return first_edge; };
    void set_first_edge(Edge* e) { first_edge = e; };
    bool has_at_most_one_incident_edge();
};


class Tree {
    
    int num_vertices;
    vector<Vertex> vertices;

    public:
    Tree(int num_vertices);
    ~Tree();
    Edge* add_edge(int u, int v, int weight);
    void del_edge(Edge* uv);

    void print_tree();

    Vertex* get_vertex(int id) {return &this->vertices[id]; };

    
    private:
    void print_edges(Vertex*);
    void del_edge_inner(Vertex*, Edge* prev, Edge* next);
};







#endif