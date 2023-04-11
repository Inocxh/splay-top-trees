#include <catch2/catch_test_macros.hpp>
#include "max_weight_path_cluster.hpp"

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