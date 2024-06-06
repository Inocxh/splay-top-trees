#include "two_edge_cluster.h"
#include <catch2/catch_test_macros.hpp>


//TODO: Change TreeEdgeData() arguments
using TwoEdgeTree = TopTree<TwoEdgeCluster,TreeEdgeData,None>;

void cover(TwoEdgeTree *T, int v, int w, int i) {
    TwoEdgeCluster *root = T->expose(v,w);
    root->cover(i);
    T->deexpose(v,w);
}
void uncover(TwoEdgeTree *T, int v, int w, int i) {
    TwoEdgeCluster *root = T->expose(v,w);
    root->uncover(i);
    root->print(0, false);
    std::cout << std::endl;
    T->deexpose(v,w);
}

TEST_CASE("Simple find size test", "[find size test]")  {
    TwoEdgeTree T = TwoEdgeTree(10);
    TwoEdgeCluster::set_l_max(3);

    T.link(1,2,TreeEdgeData());
    T.link(2,3,TreeEdgeData());
    T.link(3,4,TreeEdgeData());
    T.link(4,5,TreeEdgeData());
    T.link(6,3,TreeEdgeData());
    T.link(6,7,TreeEdgeData());
    T.link(2,8,TreeEdgeData());
    T.link(9,8,TreeEdgeData())->print(0, false);
    std::cout << "\n";
    cover(&T,1,5,0);
    TwoEdgeCluster *root = T.expose(1, 5);
    root->print(0,false);
    REQUIRE(root->get_size(0) == 5);

}
TEST_CASE("Small find size", "[find size test]")  {
    TwoEdgeTree T = TwoEdgeTree(8);
    TwoEdgeCluster::set_l_max(4);
    
    T.link(1,2, TreeEdgeData());
    T.link(2,3, TreeEdgeData());

    cover(&T,1,3,0);
    TwoEdgeCluster *root = T.expose(1, 3);
    root->print(0,false);
    REQUIRE(root->get_size(0) == 3);

}


TEST_CASE("Find size test", "[find size test]")  {
    TwoEdgeTree T = TwoEdgeTree(12);
    TwoEdgeCluster::set_l_max(3);
    TwoEdgeCluster *root;

    T.link(0,1, TreeEdgeData());
    T.link(1,2, TreeEdgeData());
    T.link(2,3, TreeEdgeData());
    T.link(2,4, TreeEdgeData());
    T.link(4,5, TreeEdgeData());
    T.link(5,6, TreeEdgeData());
    T.link(5,7, TreeEdgeData());
    T.link(4,8, TreeEdgeData());
    T.link(8,9, TreeEdgeData());

    cover(&T,1,7,0);
    root = T.expose(1, 7);
    REQUIRE(root->get_size(0) == 5);
    T.deexpose(1, 7);
    
    T.cut(5,4);
    T.link(4,6, TreeEdgeData());
    
    root = T.expose(1, 7);
    REQUIRE(root->get_size(0) == 6);
    T.deexpose(1, 7);
    
    cover(&T,3,9,0);

    root = T.expose(0, 3);
    root->print(0,false);
    REQUIRE(root->get_size(0) == 7);
    T.deexpose(0, 3);

}


TEST_CASE("FS: Massive", "[find size test]")  {
    TwoEdgeTree T = TwoEdgeTree(20);
    TwoEdgeCluster::set_l_max(4);
    TwoEdgeCluster *root;

    T.link(0,1, TreeEdgeData());
    T.link(1,2, TreeEdgeData());
    T.link(2,3, TreeEdgeData());
    T.link(1,4, TreeEdgeData()); 

    cover(&T,2,3,1);

    T.link(5,6, TreeEdgeData());
    T.link(6,7, TreeEdgeData());
    T.link(7,8, TreeEdgeData());
    T.link(6,9, TreeEdgeData()); 
    T.link(9,10, TreeEdgeData());
    T.link(11,9, TreeEdgeData()); 
    cover(&T,5,11,0);
    cover(&T,10,8,1);

    root = T.expose(5,8);
    REQUIRE(root->get_size(0) == 7);
    REQUIRE(root->get_size(1) == 6);
    REQUIRE(root->get_size(2) == 4);
    T.deexpose(5, 8);

    T.link(4,10, TreeEdgeData());
    T.link(8,12, TreeEdgeData()); 
    cover(&T,10,6,2);
    cover(&T,10,1,3);

    root = T.expose(5,8);
    REQUIRE(root->get_size(0) == 9);
    REQUIRE(root->get_size(1) == 8);
    REQUIRE(root->get_size(2) == 8);
    T.deexpose(5, 8);   
}



