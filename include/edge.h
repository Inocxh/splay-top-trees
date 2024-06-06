#ifndef EDGE
#define EDGE

#include <algorithm> //Swap
#include <cassert>
#include <memory>

struct TwoEdgeCluster;

enum EdgeType { TreeEdge, NonTreeEdge };
union ExtraData {
    TwoEdgeCluster* leaf_node;
    int index[2];
};

struct TreeEdgeData {
    int endpoints[2];  
    int level;

    TreeEdgeData(int u, int v, int level) {
        this->endpoints[0] = u;
        this->endpoints[1] = v;
        this->level = level;
    };
    TreeEdgeData() {
        this->endpoints[0] = -1;
        this->endpoints[1] = -1;
        this->level = -1;
    };
};

struct EdgeData {
    EdgeType edge_type;
    int endpoints[2];  
    int level; //If TreeEdge cover_level, if NonTreeEdge level
    ExtraData extra_data;
    
    EdgeData(int u, int v, int cl, TwoEdgeCluster* leaf_node) { // Specific tree edge constructor
        this->edge_type = TreeEdge;
        this->endpoints[0] = u;
        this->endpoints[1] = v;
        this->level = cl;
        this->extra_data.leaf_node = leaf_node;
    };
    EdgeData() {
        this->edge_type = TreeEdge;
        this->level = -1;
    };
    EdgeData(int u, int v) {
        this->edge_type = TreeEdge;
        this->endpoints[0] = u;
        this->endpoints[1] = v;
        this->level = -1;
    };
    EdgeData(EdgeType t, int u, int v, int cl) {
        this->edge_type = t;
        this->endpoints[0] = u;
        this->endpoints[1] = v;
        this->level = cl;
    };
};

#endif