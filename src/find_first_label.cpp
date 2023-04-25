#include "two_edge_cluster.h"
#include <cassert>

void set_bit(long int* bitvec, int pos) {
    *bitvec |= (1 << pos);
}

void clear_bit(long int* bitvec, int pos) {
    *bitvec &= ~(1 << pos);
}

void clear_from(long int* bitvec, int pos) {
    unsigned long int mask = ~0;
    *bitvec &= ~(mask << pos);
}

bool get_bit(long int bitvec, int pos) {
    return bitvec & (1 << pos);
}

void or_diagonal(long int *target_row, vector<long int>& source) { 
    int lmax_idx = TwoEdgeCluster::get_l_max() + 1;

    // Start from 1, i.e. row 0, as row -1 is 0s. 
    for (int i = 1; i < lmax_idx + 1; i++) {
        long int tmp = source[i];
        clear_from(&tmp, i);
        *target_row |= tmp;
    }
}

void compute_part_incident(vector<long int>& target_part_incident, vector<long int>& owner_part_incident, vector<long int>& other_part_incident, int cover_level) {
    int lmax_idx = TwoEdgeCluster::get_l_max() + 1;
    int cover_level_idx = cover_level + 1; 

    for (int i = 0; i < lmax_idx + 1; i++) {
        if (i > cover_level_idx) {
            target_part_incident[i] = owner_part_incident[i];
        } else if (i == cover_level_idx) {
            target_part_incident[i] = owner_part_incident[i];
            for (int j = i; i < lmax_idx + 1; i++) {
                target_part_incident[i] |= other_part_incident[j];
            }       
        } else if (i < cover_level_idx) {
            target_part_incident[i] = other_part_incident[i];
        }
    }
}

void or_row_range(long int *target_row, vector<long int>& source, int start, int end) { 
    for (int i = start; i < end; i++) {
        *target_row |= source[i];
    }
}

void TwoEdgeCluster::create_find_first_label(TreeEdge* edge, VertexLabel*, VertexLabel*) {
    this->boundary_vertices_id[0] = -1;
    this->boundary_vertices_id[1] = -1;
    if (this->has_left_boundary()) {
        this->boundary_vertices_id[0] = this->get_endpoint_id(0);
    }
    if (this->has_right_boundary()) {
        this->boundary_vertices_id[1] = this->get_endpoint_id(1);
    }

    int cover_level_idx = edge->cover_level + 1;
    int lmax_idx = this->l_max + 1;
    
    this->incident = 0;
    fill(this->part_incident[0].begin(), this->part_incident[0].end(), 0);
    fill(this->part_incident[1].begin(), this->part_incident[1].end(), 0);
    
    long int label[2] = {0,0};

    for (int i = 0; i < 2; i++) {     
        if (this->vertex[i]) {
            for (int j = 0; j < this->vertex[i]->labels.size(); j++) {
                if (this->vertex[i]->labels[j].size() > 0) {
                    set_bit(&label[i], j);
                }
            }
        }
    }

    if (this->get_num_boundary_vertices() == 1) {
        if (this->has_left_boundary()) {
            clear_from(&label[1], cover_level + 1);
        } else {
            clear_from(&label[0], cover_level + 1);
        }
        this->incident = label[0] | label[1];
        this->part_incident[!this->has_left_boundary()][lmax_idx] = this->incident;
    } else if (this->get_num_boundary_vertices() == 2) {
        this->incident = label[0] | label[1];
        this->part_incident[0][lmax_idx] = label[0];
        this->part_incident[0][cover_level_idx] = label[1];
        this->part_incident[1][lmax_idx] = label[1];
        this->part_incident[1][cover_level_idx] = label[0];
    }
}

void TwoEdgeCluster::merge_find_first_label(TwoEdgeCluster* left, TwoEdgeCluster* right) {
    this->boundary_vertices_id[0] = -1;
    this->boundary_vertices_id[1] = -1;

    //First update the boundary vertex ids on the cluster
    if (this->has_left_boundary()) {
        this->boundary_vertices_id[0] = left->boundary_vertices_id[0];
    }
    if (this->has_middle_boundary()) {
        //left of right and right of left equal by orientation invariant.
        int middle_id = left->boundary_vertices_id[1];
        if (this->has_left_boundary()) {
            this->boundary_vertices_id[1] = middle_id;
        } else {
            this->boundary_vertices_id[0] = middle_id;
        }
    }
    if (this->has_right_boundary()) {
        this->boundary_vertices_id[1] = right->boundary_vertices_id[1];
    }

    //Then update the incident vector and part_incident vectors.

    int lmax_idx = this->l_max + 1;
    this->incident = 0; //TODO needed?
    
    if (this->get_num_boundary_vertices() == 1 && !this->has_middle_boundary()) { // Off the path
        if (this->has_left_boundary()) { 
            long int right_cleared = right->incident;
            clear_from(&right_cleared, left->get_cover_level() + 1);
            this->incident = right_cleared;
            or_diagonal(&this->incident, left->part_incident[0]); 
        } else if (this->has_right_boundary()) {
            long int left_cleared = left->incident;
            clear_from(&left_cleared, right->get_cover_level() + 1);
            this->incident = left_cleared;
            or_diagonal(&this->incident, right->part_incident[1]);  
        }

        // Now handle part_incident. Delete data and write new.
        for (int i = 0; i < lmax_idx + 1; i++) {
            this->part_incident[0][i] = 0;
            this->part_incident[1][i] = 0;
        }
        
        // Copy size into part_incident row: lmax. 
        this->part_incident[!this->has_left_boundary()][lmax_idx] = this->incident;

    } else { // General case
        this->incident = left->incident | right->incident;
        if (this->has_left_boundary()) {
            compute_part_incident(this->part_incident[0], left->part_incident[0], right->part_incident[0], left->get_cover_level());
        }
        if (this->has_middle_boundary()) {
            if (this->has_left_boundary()) {
                compute_part_incident(this->part_incident[1], right->part_incident[0], left->part_incident[1], right->get_cover_level());
            } else {
                compute_part_incident(this->part_incident[0], left->part_incident[1], right->part_incident[0], left->get_cover_level());
            }
        }
        if (this->has_right_boundary()) {
            compute_part_incident(this->part_incident[1], right->part_incident[1], left->part_incident[1], right->get_cover_level());
        }
    }
}



