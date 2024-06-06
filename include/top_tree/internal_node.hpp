#include "top_tree.h"

#include <iostream>

template<class C, class E, class V>
InternalNode<C,E,V>::InternalNode(C* left, C* right, int num_boundary) {
    this->parent = nullptr;
    this->flipped = false;
    this->num_boundary_vertices = num_boundary;
    this->children[0] = left;
    this->children[1] = right;
    left->set_parent(this);
    right->set_parent(this);
    this->merge_internal();
}


template<class C, class E, class V>
void InternalNode<C,E,V>::push_flip() {
    if (!this->flipped) {
        return;
    }
    std::swap(this->children[0], this->children[1]);
    this->children[0]->flip();
    this->children[1]->flip();
    this->swap_data();
    this->flipped = false;
}

template<class C, class E, class V>
bool InternalNode<C,E,V>::has_middle_boundary() {
    return this->num_boundary_vertices -
           this->children[0]->is_path() -
           this->children[1]->is_path();
}

template<class C, class E, class V>
bool InternalNode<C,E,V>::has_left_boundary() {
    return this->children[this->flipped]->is_path();
}
 
template<class C, class E, class V>
bool InternalNode<C,E,V>::has_right_boundary() {
    return this->children[!this->flipped]->is_path();
}

template<class C, class E, class V>
void InternalNode<C,E,V>::merge_internal() {
    this->push_flip();

    this->children[0]->push_flip();
    this->children[1]->push_flip();
    
    this->merge(
        this->children[0], 
        this->children[1]
    );
}
template<class C, class E, class V>
void InternalNode<C,E,V>::split_internal() {    
    this->push_flip();

    this->children[0]->push_flip();
    this->children[1]->push_flip();
    this->split(
        this->children[0], 
        this->children[1]
    );
}
template<class C, class E, class V>
C* InternalNode<C,E,V>::get_child(int index) {
    return this->children[index];
}

template<class C, class E, class V>
void InternalNode<C,E,V>::print(int indent, bool was_flip) {
    for (int i = 0; i < indent; i++) {
        std::cerr << "    ";
    }
    this->print_data();
    std::cerr << std::endl;
    this->children[this->flipped != was_flip]->print(indent + 1, this->flipped != was_flip);
    this->children[!this->flipped != was_flip]->print(indent + 1, this->flipped != was_flip);
}

template<class C, class E, class V>
int InternalNode<C,E,V>::get_endpoint_id(int e) {
    return -1;
};

template<class C, class E, class V>
Edge<C,E,V>* InternalNode<C,E,V>::get_edge() {
    return nullptr;
};

template<class C, class E, class V>
void InternalNode<C,E,V>::delete_tree() {
    this->children[0]->delete_tree();
    this->children[1]->delete_tree();
    delete this;
};