#include "two_edge_cluster.h"

using std::vector;


void TwoEdgeCluster::cover(int level) {
    this->find_size_cover(level);
    this->find_first_label_cover(level);
    this->cover_level_cover(level);
}

void TwoEdgeCluster::uncover(int level) {
    this->find_size_uncover(level);
    this->find_first_label_uncover(level);
    this->cover_level_uncover(level);
}

TwoEdgeCluster::TwoEdgeCluster() {
    int lmax = TwoEdgeCluster::get_l_max();

    
    this->size = std::vector<int>(lmax);
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
    this->push_flip();
    int is_right_endpoint = this->get_endpoint_id(1) == vertex; 
    this->vertex[is_right_endpoint] = label;     
}

void TwoEdgeCluster::create(EdgeData* edge_data, VertexLabel* left, VertexLabel* right) {
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
    if (this->is_path()) {
        if (left->is_path()) {
            left->find_size_uncover(this->cover_minus);
            left->find_first_label_uncover(this->cover_minus);
            left->cover_level_uncover(this->cover_minus);
            
            left->find_size_cover(this->cover_plus);
            left->find_first_label_cover(this->cover_plus);
            left->cover_level_cover(this->cover_plus);

        }
        if (right->is_path()) {
            right->find_size_uncover(this->cover_minus);
            right->find_first_label_uncover(this->cover_minus);
            right->cover_level_uncover(this->cover_minus);
            
            right->find_size_cover(this->cover_plus);
            right->find_first_label_cover(this->cover_plus);
            right->cover_level_cover(this->cover_plus);
        }
    }
    split_find_size(left, right); // TODO: remove these split calls
    split_find_first_label(left, right);
    split_cover(left, right);
};

void TwoEdgeCluster::destroy(EdgeData* edge_data, VertexLabel* left, VertexLabel* right) {
    destroy_cover(edge_data, left, right);
    destroy_find_size(edge_data,left,right);
    destroy_find_first_label(edge_data,left,right);
}
