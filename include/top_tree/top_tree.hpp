#include "top_tree.h"

#include <tuple>
#include <vector>
#include <cassert>

template<class C, class E, class V>
TopTree<C,E,V>::TopTree(int size) {
    this->underlying_tree = Tree<C,E,V>(size);
}
template<class C, class E, class V>
TopTree<C,E,V>::~TopTree() {
    for (int i = 0; i < this->underlying_tree.get_size(); i++) {
        Vertex<C,E,V>* vertex = this->underlying_tree.get_vertex(i);
        Edge<C,E,V>* edge = vertex->get_first_edge();
        if (!edge) {
            continue;
        }
        C* root = edge->node;
        if (!root) {
            continue;
        }
        while (root->get_parent()) {
            root = root->get_parent();
        }
        root->delete_tree();
    }
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
        InternalNode<C,E,V>* parent = (InternalNode<C,E,V>*) (node->get_parent());

        bool is_left_child = parent->children[0] == node;
        is_middle = is_left_child ?
                        is_right || (is_middle && !node->has_right_boundary()) :
                        is_left || (is_middle && !node->has_left_boundary()) ;
        is_left = (is_left_child != parent->is_flipped()) && !is_middle;
        is_right = (is_left_child == parent->is_flipped()) && !is_middle;
 
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
    this->expose(vertex1_id);
    return this->expose(vertex2_id);;
}
template<class C, class E, class V>
C* TopTree<C,E,V>::expose(int vertex_id) {
    assert(this->num_exposed < 2);
    this->num_exposed += 1;
    Vertex<C,E,V>* vertex = this->underlying_tree.get_vertex(vertex_id);
    #ifdef SEMI_SPLAY
    return expose_internal_semi_splay(vertex);
    #else
    return expose_internal(vertex);
    #endif
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
        parent->push_flip();
        node_int->push_flip();
        node_int->children[node->is_right_child()]->rotate_up();

        node = parent;
    } 

    node->full_splay();

    //Assert that the depth is at most 1, by lemma 4.3
    assert(!node->get_parent() || (node->get_parent() && !node->get_parent()->get_parent())); 
    InternalNode<C,E,V>* parent = node->get_parent();
    
    if (parent) {
        parent->split_internal();
    }
    node->split_internal();

    // Exposed needed in C* for merge calls to compute correctly. probably...
    vertex->exposed = true;
    node->num_boundary_vertices += 1;
    node->merge_internal();
    if (parent) {
        parent->num_boundary_vertices += 1;
        parent->merge_internal();
    }
    
    return parent ? parent : node;
}

template<class C, class E, class V>
C* TopTree<C,E,V>::expose_internal_semi_splay(Vertex<C,E,V>* vertex) {
    assert(this->num_exposed < 2);
    this->num_exposed += 1;

    auto consuming_node = this->find_consuming_node(vertex);
    if (consuming_node) {
        consuming_node = this->prepare_expose(consuming_node);
        auto root = expose_prepared(consuming_node,vertex);
        return root;
    } else {
        vertex->exposed = true;
        return nullptr;
    }
}

template<class C, class E, class V>
C* TopTree<C,E,V>::expose_prepared(C* consuming_node, Vertex<C,E,V>* vertex) {
    bool from_left = false;
    bool from_right = false;
    auto node = consuming_node;

    std::vector<C*> root_path;
    while (true) {
        auto parent = node->get_parent();
        root_path.push_back(node);
        //node->num_boundary_vertices += 1;
        if (!parent) {
            break;
        }

        bool is_right_child = parent->children[0] == node;

        if ((is_right_child && from_left) || (!is_right_child && from_right)) {
            node->flip();
        }

        from_left = !is_right_child != parent->is_flipped();
        from_right = is_right_child != parent->is_flipped();

        node = parent;
    } 

    for (int i = root_path.size()-1;i >= 0; i--) {
        root_path[i]->split_internal();
    }

    vertex->exposed = true;
    for (int i = 0; i < root_path.size(); i++) {
        root_path[i]->num_boundary_vertices += 1;
        root_path[i]->merge_internal();
    }   
    return root_path.back();
}

