#ifndef TOPTREE_H
#define TOPTREE_H

#include "underlying_tree.h"
#include <tuple>
#include <iostream>

class NodeBase;
class InternalNodeBase;
class LeafNodeBase;

template<class T> class InternalNode;
template<class T> class LeafNode;
template<class T> class TopTree;

template<class T>
class TopTree {
    Tree* underlying_tree;
    NodeBase* find_consuming_node(Vertex*);
    void delete_all_ancestors(NodeBase*);
    NodeBase* expose_internal(Vertex*);
    NodeBase* deexpose_internal(Vertex*);
    NodeBase* link_internal(Vertex*, Vertex*, T*);
    std::tuple<NodeBase*, NodeBase*> cut_internal(Edge*);

    Vertex* get_vertex(int id);
    public:
    NodeBase* expose(int vertex);
    NodeBase* deexpose(int vertex);
    NodeBase* link(int u, int v, T*);
    std::tuple<NodeBase*, NodeBase*> cut(int, int);
    T* get_data(NodeBase*);


    TopTree<T>(int size) {
        this->underlying_tree = new Tree(size);
    }
    #ifdef TEST
    Tree* get_tree() {
        return this->underlying_tree;
    };
    #endif 
};


class NodeBase {
    protected:
    bool flipped = false;
    InternalNodeBase* parent = nullptr;
    
    public:
    int num_boundary_vertices;

    virtual bool has_left_boundary() = 0;
    virtual bool has_middle_boundary() = 0;
    virtual bool has_right_boundary() = 0;
    virtual void push_flip() = 0;
    virtual void rotate_up();

    void full_splay();
    void semi_splay();
    
    NodeBase* semi_splay_step();
    
    void flip();

    bool is_point();
    bool is_path();
    bool vertex_is_right(Vertex* v);

    NodeBase* get_sibling();
    InternalNodeBase* get_parent();
    void set_parent(InternalNodeBase*);
    
    bool is_left_child();

    NodeBase() {};
    
    #ifdef TEST
    bool is_flipped() { return flipped; }
    NodeBase(int num_bound, int f) {
        this->flipped = f;
        this->num_boundary_vertices = num_bound;
    }
    #endif
};

class InternalNodeBase : public NodeBase {
    public: 
    NodeBase* children[2];
    void push_flip();
    
    bool has_left_boundary();
    bool has_middle_boundary();
    bool has_right_boundary();

    InternalNodeBase() {};
    //virtual ~InternalNodeBase();

    #ifdef TEST
    //Test methods
    InternalNodeBase(int num_boundary,bool  f) : NodeBase(num_boundary,f) {};
    void set_children(NodeBase* left, NodeBase* right) {
        this->children[0] = left;
        this->children[1] = right;
    }

    #endif
};

class LeafNodeBase : public NodeBase {
    protected:
    Edge* edge;

    public:
    bool has_left_boundary();
    bool has_middle_boundary();
    bool has_right_boundary();


    bool vertex_is_right(Vertex* v);
    void push_flip();

    LeafNodeBase() {};
    //virtual ~LeafNodeBase();

    #ifdef TEST
    LeafNodeBase(int num_boundary,bool f) : NodeBase(num_boundary,f) {};
    Vertex* get_endpoint(int idx) {
        return this->edge->endpoints[idx];
    }
    #endif
    
};

template<class T>
class TContainer {
    public:
    T* user_data;

    virtual void merge() = 0;
    virtual void split() = 0;
};

template<class T>
class LeafNode : public LeafNodeBase, public TContainer<T> {
    public:
    LeafNode(Edge* edge, T* user_data)  {
        edge->set_leaf_node(this);
        this->edge = edge;
        this->user_data = user_data;
    };
    ~LeafNode();

    void merge() { 
        //Does nothing as no children exist
        return;
    };
    void split() {
        return;
    }; 

