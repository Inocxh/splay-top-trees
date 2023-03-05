#include "top_tree.h"

template<class C, class E, class V>
InternalNode<C,E,V>::InternalNode(C* left, C* right, int num_boundary) {
    this->parent = nullptr;
    this->flipped = false;
    this->num_boundary_vertices = num_boundary;
    this->children[0] = left;
    this->children[1] = right;
    left->set_parent(this);
    right->set_parent(this);
    this->merge();
}


template<class C, class E, class V>
void InternalNode<C,E,V>::push_flip() {
    if (!this->flipped) {
        return;
    }
    this->flipped = false;
    std::swap(this->children[0], this->children[1]);
    this->children[0]->flip();
    this->children[1]->flip();
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
void InternalNode<C,E,V>::merge() {
    this->push_flip();
    /*this->merge_internal(
        this->children[this->flipped], 
        this->children[!this->flipped]
    );*/
    this->merge_internal(
        this->children[0], 
        this->children[1]
    );
}
template<class C, class E, class V>
void InternalNode<C,E,V>::split() {
    this->split_internal(
        this->children[this->flipped], 
        this->children[!this->flipped]
    );
}

template<class C, class E, class V>
void InternalNode<C,E,V>::print(int indent, bool was_flip) {
    for (int i = 0; i < indent; i++) {
        std::cout << "    ";
    }
    this->print_data();
    std::cout << std::endl;
    this->children[this->flipped != was_flip]->print(indent + 1, this->flipped != was_flip);
    this->children[!this->flipped != was_flip]->print(indent + 1, this->flipped != was_flip);
}