template<class C, class E, class V>
C* TopTree<C,E,V>::prepare_expose(C* consuming_node) {
    auto node = consuming_node;
    while (node->get_parent()) {
        auto parent = node->get_parent();
        if (node->is_point()) {
            node = parent;
        } else {
            parent->push_flip();
            node->push_flip();

            auto sibling = node->get_sibling();
            int sibling_idx = sibling->is_right_child();

            auto same_side_child = node->get_child(sibling_idx);

            if (same_side_child->is_path() || sibling->is_point()) {
                //Rotate up other side child.
                (node->get_child(1-sibling_idx))->rotate_up();
                if (node == consuming_node) {
                    consuming_node = parent;
                }
                node = parent;
            } else {
                auto uncle = parent->get_sibling();
                int uncle_idx = parent->get_parent()->get_child(1) == uncle;

                if (sibling_idx == uncle_idx) {
                    node->rotate_up();
                } else {
                    sibling->rotate_up();
                }
            }
        }
    }
    return consuming_node;
}

template<class C, class E, class V>
C* TopTree<C,E,V>::deexpose_internal(Vertex<C,E,V>* vertex) { 
    C* root = nullptr;
    C* node = this->find_consuming_node(vertex); 

    static std::vector<C*> root_path;
    root_path.clear();
    while (node) {
        root_path.push_back(node);
        node = (C*) node->get_parent();
    }
    for (int i = root_path.size() - 1; i >= 0; i--) {
        root_path[i]->split_internal();
    }

    vertex->exposed = false;
    for (int i = 0; i < root_path.size(); i++) {
        root_path[i]->num_boundary_vertices -= 1;
        root_path[i]->merge_internal();
    }
    
    return root_path.size() != 0 ? root_path.back() : nullptr;
}

template<class C, class E, class V>
C* TopTree<C,E,V>::deexpose(int vertex1_id, int vertex2_id) { 
    assert(this->num_exposed == 2);
    this->num_exposed -= 2;
    Vertex<C,E,V>* vertex1 = this->underlying_tree.get_vertex(vertex1_id);
    Vertex<C,E,V>* vertex2 = this->underlying_tree.get_vertex(vertex2_id);
    deexpose_internal(vertex1);
    return deexpose_internal(vertex2);
}

template<class C, class E, class V>
C* TopTree<C,E,V>::deexpose(int vertex_id) { 
    assert(this->num_exposed >= 1);
    this->num_exposed -= 1;
    Vertex<C,E,V>* vertex = this->underlying_tree.get_vertex(vertex_id);
    return deexpose_internal(vertex);
}

template<class C, class E, class V>
C* TopTree<C,E,V>::link(int u_id, int v_id, E data) {
    assert(this->num_exposed == 0);
    Vertex<C,E,V>* u = this->underlying_tree.get_vertex(u_id); 
    Vertex<C,E,V>* v = this->underlying_tree.get_vertex(v_id); 
    return std::get<0>(link_internal(u, v, data));
}

template<class C, class E, class V>
Edge<C,E,V>* TopTree<C,E,V>::link_ptr(int u_id, int v_id, E data) {
    assert(this->num_exposed == 0);
    Vertex<C,E,V>* u = this->underlying_tree.get_vertex(u_id); 
    Vertex<C,E,V>* v = this->underlying_tree.get_vertex(v_id); 
    return std::get<1>(link_internal(u, v, data));
}

template<class C, class E, class V>
C* TopTree<C,E,V>::link_leaf(int u_id, int v_id, E data) {
    assert(this->num_exposed == 0);
    Vertex<C,E,V>* u = this->underlying_tree.get_vertex(u_id); 
    Vertex<C,E,V>* v = this->underlying_tree.get_vertex(v_id); 
    Edge<C,E,V>* new_edge = std::get<1>(link_internal(u, v, data));
    if (new_edge == nullptr) {
        return nullptr;
    } else {
        return new_edge->node;
    }
}

