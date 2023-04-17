#include "connected.h"

void Connected::link(int u, int v) {

    ConnectedCluster* root = top_tree->link(u, v, None());
    //The root only in this top tree, thus the pointer to this will be a unique number.
    root->id = (size_t) root; 
};

void Connected::cut(int u, int v){
    std::tuple<ConnectedCluster*, ConnectedCluster*> roots = top_tree->cut(u, v);
    ConnectedCluster* left = std::get<0>(roots);
    ConnectedCluster* right = std::get<1>(roots);
    if (left)
        left->id = (size_t) left;
    if (right)
        right->id = (size_t) right;
};

bool Connected::connected(int u, int v){
    
    ConnectedCluster* u_root = this->top_tree->expose(u);
    if (!u_root) {
        this->top_tree->deexpose(u);
        return false;
    }
    int u_id = u_root->id;
    this->top_tree->deexpose(u);

    ConnectedCluster* v_root = this->top_tree->expose(v);
    if (!v_root) {
        this->top_tree->deexpose(v);
        return false;
    }
    int v_id = v_root->id;
    this->top_tree->deexpose(v);

    return u_id == v_id;
};

Connected::Connected(int size) {
    this->top_tree = new TopTree<ConnectedCluster,None,None>(size);
};

void ConnectedCluster::create(None* , None* left, None* right) {
    //This does nothing
};
void ConnectedCluster::merge(ConnectedCluster* left, ConnectedCluster* right) {
    //Neither does this
};

void ConnectedCluster::split(ConnectedCluster* left, ConnectedCluster* right) {
    left->id = this->id;
    right->id = this->id;
};
