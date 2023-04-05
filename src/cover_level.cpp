#include "two_edge_cluster.h"

void TwoEdgeCluster::cover(int i) {
    this->cover_level = std::max(this->cover_level,i);
    this->cover_plus = std::max(this->cover_plus,i);
}
void TwoEdgeCluster::uncover(int i) {
    
}

void TwoEdgeCluster::create_cover(EdgeData* edge, None* left, None* right) {
    if (this->is_path()) {
        this->cover_level = edge->cover_level;
        this->global_cover = l_max;
        
        this->min_path_edge = edge;
        this->min_global_edge = nullptr;
    } else {
        this->cover_level = l_max;
        this->global_cover = edge->cover_level;
        
        this->min_path_edge = nullptr;
        this->min_global_edge = edge;    
    }
    this->cover_plus = -1;
    this->cover_minus = -1;
    
    /*this->cover_level = edge->cover_level != -1 ? edge->cover_level : -1;       
    this->global_cover = -1;

    if (!this->is_path()) {
        this->min_path_edge = nullptr;
        this->min_global_edge = edge;
    } else {
        this->min_path_edge = edge;
        this->min_global_edge = nullptr;
    }
    this->cover_minus = -1;
    this->cover_plus = -1;*/
};

void TwoEdgeCluster::merge_cover(TwoEdgeCluster* left, TwoEdgeCluster* right) {
    if (this->is_path()) {
        if (left->cover_level <= right->cover_level) {
            this->cover_level = left->cover_level;
            this->min_path_edge = left->min_path_edge;
        } else {
            this->cover_level = right->cover_level;
            this->min_path_edge = right->min_path_edge;
        }


        if (left->global_cover <= right->global_cover) {
            this->global_cover = left->global_cover;
            this->min_global_edge = left->min_global_edge;
        } else {
            this->global_cover = right->global_cover;
            this->min_global_edge = right->min_global_edge;
        }

    } else {
        this->cover_level = l_max;
        this->min_path_edge = nullptr;

        int global_cover_left;
        EdgeData* min_global_edge_left;
        int global_cover_right;
        EdgeData* min_global_edge_right;

        if (left->global_cover <= left->cover_level) {
            global_cover_left = left->global_cover;
            min_global_edge_left = left->min_global_edge;
        } else {
            global_cover_left = left->cover_level;
            min_global_edge_left = left->min_path_edge;
        }

        if (right->global_cover <= right->cover_level) {
            global_cover_right = right->global_cover;
            min_global_edge_right = right->min_global_edge;
        } else {
            global_cover_right = right->cover_level;
            min_global_edge_right = right->min_path_edge;
        }
        
        if (global_cover_left <= global_cover_right) {
            this->global_cover = global_cover_left;
            this->min_global_edge = left->min_global_edge;
        } else {
            this->global_cover = global_cover_right;
            this->min_global_edge = right->min_global_edge;
        }

    }
    this->cover_plus = -1;
    this->cover_minus = -1;
    









/*
    this->cover_level = get_l_max();
    this->min_path_edge = nullptr;
    this->global_cover = get_l_max();
    this->min_global_edge = nullptr;
    
    TwoEdgeCluster* children[2] = {left, right};
    for (int i = 0; i < 2; i++) {
        //bool is_subset_of_path = (i == 0 ? this->has_right_boundary() : this->has_left_boundary());
        TwoEdgeCluster* child = children[i];
        //TODO: Prove correctness of this.
        if (this->has_middle_boundary() || (this->has_left_boundary() && this->has_right_boundary())) {
            if (child->cover_level < this->cover_level) {
                this->cover_level = child->cover_level;
                this->min_path_edge = child->min_path_edge;
            }   
        } else {
            if (child->cover_level < this->global_cover) {
                this->global_cover = child->cover_level;
                this->min_global_edge = child->min_path_edge;
            }
        }
        if (child->global_cover < this->global_cover) {
            this->global_cover = child->global_cover;
            this->min_global_edge = child->min_global_edge;
        }
    }
    this->cover_minus = -1;
    this->cover_plus = -1;
*/
};

void TwoEdgeCluster::split_cover(TwoEdgeCluster* left, TwoEdgeCluster* right) {
    if (left->is_path()) {
        if (std::max(left->cover_level, left->cover_minus) <= this->cover_minus) {
            left->cover_minus = this->cover_minus;
        }
        if (left->cover_level <= std::max(this->cover_minus, this->cover_plus)) {
            left->cover_level = this->cover_plus;
            left->cover_plus = this->cover_plus;
        }
    }
    if (right->is_path()) {
        if (std::max(right->cover_level, right->cover_minus) <= this->cover_minus) {
            right->cover_minus = this->cover_minus;
        }
        if (right->cover_level <= std::max(this->cover_minus, this->cover_plus)) {
            right->cover_level = this->cover_plus;
            right->cover_plus = this->cover_plus;
        }
    }
    this->cover_minus = -1;
    this->cover_plus = -1;
};

void TwoEdgeCluster::split_leaf_cover(EdgeData* edge_data, None* left, None* right) {
    if (this->is_path()) {
        edge_data->cover_level = this->cover_level;
    }
    this->cover_minus = -1;
    this->cover_plus = -1;
}

int TwoEdgeCluster::get_cover_level() {
    assert(this->cover_level >= cover_plus && (this->cover_level <= this->cover_minus ? this->cover_level == this->cover_plus : true));
    return this->cover_level;
};

void TwoEdgeCluster::set_l_max(int l) {
    TwoEdgeCluster::l_max = l;
};

int TwoEdgeCluster::get_l_max() {
    return TwoEdgeCluster::l_max;
};













