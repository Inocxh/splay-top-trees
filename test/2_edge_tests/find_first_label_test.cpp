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


    
    root = tree.expose(2,7);
    auto res = root->find_first_label(7,2,0);
    auto ffl = std::get<1>(res);
    REQUIRE(ffl->labels[0].size() > 0);
    ffl = std::get<1>(root->find_first_label(7,2,1));
    REQUIRE(ffl->labels[1].size() > 0);
    tree.deexpose(2,7);

    tree.insert(4,12);
    tree.insert(0,12,0);
    root = tree.expose(1,10);
    ffl = std::get<1>(root->find_first_label(10,1,0));
    ffl = std::get<1>(root->find_first_label(1,10,0));
    tree.deexpose(1,10);
}

TEST_CASE("FFL: cut", "[find first label]") {
    TwoEdgeConnectivity tree = TwoEdgeConnectivity(50);
    tree.insert(1,2);
    tree.insert(2,3);
    tree.insert(3,4);
    tree.insert(4,5);
    tree.insert(5,6);

    tree.insert(1,8);
    tree.insert(8,9);

    tree.insert(3,10);
    tree.insert(10,11);

    tree.insert(4,12);
    tree.insert(12,13);

    tree.insert(5,14);
    tree.insert(6,15);

    tree.insert(8,11,0);
    tree.insert(13,14,1);
    tree.insert(14,15,0);

    auto root = tree.expose(1,6);
    auto res = root->find_first_label(1,6,0);
    auto ffl = std::get<1>(res);
    REQUIRE((ffl->labels[0][0]->endpoints[0] == 8 && ffl->labels[0][0]->endpoints[1] == 11));
    ffl = std::get<1>(root->find_first_label(6,1,0));
    REQUIRE((ffl->labels[0][0]->endpoints[0] == 14 && ffl->labels[0][0]->endpoints[1] == 15));
    ffl = std::get<1>(root->find_first_label(1,6,1));    
    REQUIRE((ffl == nullptr));
    ffl = std::get<1>(root->find_first_label(6,1,1));  
    REQUIRE((ffl == nullptr));
    tree.deexpose(1,6);
    //TODO CUT
}