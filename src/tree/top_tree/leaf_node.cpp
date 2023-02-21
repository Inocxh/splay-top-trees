#include "top_tree.h"

template<class T> LeafNode<T>::LeafNode(Edge<T>* edge, T user_data) {
    edge->set_leaf_node(this);
    this->edge = edge;
    this->user_data = user_data;
}

template<class T> bool LeafNode<T>::has_middle_boundary() {
    return false;
}
template<class T> bool LeafNode<T>::has_left_boundary() {
    Vertex<T>* endpoint = this->edge->endpoints[this->flipped];
    return endpoint->is_exposed() || !endpoint->has_at_most_one_incident_edge();
}
template<class T> bool LeafNode<T>::has_right_boundary() {
    Vertex<T>* endpoint = this->edge->endpoints[!this->flipped];
    return endpoint->is_exposed() || !endpoint->has_at_most_one_incident_edge();
}
template<class T> bool LeafNode<T>::vertex_is_right(Vertex<T>* vertex) {
    return this->edge->vertex_is_right(vertex) != this->flipped;
}
template<class T> void LeafNode<T>::push_flip() {
    if (!this->flipped) {
        std::swap(this->edge->endpoints[0], this->edge->endpoints[1]);
    }
}