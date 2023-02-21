#include "underlying_tree.h"
#include <iostream>

using namespace std;


template<class T> Tree<T>::Tree(int num_vertices) {
    this->num_vertices = num_vertices;
    vector<Vertex<T>> vertices;
    for (int i = 0; i < num_vertices; i++) {
        vertices.push_back(Vertex(i));
    }
    this->vertices = vertices;
}

template<class T> Tree<T>::~Tree() {
    for (int i = 0; i < this->vertices.size(); i++) {
        Edge<T>* current = vertices[i].get_first_edge();
        while (current) {
            this->del_edge(current);
            current = vertices[i].get_first_edge();
        }
    }
}

template<class T> Edge<T>* Tree<T>::add_edge(Vertex<T>* left, Vertex<T>* right) {
    Edge<T>* next[2] = {left->get_first_edge(), right->get_first_edge()};

    //Construct edge and set next
    Edge<T>* edge = new Edge(left, right);
    edge->next[0] = next[0],
    edge->next[1] = next[1];

    left->set_first_edge(edge);
    right->set_first_edge(edge);
    if (next[0]) {
        int vertex_is_right = next[0]->vertex_is_right(left);
        next[0]->prev[vertex_is_right] = edge;
    }
    if (next[1]) {
        int vertex_is_right = next[1]->vertex_is_right(right);
        next[1]->prev[vertex_is_right] = edge;
    }
    return edge;
}

template<class T> Edge<T>* Tree<T>::add_edge(int u, int v) {
    Vertex<T>* left = &this->vertices[u];
    Vertex<T>* right = &this->vertices[v];
    return this->add_edge(left,right);
}

template<class T> void Tree<T>::del_edge(Edge<T>* edge) {
    del_edge_inner(edge->endpoints[0], edge->prev[0], edge->next[0]);
    del_edge_inner(edge->endpoints[1], edge->prev[1], edge->next[1]);

    delete edge;
}

template<class T> void Tree<T>::del_edge_inner(Vertex<T>* vertex, Edge<T>* prev, Edge<T>* next) {
    if (prev) {
        int vertex_is_right = prev->vertex_is_right(vertex);
        prev->next[vertex_is_right] = next;
    } else {
        vertex->set_first_edge(next);
    }
    if (next) {
        int vertex_is_right = next->vertex_is_right(vertex);
        next->prev[vertex_is_right] = prev;
    }
}

template<class T> Edge<T>* Tree<T>::find_edge(int u_id, int v_id) {
    Vertex<T>* u = this->get_vertex(u_id);
    Vertex<T>* v = this->get_vertex(v_id);

    Edge<T>* edge = u->get_first_edge();
    
    int vertex_is_right = edge->vertex_is_right(u);
    while (edge->endpoints[!vertex_is_right] != v) {
        vertex_is_right = edge->vertex_is_right(u);
        edge = edge->next[!vertex_is_right];
    }
    return edge;
}

template<class T> void Tree<T>::print_tree() {
    for (int i = 0; i < this->num_vertices; i++) {
        cout << i << ": "; 
        print_edges(&(this->vertices[i]));
        cout << endl;
    }
}

template<class T> void Tree<T>::print_edges(Vertex<T> *vertex) {
    Edge<T> *current = vertex->get_first_edge();
    while (current) {
        cout << "(" << current->endpoints[0]->id << "," << current->endpoints[1]->id << ") ";
        int vertex_is_right = current->vertex_is_right(vertex);
        current = current->next[vertex_is_right];
    }
}

template<class T> bool Vertex<T>::has_at_most_one_incident_edge() {
    Edge<T> *first_edge = this->get_first_edge();
    if (first_edge) {
        int vertex_is_right = first_edge->vertex_is_right(this);
        return first_edge->next[vertex_is_right] == NULL;
    } else {
        return true;
    }
}

template<class T> Edge<T>::Edge(Vertex<T>* left, Vertex<T>* right) {
    this->endpoints[0] = left;
    this->endpoints[1] = right;
    
    this->node = nullptr;
    for (int i = 0; i < 2; i++) {
        this->prev[i] = nullptr;
        this->next[i] = nullptr;
    }
}


template<class T> int Edge<T>::vertex_is_right(Vertex<T>* vertex) {
    return this->endpoints[1] == vertex;
}

