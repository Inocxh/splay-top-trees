#include <catch2/catch_test_macros.hpp>
#include "top_tree.h"
#include <climits>

struct MaxWeightCluster : Node<MaxWeightCluster, int, None> {
    int max_non_path_weight;
    int max_weight;

    void create(int* edge_data, None* left, None* right) {
        if (this->is_path()) {
            max_weight = *edge_data;
            max_non_path_weight = INT_MIN;
        } else {
            max_weight = INT_MIN;
            max_non_path_weight = *edge_data;
        }
    };

    void merge(MaxWeightCluster* left, MaxWeightCluster* right) {
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
    };
    int get_data() {
        return std::max(max_non_path_weight, max_weight);
    };
};

TEST_CASE("Max-edge-weight full example", "[user data]") {
    int size = 10;
    TopTree<MaxWeightCluster, int, None> top_tree = TopTree<MaxWeightCluster, int, None>(size);
    top_tree.link(0, 1, 2);
    top_tree.link(1, 2, 2);
    top_tree.link(1, 3, 3);
    top_tree.link(3, 8, 1);
    top_tree.link(8, 9, 2);
    top_tree.link(3, 4, 5);
    top_tree.link(4, 5, 1);
    top_tree.link(4, 6, 1);
    top_tree.link(6, 7, 1);
    
    MaxWeightCluster* root;

    SECTION("Max weight on path") {
        root = top_tree.expose(0, 9);
        REQUIRE(root->max_weight == 3);
        top_tree.deexpose(0, 9);

        root = top_tree.expose(2, 7);
        REQUIRE(root->max_weight == 5);
        top_tree.deexpose(2, 7);

        top_tree.cut(1, 3);
        top_tree.link(0, 5, 1);

        root = top_tree.expose(1, 9);
        REQUIRE(root->max_weight == 5);
        top_tree.deexpose(1, 9);


        root = top_tree.expose(2, 7);
        REQUIRE(root->max_weight == 2);
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

TEST_CASE("Max-edge-weight full example 2", "[user data]") {
    int size = 10;
    TopTree<MaxWeightCluster, int, None> top_tree = TopTree<MaxWeightCluster, int, None>(size);
    top_tree.link(0, 1, 2);
    top_tree.link(1, 2, 2);
    top_tree.link(1, 3, 3);
    top_tree.link(3, 8, 1);
    top_tree.link(8, 9, 2);
    top_tree.link(3, 4, 5);
    top_tree.link(4, 5, 1);
    top_tree.link(4, 6, 1);
    top_tree.link(6, 7, 1);
    
    MaxWeightCluster* root;

    SECTION("Max weight on path") {
        root = top_tree.expose(0, 9);
        REQUIRE(root->max_weight == 3);
        top_tree.deexpose(0, 9);

        root = top_tree.expose(2, 7);
        REQUIRE(root->max_weight == 5);
        top_tree.deexpose(2, 7);

        top_tree.cut(1, 3);
        top_tree.link(0, 5, 1);

        root = top_tree.expose(1, 9);
        REQUIRE(root->max_weight == 5);
        top_tree.deexpose(1, 9);


        root = top_tree.expose(2, 7);
        REQUIRE(root->max_weight == 2);
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




