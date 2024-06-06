#include "two_edge_connected.h"
#include <tuple>

void TwoEdgeConnectivity::cover(int u, int v, int level) {
    TwoEdgeCluster *root = this->top_tree.expose(u, v);
    root->cover(level);
    this->top_tree.deexpose(u, v);
}

void TwoEdgeConnectivity::uncover(int u, int v, int level) {
    TwoEdgeCluster *root = this->top_tree.expose(u, v);
    root->uncover(level);
    this->top_tree.deexpose(u, v);
}

std::shared_ptr<EdgeData> TwoEdgeConnectivity::insert(int u, int v) {
    //Try to link u,v in tree
    if (u == v) {
        return nullptr;
    }

    TwoEdgeCluster* result = this->top_tree.link_leaf(u, v, TreeEdgeData(u, v, -1)); //TODO level = lmax?
    
    if (result) {
        //If successfull, try to assign vertex endpoints to new leaf
        if (!vertex_labels[u]->leaf_node) {
            result->assign_vertex(u, vertex_labels[u]);
            vertex_labels[u]->leaf_node = result;
        }
        if (!vertex_labels[v]->leaf_node) {
            result->assign_vertex(v, vertex_labels[v]);
            vertex_labels[v]->leaf_node = result;
        }
        result->full_splay();
        result->recompute_root_path();
        return std::make_shared<EdgeData>(u, v, -1, result); // Constructs tree edge, with result leaf node

    }
    std::shared_ptr<EdgeData> edge = std::make_shared<EdgeData>(NonTreeEdge, u, v, 0); // construct level 0 non tree edge
    
    this->add_label(u, edge);
    this->add_label(v, edge);
    this->cover(u, v, 0);
    return edge;
}

std::shared_ptr<EdgeData> TwoEdgeConnectivity::insert(int u, int v, int level) {
    TwoEdgeCluster* result = this->top_tree.link_leaf(u, v, TreeEdgeData(u, v, -1)); //TODO level = lmax?
    if (result) {
        return std::make_shared<EdgeData>(u, v, -1, result); // Constructs tree edge, with result leaf node
    }
    std::shared_ptr<EdgeData> edge = std::make_shared<EdgeData>(NonTreeEdge, u, v, level); // construct level non tree edge
    this->add_label(u, edge);
    this->add_label(v, edge);
    this->cover(u, v, level);
    return edge;
}

void TwoEdgeConnectivity::add_label(int vertex, std::shared_ptr<EdgeData> edge) {
    VertexLabel* vertex_label = this->vertex_labels[vertex];    
    int index = vertex_label->labels[edge->level].size();

    if (edge->endpoints[0] == vertex) {
        edge->extra_data.index[0] = index;
    } else {
        edge->extra_data.index[1] = index;
    }
    vertex_label->labels[edge->level].push_back(edge);
    
    vertex_label->leaf_node->full_splay(); //depth <= 5
    vertex_label->leaf_node->recompute_root_path(); //takes O(depth) = O(1) time
}

void TwoEdgeConnectivity::remove_labels(std::shared_ptr<EdgeData> edge) {
    int level = edge->level;

    for (int i = 0; i < 2; i++) {
        int ep = edge->endpoints[i];
        int ep_idx = edge->extra_data.index[i];

        VertexLabel* ep_label = this->vertex_labels[ep];
        
        std::shared_ptr<EdgeData> last_label = ep_label->labels[level].back();
        int ep_is_right_new = last_label->endpoints[1] == ep;

        ep_label->labels[level][ep_idx] = last_label;
        ep_label->labels[level][ep_idx]->extra_data.index[ep_is_right_new] = ep_idx;
        ep_label->labels[level].pop_back();

        if (ep_label->leaf_node) {
            ep_label->leaf_node->full_splay();
            ep_label->leaf_node->recompute_root_path();
        }
    }
}   

void TwoEdgeConnectivity::reassign_vertices(TwoEdgeCluster* leaf_node) {
    leaf_node->full_splay();
    VertexLabel* old_labels[2] = {leaf_node->vertex[0],leaf_node->vertex[1]};
    leaf_node->vertex[0] = nullptr;
    leaf_node->vertex[1] = nullptr;
    leaf_node->recompute_root_path();
    for (int i = 0; i < 2; i++) {
        if (old_labels[i]) {
            //find new edge
            int id = leaf_node->get_endpoint_id(i);
            TwoEdgeCluster* replacement = this->top_tree.get_adjacent_leaf_node(id);
            if (replacement == leaf_node) {
                replacement = this->top_tree.get_adjacent_leaf_node(id, 1);
                if (!replacement) {
                    old_labels[i]->leaf_node = nullptr;
                    continue;
                }
            
            } 
            
            replacement->full_splay();

            replacement->assign_vertex(id, old_labels[i]);
            old_labels[i]->leaf_node = replacement;

            replacement->recompute_root_path();
        }
    }
}


void TwoEdgeConnectivity::remove(std::shared_ptr<EdgeData> edge) {
    int u = edge->endpoints[0];
    int v = edge->endpoints[1];

    int alpha = edge->level;

    int test = false;
    if (edge->edge_type == TreeEdge) {
        test = true;
        int cover_level = this->cover_level(u, v);
        alpha = cover_level;
        if (cover_level == -1) {
            reassign_vertices(edge->extra_data.leaf_node);
            this->top_tree.cut_leaf(edge->extra_data.leaf_node);
            return;
        }
        edge = this->swap(edge);
    } 
    this->remove_labels(edge);

    this->uncover(u, v, alpha);

    for (int i = alpha; i >= 0; i--) {
        this->recover(v, u, i);
    }
}

