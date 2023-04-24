#ifndef EDGE
#define EDGE

struct TwoEdgeCluster;

struct GEdge {
    int endpoints[2];

    virtual bool is_tree_edge();

    GEdge() {};
    GEdge(int u, int v) {
        endpoints[0] = u;
        endpoints[1] = v;
    };
};

struct TreeEdge : GEdge {
    int cover_level;
        
    TwoEdgeCluster* leaf_node;

    bool is_tree_edge() {
        return true;
    }
    TreeEdge() {} //TODO: Delete
    TreeEdge(int u, int v){} //TODO:DElete
    TreeEdge(int u, int v, int cl) : GEdge(u, v) {
        this->cover_level = cl;
    }
};

struct NonTreeEdge : GEdge {
    int level;

    bool is_tree_edge() {
        return false;
    }

    NonTreeEdge(int u, int v, int l) : GEdge(u, v) {
        this->level = level;
    }
};



#endif