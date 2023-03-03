#include "top_tree.h"
#include <tuple>

#include <assert.h>



template<class C, class E, class V>
TopTree<C,E,V>::TopTree(int size) {
    this->underlying_tree = new Tree<C,E,V>(size);
}

template<class C, class E, class V>
C* TopTree<C,E,V>::find_consuming_node(Vertex<C,E,V>* vertex) {
    if (!(vertex->get_first_edge())) {
        return nullptr;
    }
    //The node associated to an edge will *always* be a LeafNode<C,E,V>
    LeafNode<C,E,V>* first_node = (LeafNode<C,E,V>*) vertex->get_first_edge()->node;


    first_node->semi_splay();
    
    if (vertex->has_at_most_one_incident_edge()) {
        return first_node;
    }

    bool is_right = first_node->is_right_vertex(vertex);
    bool is_left = !is_right;
    bool is_middle = false;
    C* last_middle_node = nullptr;

    C* node = first_node; 
    while (node->get_parent()) {
        C* parent = (InternalNode<C,E,V>*) (node->get_parent());
        
        is_middle = node->is_right_child() ? 
                    is_left  || (is_middle && !node->has_left_boundary()) :
                    is_right || (is_middle && !node->has_right_boundary());
        is_left = !node->is_right_child() && !is_middle;
        is_right = node->is_right_child() && !is_middle;        
 
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



template<class C, class E, class V>
C* TopTree<C,E,V>::expose(int vertex1_id, int vertex2_id) {
    assert(this->num_exposed == 0);
    this->num_exposed += 2;
    Vertex<C,E,V>* vertex1 = this->get_vertex(vertex1_id);
    Vertex<C,E,V>* vertex2 = this->get_vertex(vertex2_id);
    expose_internal(vertex1);
    return expose_internal(vertex2);
}
template<class C, class E, class V>
C* TopTree<C,E,V>::expose(int vertex_id) {
    assert(this->num_exposed < 2);
    this->num_exposed += 1;
    Vertex<C,E,V>* vertex = this->get_vertex(vertex_id);
    return expose_internal(vertex);
}

template<class C, class E, class V>
C* TopTree<C,E,V>::expose_internal(Vertex<C,E,V>* vertex) {
    C* node = this->find_consuming_node(vertex);
    if (!node) {
        vertex->exposed = true;
        return nullptr;
    }

    while (node->is_path()) {
        //Is this legal? 
        InternalNode<C,E,V>* node_int = (InternalNode<C,E,V>*) node;
        InternalNode<C,E,V>* parent = (InternalNode<C,E,V>*) (node->get_parent());

        node_int->push_flip();
        node_int->children[node->is_right_child()]->rotate_up();

        node = parent;
    }
    node->full_splay();

    C* root;
    //is needed in C* for merge calls to compute correctly. probably...
    vertex->exposed = true;
    while (node) {
        root = node;
        root->num_boundary_vertices += 1;
        root->merge();
        node = (C*) (root->get_parent());
    }
    return root;
}

template<class C, class E, class V>
C* TopTree<C,E,V>::deexpose_internal(Vertex<C,E,V>* vertex) { 
    C* root = nullptr;

    //Vertex<C,E,V><T>* vertex = this->get_vertex(id);
    C* node = this->find_consuming_node(vertex); 
    vertex->exposed = false;
    while (node) {
        root = node;
        root->num_boundary_vertices -= 1;
        root->merge();
        node = root->get_parent();
    }
    return root;
}

template<class C, class E, class V>
C* TopTree<C,E,V>::deexpose(int vertex1_id, int vertex2_id) { 
    assert(this->num_exposed == 2);
    this->num_exposed -= 2;
    Vertex<C,E,V>* vertex1 = this->get_vertex(vertex1_id);
    Vertex<C,E,V>* vertex2 = this->get_vertex(vertex2_id);
    deexpose_internal(vertex1);
    return deexpose_internal(vertex2);
}

template<class C, class E, class V>
C* TopTree<C,E,V>::deexpose(int vertex_id) { 
    assert(this->num_exposed >= 1);
    this->num_exposed -= 1;
    Vertex<C,E,V>* vertex = this->get_vertex(vertex_id);
    return deexpose_internal(vertex);
}

template<class C, class E, class V>
C* TopTree<C,E,V>::link(int u_id, int v_id, E data) {
    assert(this->num_exposed == 0);
    Vertex<C,E,V>* u = this->get_vertex(u_id); 
    Vertex<C,E,V>* v = this->get_vertex(v_id); 
    return link_internal(u, v, data);
}

//Assumes u and v in trees with no exposed vertices!
template<class C, class E, class V>
C* TopTree<C,E,V>::link_internal(Vertex<C,E,V>* u, Vertex<C,E,V>* v, E data) {
    
    C* Tu = expose_internal(u);
    if (Tu && Tu->has_left_boundary()) {
        Tu->flip();
    }
    u->exposed = false;
    C* Tv = expose_internal(v);
    if (Tv && Tv->has_right_boundary()) {
        Tv->flip();
    }
    v->exposed = false;

    Edge<C,E,V>* edge = this->underlying_tree->add_edge(u, v, data);
    C* root = new LeafNode<C,E,V>(edge, !!Tu + !!Tv);
    edge->set_leaf_node((LeafNode<C,E,V>*) root);
    //root->num_boundary_vertices = !!Tu + !!Tv;

    if (Tu) {
        InternalNode<C,E,V>* root_new = new InternalNode<C,E,V>(Tu, root, !!Tv);        
        //root_new->num_boundary_vertices = !!Tv;
        //Tu->set_parent(root_new);
        //root->set_parent(root_new);

        root = root_new;
    }
    if (Tv) {
        InternalNode<C,E,V>* root_new = new InternalNode<C,E,V>(root, Tv, 0);
        //root->num_boundary_vertices = 0;
        //Tv->set_parent(root_new);
        //root->set_parent(root_new);

        root = root_new;
    }
    return root;
}

template<class C, class E, class V>
void TopTree<C,E,V>::delete_all_ancestors(C* node) {
    C* parent = node->get_parent();
    if (parent) {
        C* sibling = node->get_sibling();
        delete_all_ancestors(parent);
        sibling->set_parent(nullptr);
    }
    node->split();
    delete node;
}



template<class C, class E, class V>
std::tuple<C*, C*> TopTree<C,E,V>::cut(int u_id, int v_id) {
    assert(this->num_exposed == 0);
    Edge<C,E,V>* e = this->underlying_tree->find_edge(u_id, v_id);
    return this->cut_internal(e);
}
    
template<class C, class E, class V>
std::tuple<C*, C*> TopTree<C,E,V>::cut_internal(Edge<C,E,V>* edge) {
    Vertex<C,E,V>* u = edge->endpoints[0];
    Vertex<C,E,V>* v = edge->endpoints[1];

    edge->node->full_splay();
    this->delete_all_ancestors(edge->node);
    this->underlying_tree->del_edge(edge);    

    u->exposed = true;
    v->exposed = true;
    C* Tu = this->deexpose_internal(u);
    C* Tv = this->deexpose_internal(v);
    return std::tuple(Tu, Tv);
}


