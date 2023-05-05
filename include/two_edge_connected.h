#include "edge.h"
#include "two_edge_cluster.h"

#include <vector>
#include <cmath>

using CoverLevel = int;

class TwoEdgeConnectivity {
    TopTree<TwoEdgeCluster,TreeEdgeData,None> top_tree;
    std::vector<VertexLabel*> vertex_labels;

    int size();
    std::shared_ptr<EdgeData> swap(std::shared_ptr<EdgeData>);
    std::shared_ptr<EdgeData> find_replacement(int,int,int);
    std::shared_ptr<EdgeData> recover_phase(int, int, int, int);
    std::shared_ptr<EdgeData> find_first_label(int, int, int);
    void recover(int, int, int);
    void add_label(int, std::shared_ptr<EdgeData>);
    void remove_labels(std::shared_ptr<EdgeData>);
    void reassign_vertices(TwoEdgeCluster*);
    int cover_level(int, int);

    public: 
    int two_size();
    int find_size(int, int, int);
    std::shared_ptr<EdgeData> insert(int, int);
    std::shared_ptr<EdgeData> insert(int, int, int); // TODO: SKAL Måske væk
    void remove(std::shared_ptr<EdgeData>); //delete is keyword.
    bool two_edge_connected(int,int);
    TreeEdgeData* find_bridge(int);
    TreeEdgeData* find_bridge(int, int);
    void cover(int, int, int); // TODO: move to private and remove test
    void uncover(int, int, int); // TODO: move to private and remove test
    
    TwoEdgeCluster* expose(int u) {
        return this->top_tree.expose(u);
    };
    TwoEdgeCluster* expose(int u, int v) {
        return this->top_tree.expose(u, v);
    };
    TwoEdgeCluster* deexpose(int u) {
        return this->top_tree.deexpose(u);
    };
    TwoEdgeCluster* deexpose(int u, int v) {
        return this->top_tree.deexpose(u, v);
    };

    TwoEdgeConnectivity();
    TwoEdgeConnectivity(int size) {
        TwoEdgeCluster::set_l_max((int) floor(log2(size)));
        this->top_tree = TopTree<TwoEdgeCluster,TreeEdgeData,None>(size);
        this->vertex_labels = std::vector<VertexLabel*>(size);
        for (int i = 0; i < size; i++) {
            vertex_labels[i] = new VertexLabel(TwoEdgeCluster::get_l_max()); // TODO, make sure lmax is accessed similarly everywhere.
        }
    };
    ~TwoEdgeConnectivity() {
        // TODO: reinsert
        // delete top_tree;
        for (int j = 0; j < this->vertex_labels.size(); j++) {
            VertexLabel* vertex_label = this->vertex_labels[j];


            delete vertex_label;
        }
    };
};

