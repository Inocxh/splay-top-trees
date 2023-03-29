#include "find_size.h"
#include <cstring>

// TODO: IMPLEMENt FLIP

int get_index(int i, int j) {//TODO: LÆG ET RIGTIGT STED
    return i * CoverLevelCluster::get_l_max() + j;
} 

FindSizeCluster::FindSizeCluster() {
    int lmax = CoverLevelCluster::get_l_max();
    this->size = new int[lmax]();
    this->part_size[0] = new int[lmax * lmax]();
    this->diag_size[0] = new int[lmax * lmax]();
    this->part_size[1] = new int[lmax * lmax]();    
    this->diag_size[1] = new int[lmax * lmax]();
}

FindSizeCluster::~FindSizeCluster() {
    delete [] size;
    delete [] part_size[0];
    delete [] part_size[1];
    delete [] diag_size[0];
    delete [] diag_size[1];
};

void FindSizeCluster::create(EdgeData* edge_data, None* left, None* right)  {

    CoverLevelCluster::create(edge_data,left,right);

    int lmax = CoverLevelCluster::get_l_max();
    
    //Set everything to zero!
    memset(size, 0, sizeof(int) * lmax);
    int* arrays[] = {this->diag_size[0],this->diag_size[1],this->part_size[0],this->part_size[1]};
    for (int i = 0; i < 4; i++) {
        memset(arrays[i], 0, sizeof(int) * lmax * lmax);
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
    int cover_level = this->get_cover_level();

    //TODO: Cache inefficient?
    if (this->is_path()) {
        for (int i = 0; i < lmax; i++) {
            this->size[i] = 2;
            this->part_size[0][get_index(cover_level, i)] = 1;
            this->part_size[1][get_index(cover_level, i)] = 1;
            if (i <= cover_level) {
                this->diag_size[0][get_index(cover_level, i)] = 1;
                this->diag_size[1][get_index(cover_level, i)] = 1;
            }
        }
    } else if (this->get_num_boundary_vertices() == 1) {
        for (int i = 0; i < lmax; i++) {
            this->size[i] = 1 + (cover_level >= i);
        }

    }
    // Do nothing if num bound is 0.
    
}
void FindSizeCluster::merge(FindSizeCluster* left, FindSizeCluster* right) {
    CoverLevelCluster::merge(left, right);
    int lmax = CoverLevelCluster::get_l_max();
    //Initialize vectors?

    //Heterogenous children into point cluster
    if (this->get_num_boundary_vertices() == 1 && !this->has_middle_boundary()) {
        memset(this->size, 0, sizeof(int) * lmax * lmax);
        memset(this->part_size[!this->has_left_boundary()], 0, sizeof(int) * lmax * lmax);
        memset(this->part_size[!this->has_left_boundary()], 0, sizeof(int) * lmax * lmax);
        if (this->has_left_boundary()) { 
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
            sum_rows_from(this->size, right->diag_size[0], 0);   
        }
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



void FindSizeCluster::compute_diag_size(int* target_diag_size, int* owner_diag_size, int* other_diag_size) {
    int lmax = CoverLevelCluster::get_l_max();
    int cover_level = this->get_cover_level();
    for (int i = 0; i < lmax; i++) {
        if (i < cover_level) {
            copy_row(target_diag_size, owner_diag_size, i);
        } else if (i == cover_level) {
            copy_row(target_diag_size, owner_diag_size, i);
            sum_diag_size(target_diag_size, other_diag_size, i);
        } else if (i > cover_level) {
            copy_row(target_diag_size, other_diag_size, i);
        }
    }
}


void FindSizeCluster::compute_part_size(int* target_part_size, int* owner_part_size, int* other_part_size) {
    int lmax = CoverLevelCluster::get_l_max();
    int cover_level = this->get_cover_level();
    for (int i = 0; i < lmax; i++) {
        if (i < cover_level) {
            copy_row(target_part_size, owner_part_size, i);
        } else if (i == cover_level) {
            copy_row(target_part_size, owner_part_size, i);
            sum_rows_from(&target_part_size[get_index(i, 0)], other_part_size, i);
        } else if (i > cover_level) {
            copy_row(target_part_size, other_part_size, i);
        }
    }
}


void FindSizeCluster::copy_row(int *target_part_size, int* part_size, int row_index) {
    int lmax = CoverLevelCluster::get_l_max();
    for (int j = 0; j < lmax; j++) {
        target_part_size[get_index(row_index, j)] = part_size[get_index(row_index, j)];
    }   

}

void FindSizeCluster::sum_rows_from(int *target_row, int* part_size, int row_index) { 
    int lmax = CoverLevelCluster::get_l_max();
    for (int i = row_index; i < lmax; i++) {
        for (int j = 0; j < lmax; j++) {
            target_row[j] += part_size[get_index(i, j)];
        }   
    }
}
void FindSizeCluster::sum_diag_size(int *target_part_size, int* part_size, int row_index) { 
    int lmax = CoverLevelCluster::get_l_max();
    for (int i = row_index; i < lmax; i++) {
        for (int j = 0; j < lmax; j++) {
            target_part_size[get_index(row_index, j)] += part_size[get_index(i, j)];
        }   
    }
    for (int j = row_index + 1; j < lmax; j++) {
        target_part_size[get_index(row_index, j)] = 0;
    }
}
