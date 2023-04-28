#ifndef EDGE
#define EDGE

#include <utility> //Swap

struct TwoEdgeCluster;

enum EdgeType { TreeEdge, NonTreeEdge };
union EdgeData {
    TwoEdgeCluster* leaf_node;
    int index[2];
};
// TODO: CHANGE NewEdge name and TreeEdge1 and NonTreeEdge1
struct NewEdge {
    EdgeType edge_type;
    int endpoints[2];  
    int level; //If TreeEdge cover_level, if NonTreeEdge level
    EdgeData extra_data;
    
    static void swap(NewEdge* u, NewEdge* v) {
        std::swap(u->edge_type, v->edge_type);
        std::swap(u->endpoints[0], v->endpoints[0]);
        std::swap(u->endpoints[1], v->endpoints[1]);
        std::swap(u->level, v->level);
        std::swap(u->extra_data, v->extra_data);
    };
    
    static NewEdge tree_edge(int u, int v, int cl, TwoEdgeCluster* leaf_node) {
        NewEdge edge = NewEdge(TreeEdge, u, v, cl);
        edge.extra_data.leaf_node = leaf_node;
        return edge;
    };

    static NewEdge* new_tree_edge(int u, int v, int cl, TwoEdgeCluster* leaf_node) {
        NewEdge* edge = new NewEdge(TreeEdge, u, v, cl);
        edge->extra_data.leaf_node = leaf_node;
        return edge;
    };
    static NewEdge* new_non_tree_edge(int u, int v, int level) {
        return new NewEdge(NonTreeEdge, u, v, level);
    };

    NewEdge(int u, int v) {
        edge_type = TreeEdge;
        endpoints[0] = u;
        endpoints[1] = v;
        level = -1;
    };

    NewEdge() {
        edge_type = TreeEdge;
        level = -1;
    };
    private:
    NewEdge(EdgeType t, int u, int v, int cl) {
        edge_type = t;
        endpoints[0] = u;
        endpoints[1] = v;
        level = cl;
    };


};

// struct GEdge {
//     int endpoints[2];

//     virtual bool is_tree_edge() = 0;

//     GEdge() {}; //TODO DELETE
//     GEdge(int u, int v) {
//         endpoints[0] = u;
//         endpoints[1] = v;
//     }
//     ~GEdge() {};
// };

// struct TreeEdge : GEdge {
//     int cover_level = -1;
        
//     TwoEdgeCluster* leaf_node;

//     bool is_tree_edge() {
//         return true;
//     }
//     TreeEdge() {}; //TODO: Delete
//     TreeEdge(int u, int v) : GEdge(u,v) {}; //TODO:DElete
//     TreeEdge(int u, int v, int cl) : GEdge(u, v) {
//         this->cover_level = cl;
//     }
//     TreeEdge(int u, int v, int cl, TwoEdgeCluster* leaf_node) : GEdge(u, v) {
//         this->cover_level = cl;
//         this->leaf_node = leaf_node;
//     }
//     ~TreeEdge() {};
// };

// struct NonTreeEdge : GEdge {
//     int level;
//     int index[2];

//     bool is_tree_edge() {
//         return false;
//     }

//     NonTreeEdge(int u, int v, int l) : GEdge(u, v) {
//         this->level = l;
//     }
//     ~NonTreeEdge() {};
// };



#endif