// This file contains implementations of methods defined in underlying_tree.h

//Only for syntax highlighting 
#include "underlying_tree.h"

template<class C, class E, class V>
Vertex<C,E,V>::Vertex(int id) {
    this->id = id;
    this->first_edge = nullptr;
    this->exposed = false;
}
template<class C, class E, class V>
Edge<C,E,V>* Vertex<C,E,V>::get_first_edge() {
    return this->first_edge; 
};

template<class C, class E, class V>
void Vertex<C,E,V>::set_first_edge(Edge<C, E, V>* e) {
    this->first_edge = e;
};

template<class C, class E, class V>
bool Vertex<C,E,V>::has_at_most_one_incident_edge() {
    Edge<C,E,V>* edge = this->first_edge;
    if (!edge) {
        return true;
    }
    int is_right_vertex = edge->is_right_vertex(this);
    //In memory of "return true"
    return !edge->get_next(is_right_vertex); 
};

template<class C, class E, class V>
bool Vertex<C,E,V>::is_exposed() {
    return this->exposed;
};

template<class C, class E, class V>
V* Vertex<C,E,V>::get_data() {
    return &(this->vertex_data);
};
template<class C, class E, class V>
int Vertex<C,E,V>::get_id() {
    return this->id;
};

template<class C, class E, class V>
Edge<C,E,V>::Edge(Vertex<C,E,V>* left, Vertex<C,E,V>* right, E data) {
    this->edge_data = data;

    this->endpoints[0] = left;
    this->endpoints[1] = right;
    
    this->node = nullptr;
    for (int i = 0; i < 2; i++) {
        this->prev[i] = nullptr;
        this->next[i] = nullptr;
    }
};
template<class C, class E, class V>
int Edge<C,E,V>::is_right_vertex(Vertex<C,E,V>* vertex) {
    return this->endpoints[1] == vertex;
};

template<class C, class E, class V>
void Edge<C,E,V>::set_leaf_node(LeafNode<C,E,V>* leaf) {
      this->node = leaf;
}; 

template<class C, class E, class V>
Vertex<C,E,V>* Edge<C,E,V>::get_endpoint(int i){
    return this->endpoints[i];
};
template<class C, class E, class V>
Edge<C,E,V>* Edge<C,E,V>::get_next(int i){
    return this->next[i];
};
template<class C, class E, class V>
Edge<C,E,V>* Edge<C,E,V>::get_prev(int i){
    return this->prev[i];
};

template<class C, class E, class V>
E* Edge<C,E,V>::get_data(){
    return &(this->edge_data);
};

template<class C, class E, class V>
void Edge<C,E,V>::flip(){
    std::swap(this->endpoints[0], this->endpoints[1]);
    std::swap(this->next[0], this->next[1]);
    std::swap(this->prev[0], this->prev[1]);
};












