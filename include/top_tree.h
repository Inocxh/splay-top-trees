#ifndef TOPTREE_H

#define TOPTREE_H

#include "underlying_tree.h"

class Node; 
class InternalNode;
class LeafNode;

class UserData {
    int weight;

    public:
    UserData(int w) {
        this->weight = w;
    }
    
    UserData* merge(UserData* other) {
        return new UserData(std::max(this->weight, other->weight));
    }
    void split(UserData* parent, UserData* left_child, UserData* right_child) {
        return;
    }

    
};

class TopTree {
    Tree underlying_tree;
    Node* find_consuming_node(Vertex*);
    void delete_all_ancestors(Node*);
    Node* expose_internal(Vertex*);
    Node* deexpose_internal(Vertex*);
    Node* link_internal(Vertex*, Vertex*, UserData*);
    std::tuple<Node*, Node*> cut_internal(Edge*);

    Vertex* get_vertex(int id);
    public:
    Node* expose(int vertex);
    Node* deexpose(int vertex);
    Node* link(int u, int v, UserData*);
    std::tuple<Node*, Node*> cut(int, int);
    

    
};

class Node {
    protected:
    bool flipped = false;
    InternalNode* parent = nullptr;
    
    public:
    void full_splay();
    void semi_splay();
    virtual bool has_left_boundary() = 0;
    virtual bool has_middle_boundary() = 0;
    virtual bool has_right_boundary() = 0;
    
    Node* semi_splay_step();
    void rotate_up();
    void flip();
    virtual void push_flip() = 0;

    bool is_point();
    bool is_path();
    bool vertex_is_right(Vertex* v);

    Node* get_sibling();
    InternalNode* get_parent();
    void set_parent(InternalNode*);
    int num_boundary_vertices;
    
    bool is_left_child();
    
    UserData* user_data;
    
    #ifdef TEST
    bool is_flipped() { return flipped; }
    Node(int num_bound, int f) {
        this->flipped = f;
        this->num_boundary_vertices = num_bound;
    }
    #endif
};

class InternalNode : public Node {
    public: 

    InternalNode(Node*, Node*);
    Node* children[2];
    void push_flip();
    
    bool has_left_boundary();
    bool has_middle_boundary();
    bool has_right_boundary();




    #ifdef TEST
    InternalNode(int num_boundary, bool f) : Node(num_boundary, f) {};
    //Test methods
    void set_children(Node* left, Node* right) {
        this->children[0] = left;
        this->children[1] = right;
    }

    #endif
    
    // Test constructor
    
};

class LeafNode : public Node {
    Edge* edge;

    public:
    bool has_left_boundary();
    bool has_middle_boundary();
    bool has_right_boundary();

    LeafNode(Edge*, UserData*);
    ~LeafNode();
    bool vertex_is_right(Vertex* v);
    void push_flip();

    #ifdef TEST
    LeafNode(Edge* e, int num_boundary, bool f): Node(num_boundary,f){
        this->edge = e;
    }
    Vertex* get_endpoint(int idx) {
        return this->edge->endpoints[idx];
    }
    #endif
    
};







#endif


