#include "top_tree.h"

#include <climits>

struct MaxPathCluster : Node<MaxPathCluster, int, None> {
    int max_weight;
    void create(int* edge, None* left, None* right) {
        this->max_weight = this->is_path() ? *edge : INT_MIN;
    };    
    void merge(MaxPathCluster* left, MaxPathCluster* right) {
        this->max_weight = std::max(
            left->is_path() ? left->max_weight : INT_MIN, 
            right->is_path() ? right->max_weight : INT_MIN
        );
    };
};