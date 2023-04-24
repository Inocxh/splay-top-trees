#include "two_edge_cluster.h"
#include "edge.h"
#include <cmath>



class TwoEdgeConnectivity {
    TopTree<TwoEdgeCluster,TreeEdge,VertexLabel> *top_tree;
    vector<VertexLabel*> vertex_labels;

    int size();
    void swap();
    void find_replacement();
    void recover();
    void add_label(int, NonTreeEdge);
    void recover_phase();

    public: 
    int two_size();
    TwoEdgeCluster* insert(int, int);
    void remove(); //delete is keyword.
    void two_edge_connected();
    void find_bridge(int);
    void find_bridge(int, int);
    void cover(int, int, int); // TODO: move to private and remove test
    TwoEdgeCluster* insert(int, int, int);
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
    }
    ~TwoEdgeConnectivity() {
        for (VertexLabel* vertex_label : vertex_labels) {
            delete vertex_label;
        }
    };
};

