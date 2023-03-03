#ifndef TOP_TREE 
#define TOP_TREE 1

#include <vector>

#include "underlying_tree.h"

/*
C: Cluster data,
E: Edge data,
V: Vertex data
*/

template<class C, class E, class V> 
class Node;

template<class C, class E, class V> 
class LeafNode;

template<class C, class E, class V> 
class InternalNode;


template<class C, class E, class V> class Edge;
template<class C, class E, class V> class Vertex;
template<class C, class E, class V> class Tree;

// DefaultC defined in bottom. Inherits from Node and has no fields.
// merge and split simply does nothing.
class DefaultC;

template<class C = DefaultC, class E = None, class V = None>
class TopTree {
    int num_exposed = 0;
    Tree<C,E,V>* underlying_tree;

    C* find_consuming_node(Vertex<C,E,V>*);
    void delete_all_ancestors(C*);
    C* expose_internal(Vertex<C,E,V>*);
    C* deexpose_internal(Vertex<C,E,V>*);
    C* link_internal(Vertex<C,E,V>*, Vertex<C,E,V>*, E);
    std::tuple<C*, C*> cut_internal(Edge<C,E,V>*);


    public:
    C* expose(int v1, int v2);
    C* expose(int vertex);
    C* deexpose(int v1, int v2);
    C* deexpose(int vertex);
    C* link(int u, int v, E);
    std::tuple<C*, C*> cut(int, int);


    TopTree(int size);

    #ifdef TEST
    public:
    Vertex<C,E,V>* get_vertex(int id) {
        return this->underlying_tree->get_vertex(id);
    }
    #endif
    
};

template<class C = DefaultC, class E = None, class V = None> 
class Node {
    friend class TopTree<C,E,V>;
    friend class InternalNode<C,E,V>;

    protected:
    InternalNode<C,E,V>* parent;
    int num_boundary_vertices;
    bool flipped = false;
 
    //These must be implemented by the user!
    virtual void merge() = 0;
    virtual void split() = 0;
    virtual void merge_internal(C*, C*) = 0;
    virtual void merge_leaf(E*, V*, V*) = 0;

    bool is_point();
    bool is_path();
    bool is_right_child();

    virtual bool has_left_boundary() = 0;
    virtual bool has_middle_boundary() = 0;
    virtual bool has_right_boundary() = 0;

    void full_splay();
    void semi_splay();
    
    public:
    C* get_sibling();
    InternalNode<C,E,V>* get_parent();
    void set_parent(InternalNode<C,E,V>*);

    #ifdef TEST
    public:
    bool is_flipped() {
        return this->flipped;   
    }
    #endif

    virtual void print(int, bool) {};
    virtual void print_data() {};
    void flip();
    void rotate_up();
    Node<C,E,V>* semi_splay_step();
    
};

// C *must* inherit from Node<C,E,V>
template<class C = DefaultC, class E = None, class V = None>
class LeafNode : public C {
    friend class TopTree<C,E,V>;

    Edge<C, E, V>* edge;

    // Defined here as we cannot express that C inherits from Node elegantly.


    bool is_right_vertex(Vertex<C,E,V>*);
    
    void merge();
    void split();

    LeafNode(Edge<C,E,V>*, int);

    public:
    bool has_left_boundary();
    bool has_middle_boundary();
    bool has_right_boundary();

    void print(int, bool);
    //In order to contruct test cases we need a stronger constructor
    #ifdef TEST
    public:
    LeafNode(Edge<C,E,V>* e, int num_boundary, bool f) {
        this->edge = e;
        this->num_boundary_vertices = num_boundary;
        this->flipped = f;
    }
    #endif

    Vertex<C,E,V>* get_endpoint(int);
};

// C *must* inherit from Node<C,E,V>
template<class C = DefaultC, class E = None, class V = None>
class InternalNode : public C {
    friend class TopTree<C,E,V>;
    friend class Node<C,E,V>;

    C* children[2];
    
    // See note in LeafNode

    void push_flip();

    void merge();
    void split();

    InternalNode(C*, C*, int);
    public:
    bool has_left_boundary();
    bool has_middle_boundary();
    bool has_right_boundary();


    void print(int, bool);
    #ifdef TEST
    void set_children(C* left, C* right) {
        this->children[0] = left;
        this->children[1] = right;
    }

    C* get_child(int idx) {
        return this->children[idx];
    }
    InternalNode(int num_boundary, bool f) {
        this->num_boundary_vertices = num_boundary;
        this->flipped = f;
    };
    #endif
};


#include "node.hpp"
#include "internal_node.hpp"
#include "leaf_node.hpp"
#include "top_tree.hpp"


class MyNode : public Node<MyNode, int, void> {
    protected:
    void merge() {
        std::cout <<"hello from merge" << std::endl;
        return;
    };
    void split() {
        return;
    };
    public:
    MyNode() : Node<MyNode,int,void>() {
        
    };
};

class DefaultC : public Node<DefaultC, None, None> {
    protected:
    void merge_internal(DefaultC*, DefaultC*) {};
    void merge_leaf(None*, None*, None*) {};
    void split_internal(DefaultC*, DefaultC*) {};
    void split_leaf(None*, None*, None*) {};
};
#endif