#ifndef EDGE
#define EDGE

#include <algorithm> //Swap
#include <cassert>

struct TwoEdgeCluster;

enum EdgeType { TreeEdge, NonTreeEdge };
union ExtraData {
    TwoEdgeCluster* leaf_node;
    int index[2];
};

struct EdgeData {
    EdgeType edge_type;
    int endpoints[2];  
    int level; //If TreeEdge cover_level, if NonTreeEdge level
    ExtraData extra_data;
    
    static void swap(EdgeData* u, EdgeData* v) {
        std::swap(u->edge_type, v->edge_type);
        std::swap(u->endpoints[0], v->endpoints[0]);
        std::swap(u->endpoints[1], v->endpoints[1]);
        std::swap(u->level, v->level);
        std::swap(u->extra_data, v->extra_data);
    };
    
    static EdgeData tree_edge(int u, int v, int cl, TwoEdgeCluster* leaf_node) {
        EdgeData edge = EdgeData(TreeEdge, u, v, cl);
        edge.extra_data.leaf_node = leaf_node;
        return edge;
    };

    static EdgeData* new_tree_edge(int u, int v, int cl, TwoEdgeCluster* leaf_node) {
        EdgeData* edge = new EdgeData(TreeEdge, u, v, cl);
        edge->extra_data.leaf_node = leaf_node;
        return edge;
    };
    static EdgeData* new_non_tree_edge(int u, int v, int level) {
        return new EdgeData(NonTreeEdge, u, v, level);
    };

    EdgeData(int u, int v) {
        edge_type = TreeEdge;
        endpoints[0] = u;
        endpoints[1] = v;
        level = -1;
    };

    EdgeData() {
        edge_type = TreeEdge;
        level = -1;
    };
    private:
    EdgeData(EdgeType t, int u, int v, int cl) {
        edge_type = t;
        endpoints[0] = u;
        endpoints[1] = v;
        level = cl;
    };
};

#endif