    void rotate_up() {
        InternalNode<T>* parent = (InternalNode<T>*) this->get_parent();
        InternalNode<T>* grandparent =(InternalNode<T>*) parent->get_parent();
        grandparent->split();
        parent->split();
        this->NodeBase::rotate_up();
        parent->merge();
        grandparent->merge();
    };

    



    #ifdef TEST

    LeafNode(Edge* e, int num_boundary, bool f) : LeafNodeBase(num_boundary,f){
        this->edge = e;
    }
    #endif
};
template<class T>
class InternalNode : public InternalNodeBase, public TContainer<T> {
    public:
    InternalNode(NodeBase* left, NodeBase* right) {
        this->flipped = false;
        this->num_boundary_vertices = 0;
        this->children[0] = left;
        this->children[1] = right;

        T* tleft = dynamic_cast<TContainer<T>*>(left)->user_data;
        T* tright = dynamic_cast<TContainer<T>*>(right)->user_data;

        this->user_data = T::merge(tleft, tright);   
    };
    
    void merge() {
        T* left_data =  (dynamic_cast<TContainer<T>*>(this->children[0]))->user_data;
        T* right_data =  (dynamic_cast<TContainer<T>*>(this->children[1]))->user_data;
        this->user_data = T::merge(left_data, right_data);
    };

    void split() {
        std::tuple<T*,T*> new_datas = T::split(this->user_data); 
        (dynamic_cast<TContainer<T>*>(this->children[0]))->user_data = std::get<0>(new_datas);
        (dynamic_cast<TContainer<T>*>(this->children[1]))->user_data = std::get<1>(new_datas);
    };


    void rotate_up() {
        InternalNode<T>* parent = (InternalNode<T>*) this->get_parent();
        InternalNode<T>* grandparent =(InternalNode<T>*) parent->get_parent();
        grandparent->split();
        parent->split();
        this->NodeBase::rotate_up();
        parent->merge();
        grandparent->merge();
    };


    #ifdef TEST
    InternalNode(int num_boundary, bool f) : InternalNodeBase(num_boundary, f) {};
    #endif
};


template<class T>
NodeBase* TopTree<T>::find_consuming_node(Vertex* vertex) {

    if (!(vertex->get_first_edge())) {
        return nullptr;
    }
    //The node associated to an edge will *always* be a LeafNode<T>
    LeafNode<T>* first_node = (LeafNode<T>*) vertex->get_first_edge()->node;

    first_node->semi_splay();
    
    if (vertex->has_at_most_one_incident_edge()) {
        return first_node;
    }

    bool is_right = first_node->vertex_is_right(vertex);
    bool is_left = !is_right;
    bool is_middle = false;
    NodeBase* last_middle_node = nullptr;

    NodeBase* node = first_node; 
    while (node->get_parent()) {
        NodeBase* parent = (InternalNode<T>*) (node->get_parent());
        
        is_middle = node->is_left_child() ? 
                    is_right || (is_middle && !node->has_right_boundary()) :
                    is_left  || (is_middle && !node->has_left_boundary());
        is_left = node->is_left_child() && !is_middle;
        is_right = !node->is_left_child() && !is_middle;        
 
        node = parent;
        if (is_middle) {
            if (!node->has_middle_boundary()) {
                return node;
            }
            last_middle_node = node;
        }
    }
    return last_middle_node;
}

template<class T>
T* TopTree<T>::get_data(NodeBase* node) {
    T* data =  (dynamic_cast<TContainer<T>*>(node))->user_data;
    return data;
}


template<class T>
Vertex* TopTree<T>::get_vertex(int id) {
    return this->underlying_tree->get_vertex(id);
}


template<class T>
NodeBase* TopTree<T>::expose(int vertex_id) {
    Vertex* vertex = this->get_vertex(vertex_id);
    return expose_internal(vertex);
}

