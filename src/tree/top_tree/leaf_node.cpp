#include "top_tree.h"

 bool LeafNodeBase::has_middle_boundary() {
    return false;
}
bool LeafNodeBase::has_left_boundary() {
    Vertex* endpoint = this->edge->endpoints[this->flipped];
    return endpoint->is_exposed() || !endpoint->has_at_most_one_incident_edge();
}
bool LeafNodeBase::has_right_boundary() {
    Vertex* endpoint = this->edge->endpoints[!this->flipped];
    return endpoint->is_exposed() || !endpoint->has_at_most_one_incident_edge();
}
bool LeafNodeBase::vertex_is_right(Vertex* vertex) {
    return this->edge->vertex_is_right(vertex) != this->flipped;
}
void LeafNodeBase::push_flip() {
    if (!this->flipped) {
        std::swap(this->edge->endpoints[0], this->edge->endpoints[1]);
    }
}