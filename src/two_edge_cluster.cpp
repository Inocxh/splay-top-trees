#include "two_edge_cluster.h"

TwoEdgeCluster::TwoEdgeCluster() {
    int lmax = TwoEdgeCluster::get_l_max();

    
    size = std::vector<int>(lmax);
    this->part_size[0] = std::vector<vector<int>>(lmax + 2);
    this->part_size[1] = std::vector<vector<int>>(lmax + 2);
    for (int i = 0; i < lmax + 2; i++) {
        this->part_size[0][i] = std::vector<int>(lmax, 0);
        this->part_size[1][i] = std::vector<int>(lmax, 0);
    }

    this->incident = 0;
    this->part_incident[0] = std::vector<long int>(lmax + 2);
    this->part_incident[1] = std::vector<long int>(lmax + 2);
}


void TwoEdgeCluster::swap_data() {
    std::swap(this->part_size[0], this->part_size[1]);
    std::swap(this->part_incident[0], this->part_incident[1]);
    std::swap(this->vertex[0],this->vertex[1]);
    std::swap(this->boundary_vertices_id[0],this->boundary_vertices_id[1]);
}

void TwoEdgeCluster::assign_vertex(int vertex, VertexLabel* label) {
    int is_right_endpoint = this->get_endpoint_id(1) == vertex; 
    this->vertex[is_right_endpoint] = label;     
}

void TwoEdgeCluster::create(TreeEdge* edge_data, VertexLabel* left, VertexLabel* right) {
    create_cover(edge_data, left, right);
    create_find_size(edge_data, left, right);
    create_find_first_label(edge_data, left, right);
};
void TwoEdgeCluster::merge(TwoEdgeCluster* left, TwoEdgeCluster* right) {
    merge_cover(left, right);
    merge_find_size(left, right);
    merge_find_first_label(left ,right);
};
void TwoEdgeCluster::split(TwoEdgeCluster* left, TwoEdgeCluster* right) {
    split_find_size(left, right);
    split_cover(left, right);
    split_find_first_label(left, right);
    
};

void TwoEdgeCluster::destroy(TreeEdge* edge_data, VertexLabel* left, VertexLabel* right) {
    destroy_cover(edge_data, left, right);
}
