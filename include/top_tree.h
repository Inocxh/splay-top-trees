#ifndef TOPTREE_H

#define TOPTREE_H

#include "underlying_tree.h"

template<class T> class Node; 
template<class T> class InternalNode;
template<class T> class LeafNode;

template<class T>
class TopTree {
    Tree<T> underlying_tree;
    Node<T>* find_consuming_node(Vertex<T>*);
    void delete_all_ancestors(Node<T>*);
    Node<T>* expose_internal(Vertex<T>*);
    Node<T>* deexpose_internal(Vertex<T>*);
    Node<T>* link_internal(Vertex<T>*, Vertex<T>*, T);
    std::tuple<Node<T>*, Node<T>*> cut_internal(Edge<T>*);

    Vertex<T>* get_vertex(int id);
    public:
    Node<T>* expose(int vertex);
    Node<T>* deexpose(int vertex);
    Node<T>* link(int u, int v, T);
    std::tuple<Node<T>*, Node<T>*> cut(int, int);
    

    
};

template<class T>
class Node {

    protected:
    bool flipped = false;
    InternalNode<T>* parent = nullptr;
    T data;
    
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
    bool vertex_is_right(Vertex<T>* v);

    Node* get_sibling();
    InternalNode<T>* get_parent();
    void set_parent(InternalNode<T>*);
    int num_boundary_vertices;
    
    bool is_left_child();
    
    #ifdef TEST
    bool is_flipped() { return flipped; }
    Node(int num_bound, int f) {
        this->flipped = f;
        this->num_boundary_vertices = num_bound;
    }
    #endif
};

template<class T>
class InternalNode : public Node<T> {

    public: 

    InternalNode(Node<T>*, Node<T>*);
    Node<T>* children[2];
    void push_flip();
    
    bool has_left_boundary();
    bool has_middle_boundary();
    bool has_right_boundary();




    #ifdef TEST
    InternalNode(int num_boundary, bool f) : Node<T>(num_boundary, f) {};
    //Test methods
    void set_children(Node<T>* left, Node<T>* right) {
        this->children[0] = left;
        this->children[1] = right;
    }

    #endif
    
    // Test constructor
    
};

template<class T>
class LeafNode : public Node<T> {
    Edge<T>* edge;

    public:
    bool has_left_boundary();
    bool has_middle_boundary();
    bool has_right_boundary();

    LeafNode(Edge<T>*, T);
    ~LeafNode();
    bool vertex_is_right(Vertex<T>* v);
    void push_flip();

    #ifdef TEST
    LeafNode(Edge<T>* e, int num_boundary, bool f): Node<T>(num_boundary,f){
        this->edge = e;
    }
    Vertex<T>* get_endpoint(int idx) {
        return this->edge->endpoints[idx];
    }
    #endif
    
};







#endif