int TwoEdgeConnectivity::cover_level(int u, int v) {
    TwoEdgeCluster* root = this->top_tree.expose(u, v);
    int cover_level = root->get_cover_level();
    this->top_tree.deexpose(u, v);
    return cover_level;    
}

std::shared_ptr<EdgeData> TwoEdgeConnectivity::swap(std::shared_ptr<EdgeData> tree_edge) {
    int u = tree_edge->endpoints[0];
    int v = tree_edge->endpoints[1];
    
    int cover_level = this->cover_level(u, v);

    reassign_vertices(tree_edge->extra_data.leaf_node);
    this->top_tree.cut_leaf(tree_edge->extra_data.leaf_node);

    std::shared_ptr<EdgeData> non_tree_edge = find_replacement(u, v, cover_level);
    int x = non_tree_edge->endpoints[0];
    int y = non_tree_edge->endpoints[1];
    this->remove_labels(non_tree_edge);
    
    TwoEdgeCluster* new_leaf = this->top_tree.link_leaf(x, y, TreeEdgeData(x, y, -1));

    //Try to reassign vertices
    new_leaf->full_splay();
    if (!vertex_labels[x]->leaf_node) {
        new_leaf->assign_vertex(x, vertex_labels[x]);
        vertex_labels[x]->leaf_node = new_leaf;
    }
    if (!vertex_labels[y]->leaf_node) {
        new_leaf->assign_vertex(y, vertex_labels[y]);
        vertex_labels[y]->leaf_node = new_leaf;
    }

    new_leaf->recompute_root_path();
    
    non_tree_edge->edge_type = TreeEdge;
    non_tree_edge->level = -1;
    non_tree_edge->extra_data.leaf_node = new_leaf;

    std::shared_ptr<EdgeData> edge = std::make_shared<EdgeData>(NonTreeEdge, u, v, cover_level);
    this->add_label(u, edge);
    this->add_label(v, edge);
    this->cover(u, v, cover_level);
    return edge;
}

int TwoEdgeConnectivity::find_size(int u, int v, int cover_level) {
    TwoEdgeCluster* root = this->top_tree.expose(u);
    if (u != v) {
        root = this->top_tree.expose(v);
    }
    int size;
    if (cover_level >= TwoEdgeCluster::get_l_max()) {
        size = INT32_MAX;
    } else if (root) {
        size = root->get_size(cover_level);
    } else {
        size = 1;
    }
    this->top_tree.deexpose(u);
    if (u != v) {
        this->top_tree.deexpose(v);
    }
    return size;
}

std::shared_ptr<EdgeData> TwoEdgeConnectivity::find_replacement(int u, int v, int cover_level) {
    int size_u = this->find_size(u, u, cover_level);
    int size_v = this->find_size(v, v, cover_level);

    if (size_u <= size_v) {
        return this->recover_phase(u, u, cover_level, size_u);
    } else {
        return this->recover_phase(v, v, cover_level, size_v);
    }
}

std::shared_ptr<EdgeData> TwoEdgeConnectivity::find_first_label(int u, int v, int cover_level) {
    std::shared_ptr<EdgeData> res;
    std::tuple<TwoEdgeCluster*,VertexLabel*> result;
    VertexLabel* label;
    TwoEdgeCluster* label_leaf;

    TwoEdgeCluster* root = this->top_tree.expose(u);
    if (u != v) {
        root = this->top_tree.expose(v);
    }
    if (!root) { // if no root, no tree edges. Look for label in vertex only.
        label = vertex_labels[u];
        if (label->labels[cover_level].size() > 0) {
            res = label->labels[cover_level].back();
        } else {
            res = nullptr;
        }
        goto out;
    }
    root->push_flip();
    result = root->find_first_label(u,v,cover_level);
    label_leaf = std::get<0>(result);
    label = std::get<1>(result);
    if (!label_leaf) {
        res = nullptr;
        goto out;
    }
    label_leaf->full_splay();
    if (!label) {
        res = nullptr;
        goto out;
    }
    res = label->labels[cover_level].back();

    out:
    this->top_tree.deexpose(u);
    if (u != v) {
        this->top_tree.deexpose(v);
    }
    return res;
}

void TwoEdgeConnectivity::recover(int u, int v, int cover_level) {
    int this_size = this->find_size(u,v,cover_level);
    int size = this->find_size(u,v,cover_level) / 2;
    this->recover_phase(u, v, cover_level, size);
    this->recover_phase(v, u, cover_level, size);

}

std::shared_ptr<EdgeData> TwoEdgeConnectivity::recover_phase(int u, int v, int cover_level, int size) {
    std::shared_ptr<EdgeData> label = this->find_first_label(u, v, cover_level);
    int i = 0;
    while (label) {
        int q = label->endpoints[0];
        int r = label->endpoints[1];
        if (!this->top_tree.connected(q, r)) {
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
    if (u == v) {
        return true;
    }
    return (this->top_tree.connected(u,v) && (this->cover_level(u,v) >= 0));
}

TreeEdgeData* TwoEdgeConnectivity::find_bridge(int u, int v) {
    TwoEdgeCluster* root = this->top_tree.expose(u,v);
    TreeEdgeData* bridge;
    if (root->cover_level == -1) {
        bridge = root->min_path_edge;
    } else {
        bridge = nullptr;
    }
    this->top_tree.deexpose(u,v);
    return bridge;
}