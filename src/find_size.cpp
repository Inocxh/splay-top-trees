#include "two_edge_cluster.h"
#include <cstring>

void TwoEdgeCluster::swap_data() {
    std::swap(this->part_size[0], this->part_size[1]);
    std::swap(this->diag_size[0], this->diag_size[1]);
}
int TwoEdgeCluster::get_size(int i) {
    return this->size[i];
}

void TwoEdgeCluster::create_find_size(EdgeData* edge_data, None* left, None* right)  {
    int lmax = TwoEdgeCluster::get_l_max();
    
    //Set everything to zero!
    std::fill(size.begin(), size.end(), 0);
    for (int i = 0; i < lmax + 1; i++) {
        std::fill(this->part_size[0][i].begin(), this->part_size[0][i].end(), 0);
        std::fill(this->part_size[1][i].begin(), this->part_size[1][i].end(), 0);
        std::fill(this->diag_size[0][i].begin(), this->diag_size[0][i].end(), 0);
        std::fill(this->diag_size[1][i].begin(), this->diag_size[1][i].end(), 0);
    }
    
    /*
    For a leaf node we have 3 cases:
    0 boundary vertices:
        There is no cluster path, thus size_C is 0
        There is no point set thus part_size and diag_size are also 0
    1 boundary vertex:
        There is only one vertex on the cluster path, the boundary with cover_level = lmax.
        Since part_size and diag_size are only maintainted for 0 <= i < lmax, we let these be 0.
    2 boundary vertices:
        The cluster path consists of the two boundary vertices:
        -> point_size = [1,1,..] => size = [2,2,...]
        Only one part_path for each boundary in i=cover_level where part_size=[1,1,...]
    */
    int cover_level = edge_data->cover_level;


    //TODO: Cache inefficient?
    if (this->is_path()) {
        fill(this->size.begin(), this->size.end(), 2);
        for (int i = 0; i < 2; i++) {
            //Fill the row of the coverlevel
            if (cover_level >= 0) {
                fill(this->part_size[i][cover_level].begin(), this->part_size[i][cover_level].end(), 1);
                fill(this->diag_size[i][cover_level].begin(), this->diag_size[i][cover_level].begin() + cover_level + 1, 1);
            }
            //Fill the row of lmax
            fill(this->part_size[i][lmax].begin(), this->part_size[i][lmax].end(), 1);
            fill(this->diag_size[i][lmax].begin(), this->diag_size[i][lmax].end(), 1);
        }
        
    } else if (this->get_num_boundary_vertices() == 1) {
        fill(size.begin(), size.begin() + cover_level + 1, 2);
        fill(size.begin() + cover_level + 1, size.end(), 1);
        for (int i = 0; i < 2; i++) {
            fill(this->part_size[i][lmax].begin(), this->part_size[i][lmax].end(), 1);
            fill(this->diag_size[i][lmax].begin(), this->diag_size[i][lmax].end(), 1);
        }
    }
    // Do nothing if num bound is 0.
    
}

void TwoEdgeCluster::merge_find_size(TwoEdgeCluster* left, TwoEdgeCluster* right) {
    int lmax = TwoEdgeCluster::get_l_max();
    //Initialize vectors?

    //Heterogenous children into point cluster
    if (this->get_num_boundary_vertices() == 1 && !this->has_middle_boundary()) {
        //We need to write to size, part and diag, so we first delete old data.
        fill(this->size.begin(), this->size.end(), 0);
        vector<vector<int>> part_vec = this->part_size[!this->has_left_boundary()];
        vector<vector<int>> diag_vec = this->diag_size[!this->has_left_boundary()];
        for (int i = 0; i < lmax + 1; i++) {
            fill(part_vec[i].begin(), part_vec[i].end(), 0);
            fill(diag_vec[i].begin(), diag_vec[i].end(), 0);
        }
        
        if (this->has_left_boundary()) { 
            //left->cover_level() correct as left -- mid is the cluster path of left
            //Matrix multiply
            for (int j = 0; j <= left->get_cover_level(); j++) {
                this->size[j] = right->size[j];
            }
            //Sum the diagsize
            sum_rows_from(this->size, left->diag_size[0], 0); 
        } else if (this->has_right_boundary()) {
            for (int j = 0; j <= right->get_cover_level(); j++) {
                this->size[j] = left->size[j];
            }
            sum_rows_from(this->size, right->diag_size[1], 0);   
        }
        //TODO fix the part and diagsize shits

    //The general case
    } else {
        // Compute size
        for (int i = 0; i < lmax; i++) {
            // As central vertex in both, subtract 1 from every point size.
            this->size[i] = left->size[i] + right->size[i] - 1;
        }

        if (this->has_left_boundary()) {
            compute_part_size(this->part_size[0], left->part_size[0], right->part_size[0]);
            compute_diag_size(this->diag_size[0], left->diag_size[0], right->diag_size[0]);
        }
        if (this->has_middle_boundary()){
            if (this->has_left_boundary()) {
                compute_part_size(this->part_size[1], right->part_size[0], left->part_size[1]);
                compute_diag_size(this->diag_size[1], right->diag_size[0], left->diag_size[1]);
            } else {
                compute_part_size(this->part_size[0], left->part_size[1], right->part_size[0]);
                compute_diag_size(this->diag_size[0], left->diag_size[1], right->diag_size[0]);
            }
        }
        if (this->has_right_boundary()) {
            compute_part_size(this->part_size[1], right->part_size[1], left->part_size[1]);
            compute_diag_size(this->diag_size[1], right->diag_size[1], left->diag_size[1]);
        }
    }
}

