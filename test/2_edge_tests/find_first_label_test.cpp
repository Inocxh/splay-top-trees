#include <catch2/catch_test_macros.hpp>
#include <cassert>
#include "two_edge_connected.h"


TEST_CASE("FFL: small", "[find first label]") {
    TwoEdgeConnectivity tree = TwoEdgeConnectivity(6);
    tree.insert(0,1);
    tree.insert(1,2);
    tree.insert(2,3);
    tree.insert(3,4);  
    tree.insert(4,5); 
    tree.insert(0,5);  
    auto root = tree.expose(0,5);
    root->print(0, false);
    tree.deexpose(0,5);
    
    root = tree.expose(0,1);
    root->print(0, false);
    tree.deexpose(0,1);

    root = tree.expose(2,3);
    root->print(0, false);
    tree.deexpose(2,3);

}

TEST_CASE("FFL: large", "[find first label]") {
    TwoEdgeConnectivity tree = TwoEdgeConnectivity(50);
    tree.insert(0,1);
    tree.insert(1,2);
    tree.insert(2,3);
    tree.insert(1,4);
    tree.insert(4,5);
    tree.insert(5,6);
    tree.insert(4,7);
    tree.insert(7,8);
    tree.insert(8,9);
    tree.insert(7,10);
    tree.insert(10,11);

    tree.insert(6,9,0);

    auto root = tree.expose(1,7);
    REQUIRE(root->get_incident() == 0b0001);
    tree.deexpose(7);
    root = tree.expose(3);
    REQUIRE(root->get_incident() == 0b0000);
    tree.deexpose(1,3);

    tree.insert(8,11,1);
    root = tree.expose(1,7);
    REQUIRE(root->get_incident() == 0b0011);
    tree.deexpose(1,7);

}