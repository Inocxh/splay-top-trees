#include "top_tree.h"

template<class C, class E, class V>
bool Node<C,E,V>::is_point() {
    return num_boundary_vertices < 2;
}

template<class C, class E, class V>
bool Node<C,E,V>::is_path() {
    return num_boundary_vertices == 2;
}

template<class C, class E, class V>
bool Node<C,E,V>::is_right_child() {
    if (!this->parent) {
        return false;
    }
    return this == this->parent->children[!parent->flipped];
}

template<class C, class E, class V>
void Node<C,E,V>::flip() {
    this->flipped = !this->flipped;
}

//Assumes that the Node<C,E,V> can be part of a valid rotation.
template<class C, class E, class V>
void Node<C,E,V>::rotate_up() {
        InternalNode<C,E,V>* parent = this->get_parent();
        InternalNode<C,E,V>* grandparent = parent->get_parent();
        C* sibling = this->get_sibling();
        C* uncle = parent->get_sibling();

        grandparent->push_flip();
        parent->push_flip();

        grandparent->split();
        parent->split();

        bool to_same_side = uncle->is_right_child() == sibling->is_right_child();

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
                flip_grandparent = grandparent->is_right_child() == uncle->is_right_child();
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

        parent->children[!uncle->is_right_child()] = sibling;
        parent->children[uncle->is_right_child()] = uncle;
        parent->flipped = flip_new_parent;
        parent->num_boundary_vertices = new_parent_is_path + 1;
        
        grandparent->children[!uncle->is_right_child()] = (C*) this;
        grandparent->children[uncle->is_right_child()] = parent;

        grandparent->flipped = flip_grandparent;
        this->parent = grandparent;
        uncle->parent = parent;

        parent->merge();
        grandparent->merge();
}

template<class C, class E, class V>
Node<C,E,V>* Node<C,E,V>::semi_splay_step() {
    Node<C,E,V>* node = this;

    while (true) {
        InternalNode<C,E,V>* parent = node->get_parent();
        if (!parent) {
            return nullptr;
        }
        InternalNode<C,E,V>* grandparent = parent->get_parent();
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
        InternalNode<C,E,V>* ggparent = grandparent->get_parent();
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
        
            if (node->is_right_child() == parent->is_right_child()) {
                node->rotate_up();
                return grandparent;
            } else if (parent->is_right_child() == grandparent->is_right_child()) {
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

template<class C, class E, class V>
void Node<C,E,V>::semi_splay() {
    Node<C,E,V>* top = this;
    while (top) {
        top = top->semi_splay_step();
    }
}

template<class C, class E, class V>
void Node<C,E,V>::full_splay() {
    while (true) {
        Node<C,E,V>* top = this->semi_splay_step();
        if (!top) { 
            return;
        }
        top->semi_splay_step();
    }
}

template<class C, class E, class V>
InternalNode<C,E,V>* Node<C,E,V>::get_parent() {
    return this->parent;
}

template<class C, class E, class V>
void Node<C,E,V>::set_parent(InternalNode<C,E,V>* p) {
    this->parent = p;
}

template<class C, class E, class V>
C* Node<C,E,V>::get_sibling() {
    //The parent is *always* an InternalNode<C,E,V>
    InternalNode<C,E,V>* parent = this->get_parent();
    if (!parent) {
        return nullptr;
    }
    //Return children[0] if 'this' is children[1] and vica versa
    return parent->children[parent->children[0] == this];
}



