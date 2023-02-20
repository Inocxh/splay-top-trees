#include "top_tree.h"

LeafNode::LeafNode(Edge* edge, UserData* user_data) {
    edge->set_leaf_node(this);
    this->edge = edge;
    this->user_data = user_data;
}

bool LeafNode::has_middle_boundary() {
    return false;
}
bool LeafNode::has_left_boundary() {
    Vertex* endpoint = this->edge->endpoints[this->flipped];
    return endpoint->is_exposed() || !endpoint->has_at_most_one_incident_edge();
}
bool LeafNode::has_right_boundary() {
    Vertex* endpoint = this->edge->endpoints[!this->flipped];
    return endpoint->is_exposed() || !endpoint->has_at_most_one_incident_edge();
}
bool LeafNode::vertex_is_right(Vertex* vertex) {
    return this->edge->vertex_is_right(vertex) != this->flipped;
}
void LeafNode::push_flip() {
    if (!this->flipped) {
        std::swap(this->edge->endpoints[0], this->edge->endpoints[1]);
    }
}