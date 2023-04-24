#include "two_edge_connected.h"

void TwoEdgeConnectivity::cover(int u, int v, int level) {
    TwoEdgeCluster *root = this->top_tree->expose(u, v);
    root->cover(level);
    this->top_tree->deexpose(u, v);
}

TwoEdgeCluster* TwoEdgeConnectivity::insert(int u, int v) {
    TwoEdgeCluster* result = this->top_tree->link(u, v, TreeEdge(u,v,-1)); //TODO level = lmax?
    if (result) {
        return result;
    }
    NonTreeEdge edge = NonTreeEdge(u, v, 0);
    this->add_label(u, edge);
    this->add_label(v, edge);
    this->cover(u, v, 0);
    return nullptr;
}

TwoEdgeCluster* TwoEdgeConnectivity::insert(int u, int v, int level) {
    TwoEdgeCluster* result = this->top_tree->link(u, v, TreeEdge(u,v,-1)); //TODO level = lmax?
    if (result) {
        return result;
    }
    NonTreeEdge edge = NonTreeEdge(u, v, level);
    this->add_label(u, edge);
    this->add_label(v, edge);
    this->cover(u, v, level);
    return nullptr;
}

void TwoEdgeConnectivity::add_label(int vertex, NonTreeEdge edge) {
    VertexLabel* vertex_label = this->vertex_labels[vertex];
    vertex_label->labels[edge.level].push_back(edge);
    TwoEdgeCluster* leaf;
    if (vertex_label->leaf_node) {
        leaf = vertex_label->leaf_node;
    } else {
        leaf = this->top_tree->get_adjacent_leaf_node(vertex);
        leaf->assign_vertex(vertex,vertex_label);
    }
    
    leaf->full_splay(); //depth <= 5
    leaf->recompute_root_path(); //takes O(depth) = O(1) time
}

