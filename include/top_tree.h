#ifndef TOPTREE_H

#define TOPTREE_H

#include "underlying_tree.h"

class Node; 
class InternalNode;
class LeafNode;

class UserData;
template <class UserData> class TopTree;

// UserData must implement merge and split
template <class UserData>
class TopTree {

    typedef Node Node<UserData>;
    /*static void merge(Node* n1, Node* n2) {
        user_data.merge();
    }    */
    Node* find_consuming_node(Vertex*);

    
    public:
    Node* expose(int vertex);
    void deexpose(int vertex);
    Node* link(int v1, int v2);
    std::tuple<Node*, Node*> cut(int v1, int v2);

    

};

class Node {
    protected:
    bool flipped = false;
    InternalNode* parent = nullptr;
    int num_boundary_vertices;

    public:
    void full_splay();
    void semi_splay();
    virtual bool has_left_boundary() = 0;
    virtual bool has_middle_boundary() = 0;
    virtual bool has_right_boundary() = 0;
    
    Node* semi_splay_step();
    void rotate_up();
    void flip();

    bool is_point();
    bool is_path();
    bool vertex_is_right(Vertex* v);

    Node* get_sibling();
    InternalNode* get_parent();
    
    bool is_left_child();
    
    #ifdef TEST
    bool is_flipped() { return flipped; }
    Node(int num_bound, int f) {
        this->flipped = f;
        this->num_boundary_vertices = num_bound;
    }
    void set_parent(InternalNode* par) {
        this->parent = par;
    };
    #endif
};

class InternalNode : public Node {
    public: 
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

    LeafNode(Edge);
    ~LeafNode();
    bool vertex_is_right(Vertex* v);

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


