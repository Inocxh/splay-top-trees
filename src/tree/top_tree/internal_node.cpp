#include "top_tree.h"


void InternalNodeBase::push_flip() {
    if (!this->flipped) {
        return;
    }
    this->flipped = false;
    std::swap(this->children[0], this->children[1]);
    this->children[0]->flip();
    this->children[1]->flip();
}

bool InternalNodeBase::has_middle_boundary() {
    return this->num_boundary_vertices -
           this->children[0]->is_path() -
           this->children[1]->is_path();
}

bool InternalNodeBase::has_left_boundary() {
    return this->children[this->flipped]->is_path();
}
 
bool InternalNodeBase::has_right_boundary() {
    return this->children[!this->flipped]->is_path();
}