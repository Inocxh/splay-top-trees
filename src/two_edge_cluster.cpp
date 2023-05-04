#include "two_edge_cluster.h"

using std::vector;


void TwoEdgeCluster::cover(int level) {
    assert(this->is_path());
    assert(!this->is_flipped());
    this->find_size_cover(level);
    this->find_first_label_cover(level);
    this->cover_level_cover(level);
}

void TwoEdgeCluster::uncover(int level) {
    assert(this->is_path());
    assert(!this->is_flipped());
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
    assert(this->is_flipped());
    std::swap(this->part_size[0], this->part_size[1]);
    std::swap(this->part_incident[0], this->part_incident[1]);
    std::swap(this->vertex[0],this->vertex[1]);
    std::swap(this->boundary_vertices_id[0],this->boundary_vertices_id[1]);
}

void TwoEdgeCluster::assign_vertex(int vertex, VertexLabel* label) {
    this->push_flip();
    assert(!this->is_flipped());
    int is_right_endpoint = this->get_endpoint_id(1) == vertex; 
    this->vertex[is_right_endpoint] = label;     
}

bool TwoEdgeCluster::everything_zero() {
    bool cover = this->cover_level == -1 &&
    this->cover_plus == -1 &&
    this->cover_minus == -1 &&
    this->global_cover == -1 &&

    this->min_path_edge == nullptr &&
    this->min_global_edge == nullptr;

    //Find Size
    bool find_size = true;
    for (int i = 0; i < this->size.size(); i++) {
        find_size &= this->size[i] == 0;
    }
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < this->part_size[i].size(); j++) {
            for (int k = 0; k < this->part_size[i][j].size(); k++) {
                find_size &= this->part_size[i][j][k] == 0;
            }
        }
    }
    bool ffl = (this->boundary_vertices_id[0] == -1) &&
    (this->boundary_vertices_id[1] == -1) &&
    (this->incident == 0);
    if (!ffl) {
        std::cerr << (this->boundary_vertices_id[0] == -1) << std::endl;
        std::cerr << (this->boundary_vertices_id[1] == -1) << std::endl;
        std::cerr << (this->incident == 0) << std::endl;
    }
    for (int i = 0; i < part_incident[0].size(); i++) {
        ffl &= part_incident[0][i] == 0;
        ffl &= part_incident[1][i] == 0;
    }
    return ffl && cover && find_size;

}

void TwoEdgeCluster::recompute_tree() {
    this->push_flip();
    assert(!(this->is_flipped()));

    int cover_level = this->cover_level; 
    long int incident = this->incident;
    std::vector<int> size = this->size;
    std::vector<std::vector<int>> part_size_0 = this->part_size[0];
    std::vector<std::vector<int>> part_size_1 = this->part_size[1];
    std::vector<long int> part_incident_0 = this->part_incident[0];
    std::vector<long int> part_incident_1 = this->part_incident[1];

    this->clean();
    if (this->get_child(0) || this->get_child(1)) {
        this->get_child(0)->recompute_tree();
        this->get_child(1)->recompute_tree();
    }
    this->merge_internal();
    for (int i = 0; i < this->part_incident[0].size(); i++) {
        assert(part_incident_0[i] == this->part_incident[0][i]);
        assert(part_incident_1[i] == this->part_incident[1][i]);
    }
    for (int i = 0; i < this->part_incident[0].size(); i++) {
        for (int k = 0; k < this->part_size[0][i].size(); k++) {
            assert(part_size_0[i][k] == this->part_size[0][i][k]);
            assert(part_size_1[i][k] == this->part_size[1][i][k]);
        }
    }
    for (int i = 0; i < this->size.size(); i++) {
        assert(this->size[i] == size[i]);
    }
    assert(incident == this->incident);
    assert(cover_level == this->cover_level);
    assert(this->cover_minus == -1);
    assert(this->cover_plus == -1);
}

void TwoEdgeCluster::create(NewEdge* edge_data, VertexLabel* left, VertexLabel* right) {
    assert(!this->is_flipped());
    assert(everything_zero());
    create_cover(edge_data, left, right);
    create_find_size(edge_data, left, right);
    create_find_first_label(edge_data, left, right);
};
void TwoEdgeCluster::merge(TwoEdgeCluster* left, TwoEdgeCluster* right) {
    assert(!this->is_flipped());
    assert(!right->is_flipped());
    assert(!left->is_flipped());
    assert(everything_zero());
    merge_cover(left, right);
    merge_find_size(left, right);
    merge_find_first_label(left ,right);
};
void TwoEdgeCluster::split(TwoEdgeCluster* left, TwoEdgeCluster* right) {
    assert(!this->is_flipped());
    assert(!right->is_flipped());
    assert(!left->is_flipped());
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
    split_find_size(left, right);
    split_find_first_label(left, right);
    split_cover(left, right);
    everything_zero();
};

void TwoEdgeCluster::destroy(NewEdge* edge_data, VertexLabel* left, VertexLabel* right) {
    assert(!this->is_flipped());
    destroy_cover(edge_data, left, right);
    destroy_find_size(edge_data,left,right);
    destroy_find_first_label(edge_data,left,right);
    everything_zero();

}
