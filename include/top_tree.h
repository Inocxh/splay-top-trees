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
};

template<class C = DefaultC, class E = None, class V = None> 
class Node {
    friend class TopTree<C,E,V>;
    friend class InternalNode<C,E,V>;
    friend class LeafNode<C,E,V>;

    InternalNode<C,E,V>* parent;
    int num_boundary_vertices;
    bool flipped = false;
 
    //Implemented by LeafNode and InternalNode
    virtual void merge_internal() = 0;
    virtual void split_internal() = 0;

    //These must be implemented by the user!
    virtual void merge(C*, C*) = 0;
    virtual void create(E*, V*, V*) = 0;
    
    //Optional user methods
    virtual void split() {};
    virtual void split_leaf() {};
    virtual void swap_data() {};

    virtual void push_flip() = 0;

    void rotate_up();
    void flip();
    Node<C,E,V>* semi_splay_step();
    void full_splay();
    void semi_splay();

    C* get_sibling();
    InternalNode<C,E,V>* get_parent();
    void set_parent(InternalNode<C,E,V>*);

    protected:
    bool is_point();
    bool is_path();
    bool is_right_child();
    bool is_flipped();

    virtual bool has_left_boundary() = 0;
    virtual bool has_middle_boundary() = 0;
    virtual bool has_right_boundary() = 0;

    
    public:

    virtual void print(int, bool) {};
    virtual void print_data() {};
    
};

// C *must* inherit from Node<C,E,V>
template<class C = DefaultC, class E = None, class V = None>
class LeafNode : public C {
    friend class TopTree<C,E,V>;

    Edge<C, E, V>* edge;

    // Defined here as we cannot express that C inherits from Node elegantly.


    bool is_right_vertex(Vertex<C,E,V>*);
    
    void merge_internal();
    void split_internal();
    void push_flip();

    LeafNode(Edge<C,E,V>*, int);

    public:
    bool has_left_boundary();
    bool has_middle_boundary();
    bool has_right_boundary();

    void print(int, bool);

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

    void merge_internal();
    void split_internal();

    InternalNode(C*, C*, int);
    public:
    bool has_left_boundary();
    bool has_middle_boundary();
    bool has_right_boundary();


    void print(int, bool);
};


#include "node.hpp"
#include "internal_node.hpp"
#include "leaf_node.hpp"
#include "top_tree.hpp"

class DefaultC : public Node<DefaultC, None, None> {
    protected:
    void merge(DefaultC*, DefaultC*) {};
    void create(None*, None*, None*) {};
    void split_internal(DefaultC*, DefaultC*) {};
    void split_leaf(None*, None*, None*) {};
};
#endif