#include "cover_level.h"

void CoverLevelCluster::create(EdgeData* edge, None* left, None* right) {
    this->cover = -1;       
    this->global_cover = -1;

    if (!this->is_path()) {
        this->min_path_edge = nullptr;
        this->min_global_edge = edge;
    } else {
        this->min_path_edge = edge;
        this->min_global_edge = nullptr;
    }
    this->cover_minus = -1;
    this->cover_plus = -1;
};

void CoverLevelCluster::merge(CoverLevelCluster* left, CoverLevelCluster* right) {
    this->cover = get_l_max();
    this->min_path_edge = nullptr;
    this->global_cover = get_l_max();
    this->min_global_edge = nullptr;
    
    CoverLevelCluster* children[2] = {left, right};
    for (int i = 0; i < 2; i++) {
        //bool is_subset_of_path = (i == 0 ? this->has_right_boundary() : this->has_left_boundary());
        CoverLevelCluster* child = children[i];
        //TODO: Prove correctness of this.
        if (this->has_middle_boundary()) {
                if (child->cover < this->cover) {
                    this->cover = child->cover;
                    this->min_path_edge = child->min_path_edge;
                }   
            } else {
                if (child->cover < this->global_cover) {
                    this->global_cover = child->cover;
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


};

void CoverLevelCluster::split(CoverLevelCluster* left, CoverLevelCluster* right) {
    if (left->is_path()) {
        if (std::max(left->cover,left->cover_minus) <= this->cover_minus) {
            left->cover_minus = this->cover_minus;
        }
        if (left->cover <= std::max(left->cover_plus,left->cover_plus)) {
            left->cover = this->cover_plus;
            left->cover_plus = this->cover_plus;
        }
    } else if (right->is_path()) {
        if (std::max(right->cover,right->cover_minus) <= this->cover_minus) {
            right->cover_minus = this->cover_minus;
        }
        if (right->cover <= std::max(right->cover_plus,right->cover_plus)) {
            right->cover = this->cover_plus;
            right->cover_plus = this->cover_plus;
        }
    }
}

int CoverLevelCluster::get_cover_level() {
    assert(cover_minus == -1 && cover_plus == -1);
    return this->cover;
};

void CoverLevelCluster::set_l_max(int l) {
    CoverLevelCluster::l_max = l;
};

int CoverLevelCluster::get_l_max() {
    return CoverLevelCluster::l_max;
};













