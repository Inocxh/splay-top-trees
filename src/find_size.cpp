#include "two_edge_cluster.h"
#include <cstring>

using std::vector;


int TwoEdgeCluster::get_size(int i) {
    return this->size[i];
}

void TwoEdgeCluster::create_find_size(TreeEdgeData* edge_data, None* left, None* right)  {
    int lmax = TwoEdgeCluster::get_l_max();
    int lmax_idx = lmax + 1;
    
    int cover_level = edge_data->level;
    int cover_level_idx = cover_level + 1;
    
    if (this->is_path()) {
        fill(this->size.begin(), this->size.end(), 2);
        for (int i = 0; i < 2; i++) {
            //Fill the row of the coverlevel
            fill(this->part_size[i][cover_level_idx].begin(), this->part_size[i][cover_level_idx].end(), 1);
            //Fill the row of lmax
            fill(this->part_size[i][lmax_idx].begin(), this->part_size[i][lmax_idx].end(), 1);
        }
        
    } else if (this->get_num_boundary_vertices() == 1) {
        fill(size.begin(), size.begin() + (cover_level + 1), 2);
        fill(size.begin() + (cover_level + 1), size.end(), 1);
        
        // There is only a lmax part        
        copy(size.begin(), size.end(), this->part_size[this->has_right_boundary()][lmax_idx].begin());       
    }
    // Do nothing if num bound is 0.
    
}

void TwoEdgeCluster::destroy_find_size(TreeEdgeData* edge_data, None* left, None* right)  {
    fill(size.begin(),size.end(),0);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < this->part_size[i].size(); j++) {
            fill(this->part_size[i][j].begin(),this->part_size[i][j].end(),0);
        }
    }
}

void TwoEdgeCluster::merge_find_size(TwoEdgeCluster* left, TwoEdgeCluster* right) {

    int lmax = TwoEdgeCluster::get_l_max();
    int lmax_idx = lmax + 1;
    
    //Heterogenous children into point cluster
    if (this->get_num_boundary_vertices() == 1 && !this->has_middle_boundary()) {
        // Handle this->size. Delete data and write new.
        if (this->has_left_boundary()) { 
            //left->cover_level() correct as left -- mid is the cluster path of left
            for (int j = 0; j <= left->get_cover_level(); j++) {
                if (j == lmax) continue;
                this->size[j] = right->size[j] - 1;
            }
            sum_diagonal(this->size, left->part_size[0]); 
        } else if (this->has_right_boundary()) {
            for (int j = 0; j <= right->get_cover_level(); j++) {
                if (j == lmax) continue;
                this->size[j] = left->size[j] - 1;
                
            }
            sum_diagonal(this->size, right->part_size[1]);   
        }

        // Copy size into partsize row: lmax. 
        std::copy(this->size.begin(), this->size.end(), this->part_size[!this->has_left_boundary()][lmax_idx].begin());
    //The general case
    } else {
        // Compute size
        for (int i = 0; i < lmax; i++) {
            // As central vertex in both, subtract 1 from every point size.
            this->size[i] = left->size[i] + right->size[i] - 1;
        }

        if (this->has_left_boundary()) {
            compute_part_size(this->part_size[0], left->part_size[0], right->part_size[0], left->get_cover_level());
        }
        if (this->has_middle_boundary()) {
            if (!this->has_right_boundary()) {
                compute_part_size(this->part_size[1], right->part_size[0], left->part_size[1], right->get_cover_level());
            }
            if (!this->has_left_boundary()) {
                compute_part_size(this->part_size[0], left->part_size[1], right->part_size[0], left->get_cover_level());
            }
        }
        if (this->has_right_boundary()) {
            compute_part_size(this->part_size[1], right->part_size[1], left->part_size[1], right->get_cover_level());
        }
    }
}
void TwoEdgeCluster::compute_part_size(vector<vector<int>>& target_part_size, vector<vector<int>>& owner_part_size, vector<vector<int>>& other_part_size, int cover_level) {
    int lmax = TwoEdgeCluster::get_l_max();
    int lmax_idx = lmax + 1;
    int cover_level_idx = cover_level + 1;


    for (int i = 0; i < lmax_idx + 1; i++) {
        if (i > cover_level_idx) {
            std::copy(owner_part_size[i].begin(), owner_part_size[i].end(), target_part_size[i].begin());
        } else if (i == cover_level_idx) {
            std::copy(owner_part_size[i].begin(), owner_part_size[i].end(), target_part_size[i].begin());
            sum_row_range(target_part_size[i], other_part_size, i, lmax_idx + 1);
            // Subtract 1, as it is otherwise counted twice.
            for (int j = 0; j < lmax; j++) {
                target_part_size[i][j] -= 1;
            }
        
        } else if (i < cover_level_idx) {
            std::copy(other_part_size[i].begin(), other_part_size[i].end(), target_part_size[i].begin());
        }
    }
}

// Sums rows source[start,end) into target_row
void TwoEdgeCluster::sum_row_range(vector<int>& target_row, vector<vector<int>>& source, int start, int end) { 
    for (int i = start; i < end; i++) {
        for (int j = 0; j < l_max; j++) {
            target_row[j] += source[i][j];
        }
    }
}

void TwoEdgeCluster::delete_row_range(vector<vector<int>>& target, int start, int end) { 
    for (int i = start; i < end; i++) {
        fill(target[i].begin(), target[i].end(), 0);
    }
}

void TwoEdgeCluster::sum_diagonal(vector<int>& target_row, vector<vector<int>>& source) { 
    int lmax = TwoEdgeCluster::get_l_max();
    int lmax_idx = lmax + 1;

    // Start from 1, i.e. row 0, as row -1 is 0s.
    for (int i = 1; i < lmax_idx + 1 ; i++) {
        for (int j = 0; j < std::min(i,lmax); j++) {

            target_row[j] += source[i][j];
        }   
    }
}

void TwoEdgeCluster::find_size_cover(int i) {
    if (i < this->cover_plus) {
        return;
    }
    if (this->cover_level < i) { 
        sum_row_range(this->part_size[0][i + 1], this->part_size[0], 0, i + 1);
        sum_row_range(this->part_size[1][i + 1], this->part_size[1], 0, i + 1);
        delete_row_range(this->part_size[0], 0, i + 1);
        delete_row_range(this->part_size[1], 0, i + 1);
    }
}

void TwoEdgeCluster::find_size_uncover(int i) {
    if (i < this->cover_plus) {
        return;
    }
    if (this->cover_level <= i) {
        // this->last_uncover = i;
        sum_row_range(this->part_size[0][0], this->part_size[0], 1, i + 2);
        sum_row_range(this->part_size[1][0], this->part_size[1], 1, i + 2);
        delete_row_range(this->part_size[0], 1, i + 2);
        delete_row_range(this->part_size[1], 1, i + 2);
    }
}


void TwoEdgeCluster::split_find_size(TwoEdgeCluster* left, TwoEdgeCluster* right) {
    for (int i = 0; i < this->size.size(); i++) {
        this->size[i] = 0;
    }
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < this->part_size[i].size(); j++) {
            for (int k = 0; k < this->part_size[i][j].size(); k++) {
                this->part_size[i][j][k] = 0;
            }
        }
    }
}
