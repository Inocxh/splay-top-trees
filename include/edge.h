#ifndef EDGE
#define EDGE

struct TwoEdgeCluster;

struct GEdge {
    int endpoints[2];

    virtual bool is_tree_edge() = 0;

    GEdge() {}; //TODO DELETE
    GEdge(int u, int v) {
        endpoints[0] = u;
        endpoints[1] = v;
    }
    ~GEdge() {};
};

struct TreeEdge : GEdge {
    int cover_level = -1;
        
    TwoEdgeCluster* leaf_node;

    bool is_tree_edge() {
        return true;
    }
    TreeEdge() {}; //TODO: Delete
    TreeEdge(int u, int v) : GEdge(u,v) {}; //TODO:DElete
    TreeEdge(int u, int v, int cl) : GEdge(u, v) {
        this->cover_level = cl;
    }
    TreeEdge(int u, int v, int cl, TwoEdgeCluster* leaf_node) : GEdge(u, v) {
        this->cover_level = cl;
        this->leaf_node = leaf_node;
    }
    ~TreeEdge() {};
};

struct NonTreeEdge : GEdge {
    int level;
    int index[2];

    bool is_tree_edge() {
        return false;
    }

    NonTreeEdge(int u, int v, int l) : GEdge(u, v) {
        this->level = l;
    }
    ~NonTreeEdge() {};
};



#endif