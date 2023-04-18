#include "two_edge_cluster.h"

TwoEdgeCluster::TwoEdgeCluster() {
    int lmax = TwoEdgeCluster::get_l_max();

    size = std::vector<int>(lmax);
    this->part_size[0] = std::vector<vector<int>>(lmax + 2);
    this->part_size[1] = std::vector<vector<int>>(lmax + 2);
    for (int i = 0; i < lmax + 2; i++) {
        this->part_size[0][i] = std::vector<int>(lmax);
        this->part_size[1][i] = std::vector<int>(lmax);
    }
}

TwoEdgeCluster::~TwoEdgeCluster() {
    /*delete [] size;
    delete [] part_size[0];
    delete [] part_size[1];
    delete [] diag_size[0];
    delete [] diag_size[1];*/
}

void TwoEdgeCluster::create(EdgeData* edge_data, None* left, None* right) {
    create_cover(edge_data, left, right);
    create_find_size(edge_data, left, right);
};
void TwoEdgeCluster::merge(TwoEdgeCluster* l, TwoEdgeCluster* r) {
    merge_cover(l, r);
    merge_find_size(l, r);
};
void TwoEdgeCluster::split(TwoEdgeCluster* l, TwoEdgeCluster* r) {
    split_find_size(l, r);
    split_cover(l, r);
    
};

void TwoEdgeCluster::split_leaf(EdgeData* edge_data, None* left, None* right) {
    split_leaf_cover(edge_data,left,right);
}
