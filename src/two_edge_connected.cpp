#include "two_edge_connected.h"

void TwoEdgeConnectivity::cover(int u, int v, int level) {
    TwoEdgeCluster *root = this->top_tree->expose(u, v);
    root->cover(level);
    this->top_tree->deexpose(u, v);
}

void TwoEdgeConnectivity::uncover(int u, int v, int level) {
     TwoEdgeCluster *root = this->top_tree->expose(u, v);
    root->uncover(level);
    this->top_tree->deexpose(u, v);
}

GEdge* TwoEdgeConnectivity::insert(int u, int v) {
    //TODO link_leaf;
    TwoEdgeCluster* result = this->top_tree->link_leaf(u, v, TreeEdge(u, v, -1)); //TODO level = lmax?
    if (result) {
        return new TreeEdge(u, v, -1, result);
    }
    NonTreeEdge* edge = new NonTreeEdge(u, v, 0);
    this->add_label(u, edge);
    this->add_label(v, edge);
    this->cover(u, v, 0);
    return edge;
}

GEdge* TwoEdgeConnectivity::insert(int u, int v, int level) {
    TwoEdgeCluster* result = this->top_tree->link_leaf(u, v,TreeEdge(u, v, -1)); //TODO level = lmax?

    if (result) {
        return new TreeEdge(u, v, -1, result);;
    }
    NonTreeEdge* edge = new NonTreeEdge(u, v, level);
    this->add_label(u, edge);
    this->add_label(v, edge);
    this->cover(u, v, level);
    return edge;
}