TEST_CASE("FS: Uncover", "[find size test]")  {
    TwoEdgeTree T = TwoEdgeTree(20);
    TwoEdgeCluster::set_l_max(4);
    TwoEdgeCluster *root;

    T.link(0,1, TreeEdgeData());
    T.link(1,2, TreeEdgeData());
    T.link(2,3, TreeEdgeData());
    T.link(1,4, TreeEdgeData()); 

    cover(&T,0,3,0);
    cover(&T,1,2,2);
    
    T.link(4,5, TreeEdgeData()); 

    cover(&T,5,2,1);

    root = T.expose(0,4);
    REQUIRE(root->get_size(0) == 6);
    REQUIRE(root->get_size(1) == 5);
    REQUIRE(root->get_size(2) == 4);
    T.deexpose(0,4);
    

    uncover(&T, 4, 3, 0);
    root = T.expose(0,4);
    REQUIRE(root->get_size(0) == 5);
    REQUIRE(root->get_size(1) == 5);
    REQUIRE(root->get_size(2) == 4);
    T.deexpose(0,4);

    uncover(&T, 4, 0, 1);
    root = T.expose(1,5);
    REQUIRE(root->get_size(0) == 4);
    REQUIRE(root->get_size(1) == 4);
    REQUIRE(root->get_size(2) == 4);
    REQUIRE(root->get_size(3) == 3);
    T.deexpose(1,5);


    cover(&T, 0, 3, 1);
    uncover(&T, 0, 2, 1);
    cover(&T, 0, 5, 3);
    uncover(&T, 4, 2, 2);

    root = T.expose(4,1);
    REQUIRE(root->get_size(0) == 4);
    REQUIRE(root->get_size(3) == 4);
    T.deexpose(4,1);

}

TEST_CASE("FS: Uncover massive", "[find size test]")  {
    TwoEdgeTree T = TwoEdgeTree(20);
    TwoEdgeCluster::set_l_max(4);
    TwoEdgeCluster *root;

    T.link(0,1, TreeEdgeData());
    T.link(1,2, TreeEdgeData());
    T.link(2,3, TreeEdgeData());
    T.link(3,4, TreeEdgeData());
    T.link(4,5, TreeEdgeData());
    T.link(5,6, TreeEdgeData());
    T.link(6,7, TreeEdgeData());
    
    cover(&T, 2, 5, 1);
    
    T.link(3,8, TreeEdgeData());
    T.link(8,9, TreeEdgeData());
    T.link(8,10, TreeEdgeData());
    
    cover(&T, 9, 1, 2);
    
    T.link(5,11, TreeEdgeData());
    T.link(11,12, TreeEdgeData());
    T.link(12,13, TreeEdgeData());
    T.link(13,14, TreeEdgeData());
    T.link(11,15, TreeEdgeData());

    cover(&T,14,4,0);
    cover(&T,15,7,1);
    cover(&T,5,11,2);
    
    T.link(4,16, TreeEdgeData());    

    cover(&T,16,8,3);
        
    root = T.expose(1,4);
    REQUIRE(root->get_size(0) == 15);
    REQUIRE(root->get_size(1) == 12);
    REQUIRE(root->get_size(2) == 7);
    REQUIRE(root->get_size(3) == 6);
    T.deexpose(1,4);

    root = T.expose(16,6);
    REQUIRE(root->get_size(0) == 15);
    REQUIRE(root->get_size(1) == 12);
    REQUIRE(root->get_size(2) == 10);
    REQUIRE(root->get_size(3) == 6);
    T.deexpose(16,6);

    uncover(&T,8,5,1);
    uncover(&T,9,0,2);
    cover(&T, 2, 10, 0);
    uncover(&T,12,13,3);
    uncover(&T,16,15,1);
    uncover(&T,16,12,2);
    cover(&T, 12, 6, 0);

    root = T.expose(1,4);
    REQUIRE(root->get_size(0) == 7);
    REQUIRE(root->get_size(1) == 6);
    REQUIRE(root->get_size(2) == 6);
    T.deexpose(1,4);

    root = T.expose(16,6);
    root->print(0,false);
    REQUIRE(root->get_size(0) == 11); 
    REQUIRE(root->get_size(1) == 7); 
    REQUIRE(root->get_size(2) == 6); 
    REQUIRE(root->get_size(3) == 6); 
    T.deexpose(16,6);
}

TEST_CASE("Small", "[find size test]") {

    TwoEdgeTree T = TwoEdgeTree(7);
    TwoEdgeCluster::set_l_max(4);
    TwoEdgeCluster *root;

    T.link(0,6, TreeEdgeData());
    T.link(0,1, TreeEdgeData());
    T.link(1,2, TreeEdgeData());
    T.link(2,3, TreeEdgeData());
    T.link(3,4, TreeEdgeData());
    T.link(4,5, TreeEdgeData());
    
    cover(&T, 1, 5, 0);
    cover(&T, 2, 5, 1);
    cover(&T, 3, 5, 2);
    cover(&T, 4, 5, 3);

    cover(&T, 0, 6, 3);

    uncover(&T, 0, 5, 0);
    uncover(&T, 0, 5, 1);
    uncover(&T, 0, 5, 2);

    T.expose(0,6);
    T.deexpose(0,6);
    T.expose(3,4);
    T.deexpose(3,4);
    T.expose(0,4)->print(0, false);
    T.deexpose(0,4);
}


