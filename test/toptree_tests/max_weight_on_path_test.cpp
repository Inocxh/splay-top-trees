#include <catch2/catch_test_macros.hpp>
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
    void add_weight(int weight) {
        
    }
    void print_data() {
        std::cout << "num_b: "  << this->get_num_boundary_vertices() << " ";
    }
};



TEST_CASE("Max-edge-weight shit", "[user data]") {
    int size = 26;
    TopTree<MaxPathCluster, int, None> top_tree = TopTree<MaxPathCluster,int,None>(size);
    top_tree.link(1,0, 1);
    top_tree.link(2,1, 1);
    top_tree.link(3,0,1);
    top_tree.link(4,3, 1);

    top_tree.link(10,0, 1);

    top_tree.link(14,1, 1);
    top_tree.link(15,10, 1);
    top_tree.link(16,10, 1);
    top_tree.link(17,16, 1);
    top_tree.link(18,16, 1);
    top_tree.link(21,18, 1);
    top_tree.link(22,8, 1);
    top_tree.link(23,16, 1);
    top_tree.link(24,7, 1);

    top_tree.expose(2);
    top_tree.deexpose(2)->print(0,false);

    std::cout << "Cutting 18 now" << std::endl;
    auto tup = top_tree.cut(21, 18);
    std::cout << "Cutting done" << std::endl;
    std::get<0>(tup)->print(0,false);
    top_tree.expose(18);
    top_tree.expose(3);
    top_tree.deexpose(18);
    top_tree.deexpose(3);
}




TEST_CASE("Max-edge-weight all edges removed", "[user data]") {
    int size = 10;
    TopTree<MaxPathCluster, int, None> top_tree = TopTree<MaxPathCluster,int,None>(size);
    top_tree.link(0,1,1);
    top_tree.link(2,1,1);
    top_tree.link(3,0,1);
    top_tree.link(4,3,1);
    top_tree.link(5,3,1);
    top_tree.link(6,3,1);
    top_tree.link(7,2,1);
    std::get<0>(top_tree.cut(3,0));
    std::get<0>(top_tree.cut(3,4));
    top_tree.cut(1,0);
    top_tree.link(2,4,1);
    top_tree.expose(7);
    top_tree.deexpose(7);
    auto root = top_tree.link(4,1,1);
    REQUIRE(root == nullptr);
}

TEST_CASE("Max-edge-weight test (small)", "[user data]") {
    int size = 10;
    TopTree<MaxPathCluster, int, None> top_tree = TopTree<MaxPathCluster,int,None>(size);
    top_tree.link(0, 1, 5);
    top_tree.link(1, 2, 10);
    SECTION("Test1") {
        MaxPathCluster* root = top_tree.expose(0, 2);
        REQUIRE(root->max_weight == 10);
    }
    SECTION("Test2") {
        MaxPathCluster* root = top_tree.expose(0, 1);
        REQUIRE(root->max_weight == 5);
    }
    SECTION("Test3") {
        MaxPathCluster* root = top_tree.expose(1, 2);
        REQUIRE(root->max_weight == 10);
    }
}
TEST_CASE("Max-edge-weight test (large)", "[user data]") {
    int size = 999;
    TopTree<MaxPathCluster, int, None> top_tree = TopTree<MaxPathCluster, int, None>(size);
    for (int i = 0; i < size - 1; i++) {
        top_tree.link(i, i + 1, i);
    }
    MaxPathCluster* root; 
    SECTION("Test1") {
        root = top_tree.expose(0, 4);
        REQUIRE(root->max_weight == 3);
    }
    SECTION("Test2") {
        root = top_tree.expose(3, 4);
        REQUIRE(root->max_weight == 3);
    }
    SECTION("Test3") {
        top_tree.expose(0);
        for (int i = 1; i < size - 1; i++) {
            root = top_tree.expose(i);
            REQUIRE(root->max_weight == i - 1);
            top_tree.deexpose(i);
        }
    }
}

TEST_CASE("Max-edge-weight cut test", "[user data]") {
    int size = 4;
    TopTree<MaxPathCluster, int, None> top_tree = TopTree<MaxPathCluster, int, None>(size);
    top_tree.link(0, 1, 1);
    top_tree.link(1, 2, 10);
    top_tree.link(2, 3, 1);
    
    MaxPathCluster* root = top_tree.expose(0, 3);
    REQUIRE(root->max_weight == 10);

    top_tree.deexpose(0, 3);
    top_tree.cut(1, 2);

    top_tree.link(1, 2, 0);
    root = top_tree.expose(0, 3);

    REQUIRE(root->max_weight == 1);
}


TEST_CASE("Test123") {
    TopTree<MaxPathCluster, int, None> top_tree = TopTree<MaxPathCluster, int, None>(10);

    top_tree.link(0,1,1);
    top_tree.link(2,1,1);
    top_tree.link(3,0,1);
    top_tree.link(4,3,1);
    top_tree.link(5,3,1);
    top_tree.link(6,3,1);
    top_tree.link(7,2,1);
    top_tree.cut(3,0);
    top_tree.cut(3,4);
    top_tree.cut(1,0);
    top_tree.link(2,4,1);
    top_tree.cut(3,6);
    top_tree.expose(7)->print(0,false);
    top_tree.deexpose(7);
    top_tree.expose(4,1);
}