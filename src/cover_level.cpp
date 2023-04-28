#include "two_edge_cluster.h"

void TwoEdgeCluster::cover(int i) {
    this->cover_level = std::max(this->cover_level,i);
    this->cover_plus = std::max(this->cover_plus,i);
}
void TwoEdgeCluster::uncover(int i) {
    if (this->cover_level <= i) {
        this->cover_level = -1;
        this->cover_plus = -1;
        this->cover_minus = std::max(this->cover_minus, i);
    }
}

void TwoEdgeCluster::create_cover(NewEdge* edge, VertexLabel* left, VertexLabel* right) {
    assert(edge->edge_type == TreeEdge1);
    if (this->is_path()) {
        this->cover_level = edge->level;
        this->global_cover = l_max;
        
        this->min_path_edge = edge;
        this->min_global_edge = nullptr;
    } else {
        this->cover_level = l_max;
        this->global_cover = edge->level;
        
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
    if (this->is_path() || this->has_middle_boundary()) {
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
        NewEdge* min_global_edge_left;
        int global_cover_right;
        NewEdge* min_global_edge_right;

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

};

void TwoEdgeCluster::split_cover(TwoEdgeCluster* left, TwoEdgeCluster* right) {
    if (this->is_path()) {
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
                right->uncover_val = this->cover_minus;
            }
            if (right->cover_level <= std::max(this->cover_minus, this->cover_plus)) {
                right->cover_level = this->cover_plus;
                right->cover_plus = this->cover_plus;
                if (this->cover_minus > this->cover_plus) {
                    right->uncover_val = this->cover_minus;
                } else {
                    right->cover_val = this->cover_plus;
                }
            }
        }
    }
    //this->cover_minus = -1;
    //this->cover_plus = -1;
};

void TwoEdgeCluster::destroy_cover(NewEdge* edge_data, VertexLabel* left, VertexLabel* right) {
    assert(edge_data->edge_type == TreeEdge1);
    if (this->is_path()) {
        edge_data->level = this->cover_level;
    }
    //this->cover_minus = -1;
    //this->cover_plus = -1;
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