void TwoEdgeCluster::split_find_first_label(TwoEdgeCluster* left, TwoEdgeCluster* right) {
    if (this->is_path()) {
        if (left->is_path()) {
            // Check for uncover.
            
            if (left->cover_level <= this->cover_minus) { // if some uncover should be propagated.
                or_row_range(&left->part_incident[0][0], left->part_incident[0], 1, this->cover_minus + 2);
                or_row_range(&left->part_incident[1][0], left->part_incident[1], 1, this->cover_minus + 2);
                fill(left->part_incident[0].begin() + 1, left->part_incident[0].begin() + (this->cover_minus + 2), 0);
                fill(left->part_incident[1].begin() + 1, left->part_incident[1].begin() + (this->cover_minus + 2), 0);
            }
            if (left->cover_level <= this->cover_plus) {
                or_row_range(&left->part_incident[0][this->cover_plus + 1], left->part_incident[0], 0, this->cover_plus + 1);
                or_row_range(&left->part_incident[1][this->cover_plus + 1], left->part_incident[1], 0, this->cover_plus + 1);
                fill(left->part_incident[0].begin(), left->part_incident[0].begin() + (this->cover_minus + 1), 0);
                fill(left->part_incident[1].begin(), left->part_incident[1].begin() + (this->cover_minus + 1), 0);
                fill(left->part_incident[0].begin(), left->part_incident[0].begin() + (this->cover_minus + 1), 0);
                fill(left->part_incident[1].begin(), left->part_incident[1].begin() + (this->cover_minus + 1), 0);
            }
        }
        if (right->is_path()) {
            if (right->cover_level <= this->cover_minus) {
                or_row_range(&right->part_incident[0][0], right->part_incident[0], 1, this->cover_minus + 2);
                or_row_range(&right->part_incident[1][0], right->part_incident[1], 1, this->cover_minus + 2);
                fill(right->part_incident[0].begin() + 1, right->part_incident[0].begin() + (this->cover_minus + 2), 0);
                fill(right->part_incident[1].begin() + 1, right->part_incident[1].begin() + (this->cover_minus + 2), 0);
            }
            if (right->cover_level <= this->cover_plus) {
                or_row_range(&right->part_incident[0][this->cover_plus + 1], right->part_incident[0], 0, this->cover_plus + 1);
                or_row_range(&right->part_incident[1][this->cover_plus + 1], right->part_incident[1], 0, this->cover_plus + 1);
                fill(right->part_incident[0].begin(), right->part_incident[0].begin() + (this->cover_minus + 1), 0);
                fill(right->part_incident[1].begin(), right->part_incident[1].begin() + (this->cover_minus + 1), 0);
            }
        }
    }
}

long int TwoEdgeCluster::get_incident() {
    return this->incident;
}


VertexLabel* TwoEdgeCluster::find_first_label(int u, int v, int level) {
    this->clean(); //Fixes orientation and calls split from above.

    // u is now leftmost of this cluster.
    int u_is_right = this->boundary_vertices_id[1] == u;
    if (this->vertex[u_is_right] && this->vertex[u_is_right]->labels[level].size() > 0) {
        this->full_splay();
        return this->vertex[u_is_right];
    } else if (this->vertex[!u_is_right] && this->cover_level >= level) {
        this->full_splay();
        return this->vertex[!u_is_right];
    } else if (!this->get_child(0) || !this->get_child(1)) {
        this->full_splay();
        return nullptr;
    }


    TwoEdgeCluster* left_child = this->get_child(0);
    TwoEdgeCluster* right_child = this->get_child(1);
    
    TwoEdgeCluster* close_child, * far_child;
    int index = 0;

    if (left_child->boundary_vertices_id[0] == u) {
        //u left
        close_child = left_child;
        far_child = right_child;
        index = 0;
    } else if (right_child->boundary_vertices_id[1] == u) {
        //u right
        close_child = right_child;
        far_child = left_child;
        index = 1;
    } else {
        // u middle. 
        assert(left_child->is_point() || right_child->is_point());
        close_child = left_child->is_point() 
                      ? left_child
                      : right_child;
        far_child = left_child->is_point() 
                    ? right_child
                    : left_child;  
        index = left_child->is_point() 
               ? 1
               : 0;
    }
    
    if (get_bit(close_child->incident, level)) {

        return close_child->find_first_label(close_child->boundary_vertices_id[index], close_child->boundary_vertices_id[!index], level);
        
    } else if (get_bit(far_child->incident, level) && close_child->get_cover_level() >= level) { 
        if (close_child->is_point()) {
            index = !index;
        }    
        return far_child->find_first_label(far_child->boundary_vertices_id[index], far_child->boundary_vertices_id[!index], level);
    }
    //Nothing exists
    return nullptr;
}