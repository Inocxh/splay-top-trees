#include "top_tree.h"

template<class T>
bool Node<T>::is_point() {
    return num_boundary_vertices < 2;
}

template<class T>
bool Node<T>::is_path() {
    return num_boundary_vertices == 2;
}
template<class T>
void Node<T>::flip() {
    this->flipped = !this->flipped;
}
template<class T>
InternalNode<T>* Node<T>::get_parent() {
    return this->parent;
}

template<class T>
void Node<T>::set_parent(InternalNode<T>* p) {
    this->parent = p;
}

template<class T>
Node<T>* Node<T>::get_sibling() {
    //The parent is *always* an InternalNode
    InternalNode<T>* parent = this->get_parent();
    if (!parent) {
        return nullptr;
    }
    //Return children[0] if 'this' is children[1] and vica versa
    return parent->children[parent->children[0] == this];
}
template<class T>
bool Node<T>::is_left_child() {
    if (!this->parent) {
        return false;
    }
    return this == this->parent->children[parent->flipped];
}
//Assumes that the node can be part of a valid rotation.
template<class T>
void Node<T>::rotate_up() {
    InternalNode<T>* parent = this->get_parent();
    InternalNode<T>* grandparent = parent->get_parent();
    Node<T>* sibling = this->get_sibling();
    Node<T>* uncle = parent->get_sibling();
    
    grandparent->push_flip();
    parent->push_flip();
    
    bool uncle_is_left_child = uncle->is_left_child();
    bool sibling_is_left_child = sibling->is_left_child();

    bool to_same_side = uncle_is_left_child == sibling_is_left_child;

    bool sibling_is_path = sibling->is_path();
    bool uncle_is_path = uncle->is_path();
    bool gp_is_path = grandparent->is_path();

    bool new_parent_is_path, flip_new_parent, flip_grandparent;

    //Rotation on path
    if (to_same_side && sibling_is_path) {
        bool gp_middle = grandparent->has_middle_boundary();
        new_parent_is_path = gp_middle || uncle_is_path;
        flip_new_parent = false;
        flip_grandparent = false;
        if (gp_middle && !gp_is_path && grandparent->parent) {
            //Equivalent to pseudo-code from 'Splay Top Trees'
            flip_grandparent = grandparent->is_left_child() == uncle_is_left_child;
        }         
    //Rotation on star
    } else if (to_same_side) {
        new_parent_is_path = uncle_is_path;
        flip_new_parent = false;
        flip_grandparent = false;
        sibling->flip();
    } else {
        new_parent_is_path = sibling_is_path || uncle_is_path;
        flip_new_parent = sibling_is_path;
        flip_grandparent = sibling_is_path;
        this->flip();
    }

    parent->children[uncle_is_left_child] = sibling;
    parent->children[!uncle_is_left_child] = uncle;
    parent->flipped = flip_new_parent;
    parent->num_boundary_vertices = new_parent_is_path + 1;
    
    grandparent->children[uncle_is_left_child] = this;
    grandparent->children[!uncle_is_left_child] = parent;

    grandparent->flipped = flip_grandparent;
    this->parent = grandparent;
    uncle->parent = parent;

    //Using user datas
}
template<class T>
Node<T>* Node<T>::semi_splay_step() {
    Node* node = this;

    while (true) {
        InternalNode<T>* parent = node->get_parent();
        if (!parent) {
            return nullptr;
        }
        InternalNode<T>* grandparent = parent->get_parent();
        if (!grandparent) {
            return nullptr;
        }
        // b0   , b1    , b2
        // point, point, point
        // point, path,  point
        if (node->is_point() && grandparent->is_point()) {
            node->rotate_up();
            return grandparent;            
        }
        InternalNode<T>* ggparent = grandparent->get_parent();
        if (!ggparent) {
            return nullptr;
        }
        // b1,   b2,    b3
        // path, path,  point
        // path, point, point
        // path, path,  path 
        if (parent->is_path() && (grandparent->is_path() || ggparent->is_point())) {
            grandparent->push_flip();
            parent->push_flip();
        
            if (node->is_left_child() == parent->is_left_child()) {
                node->rotate_up();
                return grandparent;
            } else if (parent->is_left_child() == grandparent->is_left_child()) {
                parent->rotate_up();
                return ggparent;
            } else {
                node->get_sibling()->rotate_up();
                parent->rotate_up();
                return ggparent;
            }
        }
        //Recurse on parent. This process is guaranteed to stop by Lemma 5.2 'Splay Top Trees'
        node = parent;
    }
}
template<class T>
void Node<T>::semi_splay() {
    Node* top = this;
    while (top) {
        top = top->semi_splay_step();
    }
}
template<class T>
void Node<T>::full_splay() {
    while (true) {
        Node<T>* top = this->semi_splay_step();
        if (!top) { 
            return;
        }
        top->semi_splay_step();
    }
}