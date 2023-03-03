
#include <iostream>
#include "underlying_tree.h"

template<class C, class E, class V>
Tree<C,E,V>::Tree(int num_vertices) {
    std::vector<Vertex<C,E,V>> vertices;
    for (int i = 0; i < num_vertices; i++) {
        vertices.push_back(Vertex<C,E,V>(i));
    }
    this->vertices = vertices;
};
template<class C, class E, class V>
Tree<C,E,V>::~Tree() {
    for (int i = 0; i < this->vertices.size(); i++) {
        Edge<C,E,V>* current = vertices[i].get_first_edge();
        while (current) {
            this->del_edge(current);
            current = vertices[i].get_first_edge();
        }
    }
};

template<class C, class E, class V>
Edge<C, E, V>* Tree<C,E,V>::add_edge(Vertex<C, E, V>* left, Vertex<C, E, V>* right, E data) {
    Edge<C,E,V>* next[2] = { left->get_first_edge(), right->get_first_edge() };

    //Construct edge and set next
    Edge<C,E,V>* edge = new Edge<C,E,V>(left, right, data);
    edge->next[0] = next[0],
    edge->next[1] = next[1];

    left->set_first_edge(edge);
    right->set_first_edge(edge);
    if (next[0]) {
        int is_right_vertex = next[0]->is_right_vertex(left);
        next[0]->prev[is_right_vertex] = edge;
    }
    if (next[1]) {
        int is_right_vertex = next[1]->is_right_vertex(right);
        next[1]->prev[is_right_vertex] = edge;
    }
    return edge;
};

template<class C, class E, class V>
Edge<C,E,V>* Tree<C,E,V>::add_edge(int u, int v, E data) {
    Vertex<C,E,V>* left = this->get_vertex(u);
    Vertex<C,E,V>* right = this->get_vertex(v);
    return this->add_edge(left, right, data);
};

template<class C, class E, class V>
Edge<C, E, V>* Tree<C,E,V>::find_edge(int u_id, int v_id) {
    Vertex<C,E,V>* u = this->get_vertex(u_id);
    Vertex<C,E,V>* v = this->get_vertex(v_id);
    
    Edge<C,E,V>* edge = u->get_first_edge();
    
    int is_right_vertex = edge->is_right_vertex(u);
    while (edge->endpoints[!is_right_vertex] != v) {
        is_right_vertex = edge->is_right_vertex(u);
        edge = edge->next[!is_right_vertex];
    }
    return edge;
};

template<class C, class E, class V>
void Tree<C,E,V>::del_edge(Edge<C,E,V>* edge) {
    del_edge_inner(edge->endpoints[0], edge->prev[0], edge->next[0]);
    del_edge_inner(edge->endpoints[1], edge->prev[1], edge->next[1]);

    delete edge;
}

template<class C, class E, class V>
void Tree<C,E,V>::del_edge_inner(Vertex<C, E, V>* vertex, Edge<C, E, V>* prev, Edge<C, E, V>* next) {
    if (prev) {
        int is_right_vertex = prev->is_right_vertex(vertex);
        prev->next[is_right_vertex] = next;
    } else {
        vertex->set_first_edge(next);
    }
    if (next) {
        int is_right_vertex = next->is_right_vertex(vertex);
        next->prev[is_right_vertex] = prev;
    }
};

template<class C, class E, class V>
void Tree<C,E,V>::print_tree() {
    for (int i = 0; i < this->get_size(); i++) {
        std::cout << i << ": "; 
        print_edges(&(this->vertices[i]));
        std::cout << std::endl;
    }
};
template<class C, class E, class V>
void Tree<C,E,V>::print_edges(Vertex<C, E, V>* vertex) {
    Edge<C,E,V> *current = vertex->get_first_edge();
    while (current) {
        std::cout << "(" << current->endpoints[0]->id << "," << current->endpoints[1]->id << ") ";
        int is_right_vertex = current->is_right_vertex(vertex);
        current = current->next[is_right_vertex];
    }
};

template<class C, class E, class V>
std::vector<Vertex<C, E, V>>* Tree<C,E,V>::get_vertices() {
    return &this->vertices;
};

template<class C, class E, class V>
Vertex<C, E, V>* Tree<C,E,V>::get_vertex(int id) {
    return &this->vertices[id];
};

template<class C, class E, class V>
int Tree<C,E,V>::get_size() {
    return this->vertices.size();
};