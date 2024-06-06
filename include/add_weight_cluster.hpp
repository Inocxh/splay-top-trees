#include "top_tree.h"

#include <climits>

struct AddWeightCluster : Node<AddWeightCluster, int, None> {
    int max_weight;
    int extra;

    void create(int* edge_data, None* left, None* right) {
        this->extra = 0;
        if (this->is_path()) {
            max_weight = *edge_data;
        } else {
            max_weight = INT_MIN;
        }
    };

    void destroy(int* edge_data, None* left, None* right) {
        *edge_data += this->extra;
    };

    void merge(AddWeightCluster* left, AddWeightCluster* right) {
        this->max_weight = std::max(left->max_weight, right->max_weight);
        
        this->max_weight = std::max(
            left->is_path() ? left->max_weight : INT_MIN, 
            right->is_path() ? right->max_weight : INT_MIN
        );
        this->extra = 0;
    };

    void split(AddWeightCluster* left, AddWeightCluster* right) {
        if (left->is_path()) {
            left->max_weight += this->extra;
            left->extra += this->extra;
        }
        if (right->is_path()) {
            right->max_weight += this->extra;
            right->extra += this->extra;
        }
    };

    void add_weight(int weight) {
        max_weight += weight;
        extra += weight;
    }
    void print_data() {
        std::cout << "node ";
    }
};

typedef TopTree<AddWeightCluster,int,None> MaxPathTopTree;