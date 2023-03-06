#include <catch2/catch_test_macros.hpp>
#include "top_tree.h"

struct AddWeightCluster : Node<AddWeightCluster, int, None> {
    int max_weight;
    int max_non_path_weight;
    int extra;

    void create(int* edge_data, None* left, None* right) {
        if (this->is_path()) {
            max_weight = *edge_data;
            max_non_path_weight = INT_MIN;
        } else {
            max_weight = INT_MIN;
            max_non_path_weight = *edge_data;
        }
    };

    void split_leaf(int* edge_data, None* left, None* right) {
        *edge_data += this->extra;
    };

    void merge(AddWeightCluster* left, AddWeightCluster* right) {
        max_non_path_weight = std::max(left->max_non_path_weight, right->max_non_path_weight);
        if (!this->is_path()) {
            max_non_path_weight = std::max(
                max_non_path_weight,
                left->is_path() ? left->max_weight : max_non_path_weight
            );
            max_non_path_weight = std::max(
                max_non_path_weight,
                right->is_path() ? right->max_weight : max_non_path_weight
            );
        } 
        this->max_weight = std::max(
            left->is_path() ? left->max_weight : INT_MIN, 
            right->is_path() ? right->max_weight : INT_MIN
        );
        this->extra = 0;
    };
    void split_internal(AddWeightCluster* left, AddWeightCluster* right) {
        if (left->is_path()) {
            left->max_weight += this->extra;
            left->extra += this->extra;
        }
        if (right->is_path()) {
            right->max_weight += this->extra;
            right->extra += this->extra;
        }
    };

    int get_data() {
        return std::max(max_non_path_weight, max_weight);
    }

    void add_weight(int weight) {
        max_weight += weight;
        extra += weight;
    }
};


TEST_CASE("Add weight full example", "[user data]") {
    int size = 10;
    TopTree<AddWeightCluster, int, None> top_tree = TopTree<AddWeightCluster, int, None>(size);
    top_tree.link(0, 1, 2);
    top_tree.link(1, 2, 2);
    top_tree.link(1, 3, 3);
    top_tree.link(3, 8, 1);
    top_tree.link(8, 9, 2);
    top_tree.link(3, 4, 5);
    top_tree.link(4, 5, 1);
    top_tree.link(4, 6, 1);
    top_tree.link(6, 7, 1);
    
    AddWeightCluster* root;

    SECTION("Max weight on path") {
        root = top_tree.expose(0, 9);
        REQUIRE(root->max_weight == 3);
        root->add_weight(10);
        REQUIRE(root->max_weight == 13);
        top_tree.deexpose(0, 9);

        root = top_tree.expose(9, 5);
        REQUIRE(root->max_weight == 12);

    }   

    SECTION("Max weight in component") {
        root = top_tree.expose(0);
        REQUIRE(root->get_data() == 5);
        top_tree.deexpose(0);

        top_tree.cut(1, 3);

        root = top_tree.expose(0);
        REQUIRE(root->get_data() == 2);
        top_tree.deexpose(0);

        root = top_tree.expose(7);
        REQUIRE(root->get_data() == 5);
        top_tree.deexpose(7);

        root = top_tree.expose(3);
        REQUIRE(root->get_data() == 5);
    }
}