TEST_CASE("FS: minified massive", "[find size test]")  {
    TwoEdgeTree T = TwoEdgeTree(8);
    TwoEdgeCluster::set_l_max(2);
    TwoEdgeCluster *root;

    T.link(0,1, TreeEdgeData());
    T.link(1,2, TreeEdgeData());
    T.link(2,3, TreeEdgeData());
    T.link(1,4, TreeEdgeData()); 

    cover(&T,2,3,1);
    cover(&T,0,1,0);

    T.link(2,5, TreeEdgeData());
    T.link(5,6, TreeEdgeData()); 
    T.link(5,7, TreeEdgeData());

    cover(&T,2,7,0);
    cover(&T,2,5,1);

    root = T.expose(4,2);
    REQUIRE(root->get_size(0) == 7);
    REQUIRE(root->get_size(1) == 5);
    T.deexpose(4, 2); 
}

TEST_CASE("FS: cut", "[find size test]")  {
    TwoEdgeTree T = TwoEdgeTree(20);
    TwoEdgeCluster::set_l_max(4);
    TwoEdgeCluster *root;

    T.link(0,1, TreeEdgeData());
    T.link(1,2, TreeEdgeData());
    T.link(2,3, TreeEdgeData());
    T.link(3,4, TreeEdgeData()); 

    T.link(1,5, TreeEdgeData());
    T.link(5,6, TreeEdgeData());
    T.link(6,7, TreeEdgeData());
    
    T.link(5,8, TreeEdgeData());
    T.link(8,9, TreeEdgeData());
    T.link(9,10, TreeEdgeData());

    T.link(8,11, TreeEdgeData());
    T.link(11,12, TreeEdgeData());

    cover(&T,0,10,0);
    cover(&T,12,7,1);

    root = T.expose(0,2);
    REQUIRE(root->get_size(0) == 11);
    REQUIRE(root->get_size(1) == 3);
    T.deexpose(0, 2); 

    T.cut(5,8);

    root = T.expose(8,9);
    REQUIRE(root->get_size(0) == 5);
    REQUIRE(root->get_size(1) == 4);
    T.deexpose(8,9);

    root = T.expose(0,2);
    REQUIRE(root->get_size(0) == 6);
    REQUIRE(root->get_size(1) == 3);
    T.deexpose(0, 2); 


    cover(&T,6,3,0);

    T.cut(5,1);
    T.link(7,4, TreeEdgeData());
    T.link(10,7, TreeEdgeData());

    cover(&T,7,4,2);
    cover(&T,7,3,1);
    cover(&T,7,10,1);
    uncover(&T,7,9,0);

    root = T.expose(6,7);
    REQUIRE(root->get_size(0) == 9);
    REQUIRE(root->get_size(1) == 6);
    T.deexpose(6, 7); 


    T.cut(6,7);
    T.cut(1,2);
    root = T.expose(4,7);
    REQUIRE(root->get_size(0) == 5);
    REQUIRE(root->get_size(1) == 4);
    T.deexpose(4, 7); 


    cover(&T,12,2,0);

    T.link(6,9, TreeEdgeData());

    cover(&T,9,5, 2);
    uncover(&T,2,4,1);
    cover(&T,3,4,2);

    root = T.expose(7,8);
    REQUIRE(root->get_size(0) == 10);
    REQUIRE(root->get_size(1) == 10);
    REQUIRE(root->get_size(2) == 8);
    T.deexpose(7,8); 
    

    T.cut(9,10);
    T.link(6,7, TreeEdgeData());
    T.link(1,6, TreeEdgeData());
    T.cut(8,9);
    T.cut(7,4);


    root = T.expose(8,11);
    REQUIRE(root->get_size(0) == 3);
    REQUIRE(root->get_size(1) == 3);
    REQUIRE(root->get_size(2) == 2);
    T.deexpose(11,8); 

    root = T.expose(1,6);
    REQUIRE(root->get_size(0) == 5);
    REQUIRE(root->get_size(1) == 4);
    REQUIRE(root->get_size(2) == 4);
    REQUIRE(root->get_size(3) == 2);
    T.deexpose(1,6); 

    root = T.expose(2,3);
    REQUIRE(root->get_size(0) == 3);
    REQUIRE(root->get_size(1) == 3);
    REQUIRE(root->get_size(2) == 3);
    REQUIRE(root->get_size(3) == 2);
    T.deexpose(2,3); 

}

