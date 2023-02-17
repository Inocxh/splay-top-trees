#include "top_tree.h"


//General Node impls
Node* find_consuming_node(Vertex* vertex) {
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
                    is_left || (is_middle && !node->has_left_boundary());
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