void TwoEdgeConnectivity::add_label(int vertex, NonTreeEdge* edge) {
    VertexLabel* vertex_label = this->vertex_labels[vertex];
    int index = vertex_label->labels[edge->level].size();
    if (edge->endpoints[0] == vertex) {
        edge->index[0] = index;
    } else {
        edge->index[1] = index;
    }
    vertex_label->labels[edge->level].push_back(edge);
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

void TwoEdgeConnectivity::remove_labels(NonTreeEdge* edge) {
    int level = edge->level;
    for (int i = 0; i < 2; i++) {
        int ep = edge->endpoints[i];
        int ep_idx = edge->index[i];

        VertexLabel* ep_label = this->vertex_labels[ep];
        
        NonTreeEdge* last_label = ep_label->labels[level].back();
        int ep_is_right_new = last_label->endpoints[1] == ep;

        ep_label->labels[level][ep_idx] = last_label;
        ep_label->labels[level][ep_idx]->index[ep_is_right_new] = ep_idx;
        ep_label->labels[level].pop_back();
    }
    
}   

void TwoEdgeConnectivity::reassign_vertices(TwoEdgeCluster* leaf_node) {
    for (int i = 0; i < 2; i++) {
        if (leaf_node->vertex[i]) {
            //find new edge
            int id = leaf_node->get_endpoint_id(i);
            TwoEdgeCluster* replacement = this->top_tree->get_adjacent_leaf_node(id);
            if (replacement == leaf_node) {
                replacement = this->top_tree->get_adjacent_leaf_node(id, 1);
                if (!replacement) {
                    continue;
                }
            } 
            int i_is_right = id == replacement->get_endpoint_id(1);
            replacement->vertex[i_is_right] = leaf_node->vertex[i];
            leaf_node->vertex[i]->leaf_node = replacement;
            leaf_node->vertex[i] = nullptr;
        }
    }

}


void TwoEdgeConnectivity::remove(GEdge* edge) {
    int u = edge->endpoints[0];
    int v = edge->endpoints[1];

    if (edge->is_tree_edge()) {
        int cover_level = this->cover_level(u, v);
        TreeEdge* tree_edge = (TreeEdge*) edge; //Safe as is_tree_edge is true
        if (cover_level == -1) {
            reassign_vertices(tree_edge->leaf_node);
            top_tree->cut_leaf(tree_edge->leaf_node);
            return;
        }
        edge = (GEdge*) this->swap(tree_edge);
    } 
    NonTreeEdge* non_tree_edge = (NonTreeEdge*) edge;
    this->remove_labels(non_tree_edge);
    this->uncover(u, v, non_tree_edge->level);
    for (int i = non_tree_edge->level; i >= 0; i--) {
        this->recover(v, u, i);
    }

}

int TwoEdgeConnectivity::cover_level(int u, int v) {
    TwoEdgeCluster* root = this->top_tree->expose(u, v);
    int cover_level = root->get_cover_level();
    this->top_tree->deexpose(u, v);
    return cover_level;    
}

NonTreeEdge* TwoEdgeConnectivity::swap(TreeEdge* tree_edge) {
    int u = tree_edge->endpoints[0];
    int v = tree_edge->endpoints[1];
    int cover_level = this->cover_level(u, v);
    this->top_tree->cut_leaf(tree_edge->leaf_node);

    NonTreeEdge* non_tree_edge = find_replacement(u, v, cover_level);
    int x = non_tree_edge->endpoints[0];
    int y = non_tree_edge->endpoints[1];
    this->remove_labels(non_tree_edge);
    delete non_tree_edge;
    
    this->top_tree->link(x, y, TreeEdge(x, y, -1));
    
    NonTreeEdge* edge = new NonTreeEdge(u, v, cover_level);
    this->add_label(u, edge);
    this->add_label(v, edge);
    this->cover(u, v, cover_level);
    return edge;
}

int TwoEdgeConnectivity::find_size(int u, int v, int cover_level) {
    TwoEdgeCluster* root = this->top_tree->expose(u);
    if (u != v) {
        root = this->top_tree->expose(v);
    }
    int size = root->get_size(cover_level);
    this->top_tree->deexpose(u);
    if (u != v) {
        this->top_tree->deexpose(v);
    }
    return size;
}

NonTreeEdge* TwoEdgeConnectivity::find_replacement(int u, int v, int cover_level) {
    int size_u = this->find_size(u, u, cover_level);
    int size_v = this->find_size(v, v, cover_level);
    if (size_u <= size_v) {
        return this->recover_phase(u, u, cover_level, size_u);
    } else {
        return this->recover_phase(v, v, cover_level, size_v);
    }
}

NonTreeEdge* TwoEdgeConnectivity::find_first_label(int u, int v, int cover_level) {
    TwoEdgeCluster* root = this->top_tree->expose(u);
    if (u != v) {
        root = this->top_tree->expose(v);
    }
    VertexLabel* label = root->find_first_label(u, v, cover_level); 
    if (!label) {
        this->top_tree->deexpose(u);
        if (u != v) {
            this->top_tree->deexpose(v);
        }
        return nullptr;
    }
    NonTreeEdge* non_tree_edge = label->labels[cover_level].back();
    this->top_tree->deexpose(u);
    if (u != v) {
        this->top_tree->deexpose(v);
    }
    return non_tree_edge;
}

void TwoEdgeConnectivity::recover(int u, int v, int cover_level) {
    int size = this->find_size(u,v,cover_level) / 2;
    this->recover_phase(u, v, cover_level, size);
    this->recover_phase(v, u, cover_level, size);
}

NonTreeEdge* TwoEdgeConnectivity::recover_phase(int u, int v, int cover_level, int size) {
    NonTreeEdge* label = this->find_first_label(u, v, cover_level);
    while (label) {
        int q = label->endpoints[0];
        int r = label->endpoints[1];
        if (!this->top_tree->connected(q, r)) {
            return label;
        }
        if (this->find_size(q, r, cover_level + 1) <= size) {
            this->remove_labels(label);
            label->level = cover_level + 1;
            this->add_label(q, label);
            this->add_label(r, label);
            this->cover(q, r, cover_level + 1);
        } else {
            this->cover(q, r, cover_level); 
            return nullptr;
        }
        label = this->find_first_label(u, v, cover_level);
    }
    return nullptr;
}

bool TwoEdgeConnectivity::two_edge_connected(int u, int v) {
    return (this->top_tree->connected(u,v) && (this->cover_level(u,v) >= 0));
}

TreeEdge* TwoEdgeConnectivity::find_bridge(int u, int v) {
    TwoEdgeCluster* root = this->top_tree->expose(u,v);
    TreeEdge* bridge;
    if (root->cover_level == -1) {
        bridge = root->min_path_edge;
    } else {
        bridge = nullptr;
    }
    this->top_tree->deexpose(u,v);
    return bridge;
}