//Assumes u and v in trees with no exposed vertices!
template<class C, class E, class V>
std::tuple<C*,Edge<C,E,V>*> TopTree<C,E,V>::link_internal(Vertex<C,E,V>* u, Vertex<C,E,V>* v, E data) {
    #ifdef SEMI_SPLAY
    C* Tu = expose_internal_semi_splay(u);
    C* Tv = expose_internal_semi_splay(v);
    #else
    C* Tu = expose_internal(u);
    C* Tv = expose_internal(v);
    #endif

    //Tu now has constant depth 
    C* root_u = Tu;
    int depth = 0;
    while (root_u && root_u->get_parent()) {
        root_u = root_u->get_parent();
        depth++;
    }
    assert(depth <= 5);

    //Compare the root of Tu, Tv if u and v already connected, return null.
    if (root_u == Tv && root_u && Tv) {
        deexpose_internal(u);
        deexpose_internal(v);
        return std::make_tuple(nullptr, nullptr);
    }

    if (Tu && Tu->has_left_boundary()) {
        Tu->flip();
    }
    if (Tv && Tv->has_right_boundary()) {
        Tv->flip();
    }
    u->exposed = false;
    v->exposed = false;

    Edge<C,E,V>* edge = this->underlying_tree.add_edge(u, v, data);
    C* root = new LeafNode<C,E,V>(edge, !!Tu + !!Tv);
    edge->set_leaf_node((LeafNode<C,E,V>*) root);

    if (Tu) {
        InternalNode<C,E,V>* root_new = new InternalNode<C,E,V>(Tu, root, !!Tv);  
        root = root_new;
    }
    if (Tv) {
        InternalNode<C,E,V>* root_new = new InternalNode<C,E,V>(root, Tv, 0);
        root = root_new;
    }
    return std::make_tuple(root, edge);
}

template<class C, class E, class V>
void TopTree<C,E,V>::delete_all_ancestors(C* node) {
    C* parent = node->get_parent();
    if (parent) {
        C* sibling = node->get_sibling();
        delete_all_ancestors(parent);
        sibling->set_parent(nullptr);
    }
    node->split_internal();
    delete node;
}



template<class C, class E, class V>
std::tuple<C*, C*> TopTree<C,E,V>::cut(int u_id, int v_id) {
    assert(this->num_exposed == 0);
    Edge<C,E,V>* e = this->underlying_tree.find_edge(u_id, v_id);
    return this->cut_internal(e);
}

template<class C, class E, class V>
std::tuple<C*, C*> TopTree<C,E,V>::cut_ptr(Edge<C,E,V>* edge) {
    assert(this->num_exposed == 0);
    return this->cut_internal(edge);
}

template<class C, class E, class V>
std::tuple<C*, C*> TopTree<C,E,V>::cut_leaf(C* node) {
    assert(this->num_exposed == 0);
    LeafNode<C,E,V>* leaf_node = (LeafNode<C,E,V> *) node;
    return this->cut_internal(leaf_node->edge);
}
    
template<class C, class E, class V>
std::tuple<C*, C*> TopTree<C,E,V>::cut_internal(Edge<C,E,V>* edge) {
    Vertex<C,E,V>* u = edge->endpoints[0];
    Vertex<C,E,V>* v = edge->endpoints[1];
    edge->node->full_splay();
    this->delete_all_ancestors(edge->node);
    this->underlying_tree.del_edge(edge);    

    u->exposed = true;
    v->exposed = true;
    C* Tu = this->deexpose_internal(u);
    C* Tv = this->deexpose_internal(v);
    
    return std::tuple<C*,C*>(Tu, Tv);
}


//Takes O(index) time
template<class C, class E, class V>
C* TopTree<C,E,V>::get_adjacent_leaf_node(int vertex_id, int index) {
    Vertex<C,E,V>* vertex = this->underlying_tree.get_vertex(vertex_id);
    Edge<C,E,V>* current = vertex->get_first_edge();
    if (!current) {
        return nullptr;
    }
    for (int i = 0; i < index; i++) {
        int is_right_vertex = current->is_right_vertex(vertex);
        current = current->next[is_right_vertex];
        if (!current) {
            return nullptr;
        }
    }
    return current->node;
}

//Takes O(1) time
template<class C, class E, class V>
C* TopTree<C,E,V>::get_adjacent_leaf_node(int vertex_id) {
    return this->get_adjacent_leaf_node(vertex_id, 0);
}

template<class C, class E, class V>
bool TopTree<C,E,V>::connected(int u, int v) {
    assert(this->num_exposed == 0);
    C* Tu = expose(u);
    C* Tv = expose(v);

    C* root_u = Tu;
    int depth = 0;
    while (root_u && root_u->get_parent()) {
        root_u = root_u->get_parent();
        depth++;
    }
    assert(depth <= 5);
    bool result = root_u && Tv && root_u == Tv;
    deexpose(u);
    deexpose(v);
    return result;
}