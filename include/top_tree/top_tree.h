#ifndef TOP_TREE 
#define TOP_TREE 1

#include "underlying_tree.h"
#include <vector>


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
    Tree<C,E,V> underlying_tree;

    C* find_consuming_node(Vertex<C,E,V>*);
    void delete_all_ancestors(C*);

    C* expose_internal(Vertex<C,E,V>*);
    C* prepare_expose(C*);
    C* expose_prepared(C*);

    C* deexpose_internal(Vertex<C,E,V>*);
    std::tuple<C*,Edge<C,E,V>*> link_internal(Vertex<C,E,V>*, Vertex<C,E,V>*, E);
    std::tuple<C*, C*> cut_internal(Edge<C,E,V>*);


    public:
    C* expose(int v1, int v2);
    C* expose(int vertex);

    C* expose_fast(int vertex);
    C* expose_fast(int v1, int v2);

    C* deexpose(int v1, int v2);
    C* deexpose(int vertex);

    C* link(int u, int v, E);
    std::tuple<C*, C*> cut(int, int);

    Edge<C,E,V>* link_ptr(int u, int v, E);
    std::tuple<C*,C*> cut_ptr(Edge<C,E,V>*);

    C* link_leaf(int u, int v, E);
    std::tuple<C*,C*> cut_leaf(C*);

    C* get_adjacent_leaf_node(int);
    C* get_adjacent_leaf_node(int, int);

    bool connected(int v1, int v2);
    
    TopTree(int size);
    TopTree() {};
    ~TopTree();

    void print_tree() {
        this->underlying_tree.print_tree();
    }

};

template<class C = DefaultC, class E = None, class V = None> 
class Node {
    friend class TopTree<C,E,V>;
    friend class InternalNode<C,E,V>;
    friend class LeafNode<C,E,V>;

    InternalNode<C,E,V>* parent;
    int num_boundary_vertices;
    bool flipped = false;
 
    //These must be implemented by the user!
    virtual void merge(C*, C*) = 0;
    virtual void create(E*, V*, V*) = 0;
    
    //Optional user methods
    virtual void split(C*, C*) {};
    virtual void destroy(E*, V*, V*) {};
    virtual void swap_data() {};

    void rotate_up();
    void flip();
    Node<C,E,V>* semi_splay_step();

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
    virtual int get_endpoint_id(int) = 0;
    virtual Edge<C,E,V>* get_edge() = 0;
    virtual void delete_tree() = 0;

    
    public:
    virtual C* get_child(int) = 0;

    virtual void push_flip() = 0;
    void clean();
    void full_splay();
    void semi_splay();
    
    //Implemented by LeafNode and InternalNode //TODO Make private
    virtual void merge_internal() = 0; //TODO MOVE BACK;
    virtual void split_internal() = 0;

    void recompute_root_path();
    int get_num_boundary_vertices();
    virtual void print(int, bool) {};
    virtual void print_data() {};

    Node<C,E,V>() {};
    ~Node<C,E,V>() {};
    
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

    C* get_child(int);

    public:
    bool has_left_boundary();
    bool has_middle_boundary();
    bool has_right_boundary();
    Edge<C,E,V>* get_edge();
    int get_endpoint_id(int);
    void delete_tree();

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
    Edge<C,E,V>* get_edge();
    int get_endpoint_id(int);
    C* get_child(int);
    void delete_tree();


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
    void destroy(None*, None*, None*) {};
};
#endif