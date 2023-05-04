#include "edge.h"
#include "two_edge_cluster.h"

#include <vector>
#include <cmath>

using CoverLevel = int;

class TwoEdgeConnectivity {
    void check_levels(int u, int v);
    TopTree<TwoEdgeCluster,NewEdge,VertexLabel> *top_tree;
    std::vector<VertexLabel*> vertex_labels;

    int size();
    NewEdge* swap(NewEdge*);
    NewEdge* find_replacement(int,int,int);
    NewEdge* recover_phase(int, int, int, int);
    NewEdge* find_first_label(int, int, int);
    void recover(int, int, int);
    void add_label(int, NewEdge*);
    void remove_labels(NewEdge*);
    void reassign_vertices(TwoEdgeCluster*);
    int cover_level(int, int);

    public: 
    int two_size();
    int find_size(int, int, int);
    NewEdge* insert(int, int);
    NewEdge* insert(int, int, int); // TODO: SKAL Måske væk
    void remove(NewEdge*); //delete is keyword.
    bool two_edge_connected(int,int);
    NewEdge* find_bridge(int);
    NewEdge* find_bridge(int, int);
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
        TwoEdgeCluster::set_l_max((int) floor(log2 (size))); //TODO
        this->top_tree = new TopTree<TwoEdgeCluster,NewEdge,VertexLabel>(size);
        this->vertex_labels = std::vector<VertexLabel*>(size);
        for (int i = 0; i < size; i++) {
            vertex_labels[i] = new VertexLabel(TwoEdgeCluster::get_l_max());
        }
    };
    ~TwoEdgeConnectivity() {
        // TODO:
        // for (VertexLabel* vertex_label : vertex_labels) {
        //     delete vertex_label;
        // }
    };
};

