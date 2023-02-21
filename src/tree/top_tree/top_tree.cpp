#include "top_tree.h"
#include <tuple>

//General Node impls
template<class T>
TopTree<T>::TopTree(int size) {
    this->underlying_tree<T> = Tree<T>(size);
}

template<class T>
Node<T>* TopTree<T>::find_consuming_node(Vertex<T>* vertex) {
    if (!vertex->get_first_edge()) {
        return nullptr;
    }
    LeafNode<T>* first_node = (vertex->get_first_edge())->node;
    
    first_node->semi_splay();
    
    if (vertex->has_at_most_one_incident_edge()) {
        return first_node;
    }
    

    bool is_right = first_node->vertex_is_right(vertex);
    bool is_left = !is_right;
    bool is_middle = false;
    Node<T>* last_middle_node = nullptr;

    Node<T>* node = first_node; 
    while (!node->get_parent()) {
        Node<T>* parent = node->get_parent();
        
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
Vertex<T>* TopTree<T>::get_vertex(int id) {
    return this->underlying_tree.get_vertex(id);
}


template<class T>
Node<T>* TopTree<T>::expose(int vertex_id) {
    Vertex<T>* vertex = this->get_vertex(vertex_id);
    return expose_internal(vertex);
}

template<class T>
Node<T>* TopTree<T>::expose_internal(Vertex<T>* vertex) {
    Node<T>* node = this->find_consuming_node(vertex);
    if (!node) {
        vertex->exposed = true;
        return nullptr;
    }

    while (node->is_path()) {
        //Is this legal?
        InternalNode<T>* node_cast = (InternalNode<T>*) node;

        InternalNode<T>* parent = node->get_parent();
        node_cast->push_flip();
        int node_idx = !node->is_left_child();
        node_cast->children[node_idx]->rotate_up();
        node = parent;
    }

    node->full_splay();

    Node<T>* root;
    while (node) {
        root = node;
        root->num_boundary_vertices += 1;
        node = root->get_parent();
    }  
    vertex->exposed = true;
    return root;
}
template<class T>
Node<T>* TopTree<T>::deexpose_internal(Vertex<T>* vertex) { 
    Node<T>* root = nullptr;

    //Vertex<T>* vertex = this->get_vertex(id);
    Node<T>* node = this->find_consuming_node(vertex); 
    while (node) {
        root = node;
        root->num_boundary_vertices -= 1;
        node = root->get_parent();
    }
    vertex->exposed = false;
    return root;
}

template<class T>
Node<T>* TopTree<T>::deexpose(int vertex_id) { 
    Vertex<T>* vertex = this->get_vertex(vertex_id);
    return deexpose_internal(vertex);
}

template<class T>
Node<T>* TopTree<T>::link(int u_id, int v_id, T data) {
    Vertex<T>* u = this->get_vertex(u_id); 
    Vertex<T>* v = this->get_vertex(v_id); 
    return link_internal(u, v, data);
}

//Assumes u and v in trees with no exposed vertices!
template<class T>
Node<T>* TopTree<T>::link_internal(Vertex<T>* u, Vertex<T>* v, T data) {
    
    Node<T>* Tu = expose_internal(u);
    if (Tu && Tu->has_left_boundary()) {
        Tu->flip();
    }

    Node<T>* Tv = expose_internal(v);
    if (Tv && Tv->has_right_boundary()) {
        Tv->flip();
    }

    Edge<T>* e = this->underlying_tree.add_edge(u, v);
    Node<T>* tree = new LeafNode(e, data);
    tree->num_boundary_vertices = (Tu != nullptr) + (Tv != nullptr);

    if (Tu) {
        tree = new InternalNode(Tu, tree);
        tree->num_boundary_vertices = (Tv != nullptr);
    }
    if (Tv) {
        tree = new InternalNode(tree, Tv);
        tree->num_boundary_vertices = 0;
    }
    return tree;
}

template<class T>
void TopTree<T>::delete_all_ancestors(Node<T>* node) {
    Node<T>* parent = node->get_parent();
    if (parent) {
        Node<T>* sibling = node->get_sibling();
        delete_all_ancestors(parent);
        sibling->set_parent(nullptr);
    }
    delete node;
}



template<class T>
std::tuple<Node<T>*, Node<T>*> TopTree<T>::cut(int u_id, int v_id) {
    Edge<T>* e = this->underlying_tree.find_edge(u_id,v_id);
    return this->cut_internal(e);
}
    
template<class T>
std::tuple<Node<T>*, Node<T>*> TopTree<T>::cut_internal(Edge<T>* edge) {
    Vertex<T>* u = edge->endpoints[0];
    Vertex<T>* v = edge->endpoints[1];

    edge->node->full_splay();
    this->delete_all_ancestors(edge->node);

    u->exposed = true;
    v->exposed = true;
    Node<T>* Tu = this->deexpose_internal(u);
    Node<T>* Tv = this->deexpose_internal(v);
    return std::tuple(Tu,Tv);
}