template<class T>
NodeBase* TopTree<T>::expose_internal(Vertex* vertex) {
    NodeBase* node = this->find_consuming_node(vertex);
    if (!node) {
        vertex->exposed = true;
        return nullptr;
    }

    while (node->is_path()) {
        //Is this legal?
        InternalNode<T>* node_int = (InternalNode<T>*) node;
        InternalNode<T>* parent = (InternalNode<T>*) (node->get_parent());

        node_int->push_flip();
        int node_idx = !node->is_left_child();
        node_int->children[node_idx]->rotate_up();

        node = parent;
    }
    node->full_splay();

    NodeBase* root;
    while (node) {
        root = node;
        root->num_boundary_vertices += 1;
        node = (NodeBase*) (root->get_parent());
    }  
    vertex->exposed = true;
    return root;
}

template<class T>
NodeBase* TopTree<T>::deexpose_internal(Vertex* vertex) { 
    NodeBase* root = nullptr;

    //Vertex<T>* vertex = this->get_vertex(id);
    NodeBase* node = this->find_consuming_node(vertex); 
    while (node) {
        root = node;
        root->num_boundary_vertices -= 1;
        node = root->get_parent();
    }
    vertex->exposed = false;
    return root;
}

template<class T>
NodeBase* TopTree<T>::deexpose(int vertex_id) { 
    Vertex* vertex = this->get_vertex(vertex_id);
    return deexpose_internal(vertex);
}

template<class T>
NodeBase* TopTree<T>::link(int u_id, int v_id, T* data) {
    Vertex* u = this->get_vertex(u_id); 
    Vertex* v = this->get_vertex(v_id); 
    return link_internal(u, v, data);
}

//Assumes u and v in trees with no exposed vertices!
template<class T>
NodeBase* TopTree<T>::link_internal(Vertex* u, Vertex* v, T* data) {
    
    NodeBase* Tu = expose_internal(u);
    if (Tu && Tu->has_left_boundary()) {
        Tu->flip();
    }
    u->exposed = false;
    NodeBase* Tv = expose_internal(v);
    if (Tv && Tv->has_right_boundary()) {
        Tv->flip();
    }
    v->exposed = false;



    Edge* edge = this->underlying_tree->add_edge(u, v);
    NodeBase* root = new LeafNode<T>(edge, data);
    root->num_boundary_vertices = (Tu != nullptr) + (Tv != nullptr);
    
    if (Tu) {
        InternalNode<T>* root_new = new InternalNode<T>(Tu, root);        
        root_new->num_boundary_vertices = (Tv != nullptr);
        Tu->set_parent(root_new);
        root->set_parent(root_new);
        root_new->merge();

        root = root_new;
    }
    if (Tv) {
        InternalNode<T>* root_new = new InternalNode<T>(root, Tv);
        root->num_boundary_vertices = 0;
        Tv->set_parent(root_new);
        root->set_parent(root_new);
        root_new->merge();

        root = root_new;
    }
    return root;
}

template<class T>
void TopTree<T>::delete_all_ancestors(NodeBase* node) {
    NodeBase* parent = node->get_parent();
    if (parent) {
        NodeBase* sibling = node->get_sibling();
        delete_all_ancestors(parent);
        sibling->set_parent(nullptr);
    }
    delete node;
}



template<class T>
std::tuple<NodeBase*, NodeBase*> TopTree<T>::cut(int u_id, int v_id) {
    Edge* e = this->underlying_tree->find_edge(u_id,v_id);
    return this->cut_internal(e);
}
    
template<class T>
std::tuple<NodeBase*, NodeBase*> TopTree<T>::cut_internal(Edge* edge) {
    Vertex* u = edge->endpoints[0];
    Vertex* v = edge->endpoints[1];

    edge->node->full_splay();
    this->delete_all_ancestors(edge->node);

    u->exposed = true;
    v->exposed = true;
    NodeBase* Tu = this->deexpose_internal(u);
    NodeBase* Tv = this->deexpose_internal(v);
    return std::tuple(Tu, Tv);
}



#endif




