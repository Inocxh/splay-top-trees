#include <catch2/catch_test_macros.hpp>

#define TEST 1
#include <cassert>
#include "top_tree.h"



struct DiameterCluster : Node<DiameterCluster, int, None> {
    int length;
    int diameter;

    int max_dist[2] = {-100,-200}; //0 leftmost, 1 rightmost

    void merge_leaf(int* edge_data, None* left, None* right) {
        assert(!this->is_flipped());

        if (this->is_path()) {
            this->length = *edge_data;
        } else {
            this->length = 0;
        }
 
        //this->max_dist[this->flipped] = this->has_left_boundary() ? *edge_data : 0;
        //this->max_dist[!this->flipped] = this->has_right_boundary() ? *edge_data : 0;
        this->max_dist[0] = this->has_left_boundary() ? *edge_data : 0;
        this->max_dist[1] = this->has_right_boundary() ? *edge_data : 0;

        this->diameter = *edge_data;
    };

    void split_leaf(int* edge_data, None* left, None* right) {};

    void merge_internal(DiameterCluster* left, DiameterCluster* right) {
        assert(!this->is_flipped()); 
        /*assert(!left->is_flipped()); 
        assert(!right->is_flipped());*/



        bool left_flipped = left->is_flipped();
        bool right_flipped = right->is_flipped();
        this->length = (left->is_path() ? left->length : 0) +
                       (right->is_path() ? right->length : 0);

        //By orientation invariant, middle is leftmost of right and rightmost of left.
        this->diameter = std::max(std::max(
            left->diameter, 
            right->diameter), 
            left->max_dist[!left_flipped] + right->max_dist[right_flipped]
        );
        
        if (this->has_left_boundary()) {
            this->max_dist[0] = std::max(left->max_dist[left_flipped], left->length + right->max_dist[right_flipped]);
        }
        if (this->has_right_boundary()) {
            this->max_dist[1] = std::max(right->max_dist[!right_flipped], right->length + left->max_dist[!left_flipped]);
        }
        if (this->has_middle_boundary()) {
            this->max_dist[this->has_left_boundary()] = std::max(left->max_dist[!left_flipped], right->max_dist[right_flipped]);
        }
    };
    void split_internal(DiameterCluster* left, DiameterCluster* right) {}; 

    void print_data() {
        std::cout << "[length: " << this->length <<" diameter: " << this->diameter <<" max_dist: " << this->max_dist[0] << "; " << this->max_dist[1] << "]:";
    }
};

TEST_CASE("Simple test 2", "[user data]")
{
    int size = 10;
    int vertex_id = 1;
    TopTree<DiameterCluster, int, None> top_tree = TopTree<DiameterCluster, int, None>(size);
    top_tree.expose(vertex_id);
    REQUIRE((top_tree.get_vertex(vertex_id)->is_exposed()));
}

TEST_CASE("Diameter small example", "[user data]") {
    int size = 10;

    InternalNode<DiameterCluster,int,None>* lol;
    DiameterCluster* root;
    TopTree<DiameterCluster, int, None> top_tree = TopTree<DiameterCluster, int, None>(size);
    top_tree.link(0, 1, 2);
    top_tree.link(1, 2, 1);
    top_tree.link(2, 3, 1);
    root = top_tree.link(3, 4, 1);
    lol = (InternalNode<DiameterCluster,int,None>*) root;
    lol->print(0, false);
    std::cout << std::endl;
    

        root = top_tree.expose(0);
        lol = (InternalNode<DiameterCluster,int,None>*) root;
        lol->print(0, false); 
        REQUIRE(root->diameter == 5);
        top_tree.deexpose(0);

        top_tree.cut(1,2);

        root = top_tree.expose(0);
        REQUIRE(root->diameter == 2);
        top_tree.deexpose(0);

        root = top_tree.expose(3);
        REQUIRE(root->diameter == 2);
        top_tree.deexpose(3);
}

TEST_CASE("Diameter full example", "[user data]") {
    int size = 10;
    TopTree<DiameterCluster, int, None> top_tree = TopTree<DiameterCluster, int, None>(size);
    top_tree.link(0, 1, 2);
    top_tree.link(1, 2, 2);
    top_tree.link(1, 3, 3);
    top_tree.link(3, 8, 1);
    top_tree.link(8, 9, 2);
    top_tree.link(3, 4, 5);
    top_tree.link(4, 5, 1);
    top_tree.link(4, 6, 1);
    top_tree.link(6, 7, 1);
    
    DiameterCluster* root;

    SECTION("Diameter") {
        root = top_tree.expose(0);        
        REQUIRE(root->diameter == 12);
        
        top_tree.deexpose(0);
        top_tree.cut(1, 3);

        root = top_tree.expose(0);
        REQUIRE(root->diameter == 4);
        top_tree.deexpose(0);

        root = top_tree.expose(6);
        REQUIRE(root->diameter == 10);
        top_tree.deexpose(6);

        top_tree.link(0, 5, 1);
        root = top_tree.expose(7);
        //InternalNode<DiameterCluster,int,None>* lol = (InternalNode<DiameterCluster,int,None>*) root;
        //lol->print(0, false);
        REQUIRE(root->diameter == 14);

    }   
}



TEST_CASE("Diameter complete example", "[user data]") {
    int size = 10;
    TopTree<DiameterCluster, int, None> top_tree = TopTree<DiameterCluster, int, None>(size);
    DiameterCluster* root;

    top_tree.link(0, 1, 3);
    root = top_tree.expose(0);        
    REQUIRE(root->diameter == 3);
    top_tree.deexpose(0);
    
    top_tree.link(2, 3, 4);
    root = top_tree.expose(3);        
    REQUIRE(root->diameter == 4);
    top_tree.deexpose(3);

    top_tree.link(1, 2, 1);
    root = top_tree.expose(2);        
    REQUIRE(root->diameter == 8);
    top_tree.deexpose(2);

    top_tree.link(5, 4, 5);
    root = top_tree.expose(2);        
    REQUIRE(root->diameter == 8);
    top_tree.deexpose(2);
    root = top_tree.expose(5);        
    REQUIRE(root->diameter == 5);
    top_tree.deexpose(5);
    
    top_tree.link(1, 4, 1);
    root = top_tree.expose(2);        
    REQUIRE(root->diameter == 11);
    top_tree.deexpose(2);
    
    top_tree.cut(1, 2);
    root = top_tree.expose(0);        
    REQUIRE(root->diameter == 9);
    top_tree.deexpose(0);

    top_tree.cut(1, 0);
    root = top_tree.expose(4);
    REQUIRE(root->diameter == 6);
    top_tree.deexpose(4);
}   

