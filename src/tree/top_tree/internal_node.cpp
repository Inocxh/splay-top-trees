#include "top_tree.h"


InternalNode::InternalNode(Node* left, Node* right) {
    this->children[0] = left;
    this->children[1] = right;
   // this->user_data = new UserData(left->user_data, right->user_data);
    this->user_data = left->user_data->merge(right->user_data);

}

void InternalNode::push_flip() {
    if (!this->flipped) {
        return;
    }
    this->flipped = false;
    std::swap(this->children[0], this->children[1]);
    this->children[0]->flip();
    this->children[1]->flip();
}

bool InternalNode::has_middle_boundary() {
    return this->num_boundary_vertices -
           this->children[0]->is_path() -
           this->children[1]->is_path();
}
bool InternalNode::has_left_boundary() {
    return this->children[this->flipped]->is_path();
}
bool InternalNode::has_right_boundary() {
    return this->children[!this->flipped]->is_path();
}