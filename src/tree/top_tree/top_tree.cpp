#include "top_tree.h"
#include <tuple>

//General Node impls
Node* TopTree::find_consuming_node(Vertex* vertex) {
    if (!vertex->get_first_edge()) {
        return nullptr;
    }
    LeafNode* first_node = (vertex->get_first_edge())->node;
    
    first_node->semi_splay();
    
    if (vertex->has_at_most_one_incident_edge()) {
        return first_node;
    }
    

    bool is_right = first_node->vertex_is_right(vertex);
    bool is_left = !is_right;
    bool is_middle = false;
    Node* last_middle_node = nullptr;

    Node* node = first_node; 
    while (!node->get_parent()) {
        Node* parent = node->get_parent();
        
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

Vertex* TopTree::get_vertex(int id) {
    return this->underlying_tree.get_vertex(id);
}


Node* TopTree::expose(int vertex_id) {
    Vertex* vertex = this->get_vertex(vertex_id);
    return expose_internal(vertex);
}

Node* TopTree::expose_internal(Vertex* vertex) {
    //Vertex* vertex = this->get_vertex(id);

    Node* node = this->find_consuming_node(vertex);
    if (!node) {
        vertex->exposed = true;
        return nullptr;
    }

    while (node->is_path()) {
        //Is this legal?
        InternalNode* node_cast = (InternalNode*) node;

        InternalNode* parent = node->get_parent();
        node_cast->push_flip();
        int node_idx = !node->is_left_child();
        node_cast->children[node_idx]->rotate_up();
        node = parent;
    }

    node->full_splay();

    Node* root;
    while (node) {
        root = node;
        root->num_boundary_vertices += 1;
        node = root->get_parent();
    }  
    vertex->exposed = true;
    return root;
}
Node* TopTree::deexpose_internal(Vertex* vertex) { 
    Node* root = nullptr;

    //Vertex* vertex = this->get_vertex(id);
    Node* node = this->find_consuming_node(vertex); 
    while (node) {
        root = node;
        root->num_boundary_vertices -= 1;
        node = root->get_parent();
    }
    vertex->exposed = false;
    return root;
}

Node* TopTree::deexpose(int vertex_id) { 
    Vertex* vertex = this->get_vertex(vertex_id);
    return deexpose_internal(vertex);
}

Node* TopTree::link(int u_id, int v_id, UserData* data) {
    Vertex* u = this->get_vertex(u_id); 
    Vertex* v = this->get_vertex(v_id); 
    return link_internal(u, v, data);
}

//Assumes u and v in trees with no exposed vertices!
Node* TopTree::link_internal(Vertex* u, Vertex* v, UserData* data) {
    
    Node* Tu = expose_internal(u);
    if (Tu && Tu->has_left_boundary()) {
        Tu->flip();
    }

    Node* Tv = expose_internal(v);
    if (Tv && Tv->has_right_boundary()) {
        Tv->flip();
    }

    Edge* e = this->underlying_tree.add_edge(u, v);
    Node* T = new LeafNode(e, data);
    T->num_boundary_vertices = (Tu != nullptr) + (Tv != nullptr);

    if (Tu) {
        T = new InternalNode(Tu, T);
        T->num_boundary_vertices = (Tv != nullptr);
    }
    if (Tv) {
        T = new InternalNode(T, Tv);
        T->num_boundary_vertices = 0;
    }
    return T;
}

void TopTree::delete_all_ancestors(Node* node) {
    Node* parent = node->get_parent();
    if (parent) {
        Node* sibling = node->get_sibling();
        delete_all_ancestors(parent);
        sibling->set_parent(nullptr);
    }
    delete node;
}



std::tuple<Node*, Node*> TopTree::cut(int u_id, int v_id) {
    Edge* e = this->underlying_tree.find_edge(u_id,v_id);
    return this->cut_internal(e);
}
    
std::tuple<Node*, Node*> TopTree::cut_internal(Edge* edge) {
    Vertex* u = edge->endpoints[0];
    Vertex* v = edge->endpoints[1];

    edge->node->full_splay();
    this->delete_all_ancestors(edge->node);

    u->exposed = true;
    v->exposed = true;
    Node* Tu = this->deexpose_internal(u);
    Node* Tv = this->deexpose_internal(v);
    return std::tuple(Tu,Tv);
}








