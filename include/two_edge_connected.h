#include "two_edge_cluster.h"
#include "edge.h"
#include <cmath>

using CoverLevel = int;

class TwoEdgeConnectivity {
    TopTree<TwoEdgeCluster,TreeEdge,VertexLabel> *top_tree;
    vector<VertexLabel*> vertex_labels;

    int size();
    NonTreeEdge* swap(TreeEdge*);
    NonTreeEdge* find_replacement(int,int,int);
    NonTreeEdge* recover_phase(int, int, int, int);
    NonTreeEdge* find_first_label(int, int, int);
    void recover(int, int, int);
    void add_label(int, NonTreeEdge*);
    void remove_labels(NonTreeEdge*);
    void reassign_vertices(TwoEdgeCluster*);
    int cover_level(int, int);

    public: 
    int two_size();
    int find_size(int, int, int);
    GEdge* insert(int, int);
    GEdge* insert(int, int, int); // TODO: SKAL Måske væk
    void remove(GEdge*); //delete is keyword.
    bool two_edge_connected(int,int);
    TreeEdge* find_bridge(int);
    TreeEdge* find_bridge(int, int);
    void cover(int, int, int); // TODO: move to private and remove test
    void uncover(int, int, int); // TODO: move to private and remove test
    
    TwoEdgeCluster* expose(int u) {
        return this->top_tree->expose(u);
    };
    TwoEdgeCluster* expose(int u, int v) {
        return this->top_tree->expose(u, v);
    };
    TwoEdgeCluster* deexpose(int u) {
        return this->top_tree->deexpose(u);
    };
    TwoEdgeCluster* deexpose(int u, int v) {
        return this->top_tree->deexpose(u, v);
    };

    TwoEdgeConnectivity();
    TwoEdgeConnectivity(int size) {
        TwoEdgeCluster::set_l_max((int) floor(log2(size)));
        this->top_tree = new TopTree<TwoEdgeCluster, TreeEdge, VertexLabel>(size);
        this->vertex_labels = vector<VertexLabel*>(size);
        for (int i = 0; i < size; i++) {
            vertex_labels[i] = new VertexLabel();
        }
    };
    ~TwoEdgeConnectivity() {
        // TODO:
        // for (VertexLabel* vertex_label : vertex_labels) {
        //     delete vertex_label;
        // }
    };
};

