#include "two_edge_cluster.h"
#include <cstring>



int TwoEdgeCluster::get_size(int i) {
    return this->size[i];
}

void TwoEdgeCluster::create_find_size(NewEdge* edge_data, VertexLabel* left, VertexLabel* right)  {
    assert(this->cover_minus == -1 && this->cover_plus == -1);
    assert(edge_data->edge_type == TreeEdge1);
    int lmax = TwoEdgeCluster::get_l_max();
    int lmax_idx = lmax + 1;
    
    //Set everything to zero!
    std::fill(size.begin(), size.end(), 0);

    for (int i = 0; i < lmax_idx + 1; i++) {
        std::fill(this->part_size[0][i].begin(), this->part_size[0][i].end(), 0);
        std::fill(this->part_size[1][i].begin(), this->part_size[1][i].end(), 0);
    }
    
    /*
    For a leaf node we have 3 cases:
    0 boundary vertices:
        There is no cluster path, thus size_C is 0
        There is no point set thus part_size and diag_size are also 0
    1 boundary vertex:
        There is only one vertex on the cluster path, the boundary with cover_level = lmax.
        Since part_size and diag_size are only maintainted for 0 <= i < lmax, we let these be 0. let the lmax row be 1.
    2 boundary vertices:
        The cluster path consists of the two boundary vertices:
        -> point_size = [1,1,..] => size = [2,2,...]
        Only one part_path for each boundary in i=cover_level where part_size=[1,1,...]
    */
    int cover_level = edge_data->level;
    int cover_level_idx = cover_level + 1;
    
    //TODO: Cache inefficient?
    if (this->is_path()) {
        fill(this->size.begin(), this->size.end(), 2);
        for (int i = 0; i < 2; i++) {
            //Fill the row of the coverlevel
            fill(this->part_size[i][cover_level_idx].begin(), this->part_size[i][cover_level_idx].end(), 1);
            //Fill the row of lmax
            fill(this->part_size[i][lmax_idx].begin(), this->part_size[i][lmax_idx].end(), 1);
        }
        
    } else if (this->get_num_boundary_vertices() == 1) {
        if (cover_level == 5) {
            int ost = 0;
        }

        
        fill(size.begin(), size.begin() + (cover_level + 1), 2);
        fill(size.begin() + (cover_level + 1), size.end(), 1);
        
        // There is only a lmax part
        int is_right = this->has_right_boundary();
        
        fill(this->part_size[is_right][lmax_idx].begin(), this->part_size[is_right][lmax_idx].begin() + (cover_level + 1), 2);
        fill(this->part_size[is_right][lmax_idx].begin() + (cover_level + 1), this->part_size[is_right][lmax_idx].end(), 1);        
        
    }
    // Do nothing if num bound is 0.
    
}

void TwoEdgeCluster::merge_find_size(TwoEdgeCluster* left, TwoEdgeCluster* right) {

    assert(this->cover_minus == -1 && this->cover_plus == -1);

    int lmax = TwoEdgeCluster::get_l_max();
    int lmax_idx = lmax + 1;
    //Initialize vectors?
    
    //Heterogenous children into point cluster
    if (this->get_num_boundary_vertices() == 1 && !this->has_middle_boundary()) {
        // Handle this->size. Delete data and write new.
        fill(this->size.begin(), this->size.end(), 0); // TODO NECESSARY?
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

        // Now handle partsize. Delete data and write new.
        for (int i = 0; i < lmax_idx + 1; i++) {
            fill(this->part_size[0][i].begin(), this->part_size[0][i].end(), 0);
            fill(this->part_size[1][i].begin(), this->part_size[1][i].end(), 0);
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
        if (this->has_middle_boundary()){
            if (this->has_left_boundary()) {
                compute_part_size(this->part_size[1], right->part_size[0], left->part_size[1], right->get_cover_level());
            } else {
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

void TwoEdgeCluster::split_find_size(TwoEdgeCluster* left, TwoEdgeCluster* right) {
    
    if (this->is_path()) {
        if (left->is_path()) {
            // Check for uncover.
            
            if (left->cover_level <= this->cover_minus) { // if some uncover should be propagated.
                sum_row_range(left->part_size[0][0], left->part_size[0], 1, this->cover_minus + 2);
                sum_row_range(left->part_size[1][0], left->part_size[1], 1, this->cover_minus + 2);
                delete_row_range(left->part_size[0], 1, this->cover_minus + 2);
                delete_row_range(left->part_size[1], 1, this->cover_minus + 2);
            }
            if (left->cover_level <= this->cover_plus) {
                sum_row_range(left->part_size[0][this->cover_plus + 1], left->part_size[0], 0, this->cover_plus + 1);
                sum_row_range(left->part_size[1][this->cover_plus + 1], left->part_size[1], 0, this->cover_plus + 1);
                delete_row_range(left->part_size[0], 0, this->cover_plus + 1);
                delete_row_range(left->part_size[1], 0, this->cover_plus + 1);
            }
        }
        if (right->is_path()) {
            if (right->cover_level <= this->cover_minus) {
                sum_row_range(right->part_size[0][0], right->part_size[0], 1, this->cover_minus + 2);
                sum_row_range(right->part_size[1][0], right->part_size[1], 1, this->cover_minus + 2);
                delete_row_range(right->part_size[0], 1, this->cover_minus + 2);
                delete_row_range(right->part_size[1], 1, this->cover_minus + 2);
            }
            if (right->cover_level <= this->cover_plus) {
                sum_row_range(right->part_size[0][this->cover_plus + 1], right->part_size[0], 0, this->cover_plus + 1);
                sum_row_range(right->part_size[1][this->cover_plus + 1], right->part_size[1], 0, this->cover_plus + 1);
                delete_row_range(right->part_size[0], 0, this->cover_plus + 1);
                delete_row_range(right->part_size[1], 0, this->cover_plus + 1);
            }
        }

    }
}
