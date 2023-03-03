#include <catch2/catch_test_macros.hpp>
#define TEST 1

#include "top_tree.h"


struct MaxWeightData : Node<MaxWeightData, int, None> {
    int data;

    public:
    void merge() {};
    void split() {};

    void merge_leaf(int* edge, None* left, None* right) {
        if (this->is_path()) {
            this->data = *edge;
        } else {
            this->data = INT_MIN;
        }
    };

    void split_leaf(int* edge, None* left, None* right) {};
    void split_internal(MaxWeightData* left, MaxWeightData* right) {};
    
    void merge_internal(MaxWeightData* left, MaxWeightData* right) {
        this->data = std::max(
            left->is_path() ? left->data : INT_MIN, 
            right->is_path() ? right->data : INT_MIN
        );
    };
};

struct MaxEdgeData : Node<MaxEdgeData, int, None> {
    int max_non_path_weight;
    int max_weight;

    void merge() {};
    void split() {};

    void split_leaf(int* edge_data, None* left, None* right) {};
    void merge_leaf(int* edge_data, None* left, None* right) {
        if (this->is_path()) {
            max_weight = *edge_data;
            max_non_path_weight = INT_MIN;
        } else {
            max_weight = INT_MIN;
            max_non_path_weight = *edge_data;
        }
    };

    void split_internal(MaxEdgeData* left, MaxEdgeData* right) {};
    void merge_internal(MaxEdgeData* left, MaxEdgeData* right) {
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
    }
};

TEST_CASE("Simple test", "[user data]")
{
    int size = 10;
    int vertex_id = 1;
    TopTree<MaxWeightData, int, None> top_tree = TopTree<MaxWeightData, int, None>(size);
    top_tree.expose(vertex_id);
    REQUIRE((top_tree.get_vertex(vertex_id)->is_exposed()));
}

TEST_CASE("Constructor test", "[user data]")
{
    int size = 10;
    TopTree<MaxWeightData, int, None> top_tree = TopTree<MaxWeightData, int, None>(size);
    MaxWeightData* e1 = top_tree.link(0, 1, 5);
    MaxWeightData* e2 = top_tree.link(1, 2, 10);

}

TEST_CASE(" test", "[user data]") {
    int size = 10;
    TopTree<MaxWeightData, int, None> top_tree = TopTree<MaxWeightData, int, None>(size);
    MaxWeightData* e1 = top_tree.link(0, 1, 0);
    MaxWeightData* e2 = top_tree.link(1, 2, 0);
}

TEST_CASE("Max-edge-weight test (small)", "[user data]") {
    int size = 10;
    TopTree<MaxWeightData, int, None> top_tree = TopTree<MaxWeightData,int,None>(size);
    top_tree.link(0, 1, 5);
    top_tree.link(1, 2, 10);
    SECTION("Test1") {
        MaxWeightData* root = top_tree.expose(0, 2);
        REQUIRE(root->data == 10);
    }
    SECTION("Test2") {
        MaxWeightData* root = top_tree.expose(0, 1);
        REQUIRE(root->data == 5);
    }
    SECTION("Test3") {
        MaxWeightData* root = top_tree.expose(1, 2);
        REQUIRE(root->data == 10);
    }
}
TEST_CASE("Max-edge-weight test (large)", "[user data]") {
    int size = 999;
    TopTree<MaxWeightData, int, None> top_tree = TopTree<MaxWeightData, int, None>(size);
    for (int i = 0; i < size - 1; i++) {
        top_tree.link(i, i + 1, i);
    }
    SECTION("Test1") {
        MaxWeightData* root = top_tree.expose(0, 4);
        REQUIRE(root->data == 3);
    }
    SECTION("Test2") {
        MaxWeightData* root = top_tree.expose(3, 4);
        REQUIRE(root->data == 3);
    }
    SECTION("Test3") {
        top_tree.expose(0);
        for (int i = 1; i < size - 1; i++) {
            MaxWeightData* root = top_tree.expose(i);
            REQUIRE(root->data == i - 1);
            top_tree.deexpose(i);
        }
    }
}
TEST_CASE("Max-edge-weight cut test", "[user data]") {
    int size = 4;
    TopTree<MaxWeightData, int, None> top_tree = TopTree<MaxWeightData, int, None>(size);
    top_tree.link(0, 1, 1);
    top_tree.link(1, 2, 10);
    top_tree.link(2, 3, 1);
    
    MaxWeightData* root = top_tree.expose(0, 3);
    REQUIRE(root->data == 10);

    top_tree.deexpose(0, 3);
    top_tree.cut(1, 2);

    top_tree.link(1, 2, 0);
    root = top_tree.expose(0, 3);

    REQUIRE(root->data == 1);
    

}
TEST_CASE("Max-edge-weight full example", "[user data]") {
    int size = 10;
    TopTree<MaxEdgeData, int, None> top_tree = TopTree<MaxEdgeData, int, None>(size);
    top_tree.link(0, 1, 2);
    top_tree.link(1, 2, 2);
    top_tree.link(1, 3, 3);
    top_tree.link(3, 8, 1);
    top_tree.link(8, 9, 2);
    top_tree.link(3, 4, 5);
    top_tree.link(4, 5, 1);
    top_tree.link(4, 6, 1);
    top_tree.link(6, 7, 1);
    
    MaxEdgeData* root;

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
        int lol = root->get_data();
        REQUIRE(root->get_data() == 2);
        top_tree.deexpose(0);

        root = top_tree.expose(7);
        REQUIRE(root->get_data() == 5);
        top_tree.deexpose(7);

        root = top_tree.expose(3);
        REQUIRE(root->get_data() == 5);
    }
}




