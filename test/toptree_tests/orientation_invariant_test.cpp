#include <catch2/catch_test_macros.hpp>
#include "top_tree.h"
#include <cassert>
#include <iostream>

struct OrientationCluster : Node<OrientationCluster, None, None> {
    int leftmost_boundary = -1;
    int rightmost_boundary = -1;
    int central_vertex = -1;
    bool is_edge = false;

    OrientationCluster* left = nullptr;
    OrientationCluster* right = nullptr;

    void create(None* edge, None* left, None* right) {
        assert(!this->is_flipped());
        is_edge = true;
        if (this->has_left_boundary()) {
            leftmost_boundary = this->get_endpoint_id(0);
        }
        if (this->has_right_boundary()) {
            rightmost_boundary = this->get_endpoint_id(1);
        }
    };

    void merge(OrientationCluster* left, OrientationCluster* right) {
        this->left = left;
        this->right = right;
        assert(!this->is_flipped() && !left->is_flipped() && !right->is_flipped());
        if (!(left->rightmost_boundary == right->leftmost_boundary)) {
            this->print(0,false);
        }
        assert(left->rightmost_boundary == right->leftmost_boundary);
        leftmost_boundary = this->has_left_boundary() 
                            ? left->leftmost_boundary
                            : this->has_middle_boundary() 
                            ? left->rightmost_boundary
                            : -1;
        rightmost_boundary = this->has_right_boundary() 
                            ? right->rightmost_boundary
                            : this->has_middle_boundary() 
                            ? right->leftmost_boundary
                            : -1;
    };

    void swap_data() {
        std::swap(leftmost_boundary,rightmost_boundary);
        std::swap(left,right);
    }

    void print_data() {
        std::cout << "fl:" << this->is_flipped() << " l: " << leftmost_boundary 
        << "; r: " << rightmost_boundary << " id: " << std::hex <<(long long int) this<< std::dec << " " << this->get_num_boundary_vertices() << " " ;
    }
};

TEST_CASE("Orientation test 1", "[user data]") {
    int size = 26;
    auto top_tree = TopTree<OrientationCluster,None,None>(size);
    top_tree.link(1,0, None());
    top_tree.link(2,1, None())->print(0,false);
    top_tree.link(3,0, None());
    top_tree.link(4,3, None());
    //top_tree.link(5,31);
    //top_tree.link(6,31);
    //top_tree.link(7,21);
    //top_tree.link(8,1);
    //top_tree.link(9,1);
    top_tree.link(10,0, None());
    //top_tree.link(11,1);
    //top_tree.link(12,1);
    // top_tree.link(13,1);
    top_tree.link(14,1, None());
    top_tree.link(15,10, None());
    top_tree.link(16,10, None());
    top_tree.link(17,16, None());
    top_tree.link(18,16, None());
    top_tree.link(21,18, None())->print(0,false);
    auto l = top_tree.link(23,16, None());
    l->print(0,false);


    top_tree.expose(2);
    top_tree.deexpose(2);

    // top_tree.expose(22);
    // top_tree.expose(5);
    // top_tree.deexpose(22);
    // top_tree.deexpose(5);

    // top_tree.expose(22);
    // top_tree.expose(13);
    // top_tree.deexpose(22);
    // top_tree.deexpose(13);

    std::cout << "Cutting 18 now" << std::endl;
    auto tup = top_tree.cut(21, 18);
    std::cout << "Cutting done" << std::endl;
    std::get<0>(tup);
    top_tree.expose(18);
    top_tree.expose(3);
    top_tree.deexpose(18);
    top_tree.deexpose(3);
}