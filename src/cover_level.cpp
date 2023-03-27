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
    this->cover = this->l_max;
    this->min_path_edge = nullptr;
    this->global_cover = this->l_max;
    this->min_global_edge = nullptr;
    
    CoverLevelCluster* children[2] = {left, right};
    for (int i = 0; i < 2; i++) {
        //bool is_subset_of_path = (i == 0 ? this->has_right_boundary() : this->has_left_boundary());
        //CoverLevelCluster* child = children[i];
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