void TwoEdgeCluster::compute_diag_size(vector<vector<int>>& target_diag_size, vector<vector<int>>& owner_diag_size, vector<vector<int>>& other_diag_size) {
    int lmax = TwoEdgeCluster::get_l_max();
    int cover_level = this->get_cover_level();
    for (int i = 0; i < lmax + 1; i++) {
        if (i < cover_level) {
            copy(owner_diag_size[i].begin(), owner_diag_size[i].end(), target_diag_size[i].begin());
        } else if (i == cover_level) {
            copy(owner_diag_size[i].begin(), owner_diag_size[i].end(), target_diag_size[i].begin());
            sum_diag_size(target_diag_size[i], other_diag_size, i);
        } else if (i > cover_level) {
            copy(other_diag_size[i].begin(), other_diag_size[i].end(), target_diag_size[i].begin());
        }
    }
}


void TwoEdgeCluster::compute_part_size(vector<vector<int>>& target_part_size, vector<vector<int>>& owner_part_size, vector<vector<int>>& other_part_size) {
    int lmax = TwoEdgeCluster::get_l_max();
    int cover_level = this->get_cover_level();
    for (int i = 0; i < lmax + 1; i++) {
        if (i < cover_level) {
            copy(owner_part_size[i].begin(), owner_part_size[i].end(), target_part_size[i].begin());
        } else if (i == cover_level) {
            copy(owner_part_size[i].begin(), owner_part_size[i].end(), target_part_size[i].begin());
            sum_rows_from(target_part_size[i], other_part_size, i);
        } else if (i > cover_level) {
            copy(other_part_size[i].begin(), other_part_size[i].end(), target_part_size[i].begin());
        }
    }
}

void TwoEdgeCluster::sum_rows_from(vector<int>& target_row, vector<vector<int>>& part_size, int row_index) { 
    cout << "Before size in sum: ";
    for (int i = 0; i < l_max; i++) {
        cout << target_row[i] << ", ";
    }
    cout << endl;
    cout << "Before diag: ";
    for (int i = 0; i < l_max + 1; i++) {
        for (int j = 0; j < l_max; j++) {
            cout << part_size[i][j] << " ";
        }
        cout << " , ";
    }
    for (int i = row_index; i < l_max + 1; i++) {
        for (int j = 0; j < l_max; j++) {
            target_row[j] += part_size[i][j];
        }   
    }
    cout << "After size in sum: ";
    for (int i = 0; i < l_max; i++) {
        cout << target_row[i] << ", ";
    }
    cout << endl;
}
void TwoEdgeCluster::sum_diag_size(vector<int>& target_diag_size, vector<vector<int>>& diag_size, int row_index) { 
    int lmax = TwoEdgeCluster::get_l_max();
    for (int i = row_index; i < lmax + 1; i++) {
        for (int j = 0; j < lmax; j++) {
            target_diag_size[j] += diag_size[i][j];
        }   
    }
    for (int j = row_index + 1; j < lmax; j++) {
        target_diag_size[j] = 0;
    }
}
