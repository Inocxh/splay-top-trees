#include <catch2/catch_test_macros.hpp>
#include <cassert>
#include "two_edge_connected.h"


TEST_CASE("2-edge: small", "[2-edge]") {
    TwoEdgeConnectivity tree = TwoEdgeConnectivity(50);
    vector<GEdge*> edges;
    
    edges.push_back(tree.insert(0,1));
    edges.push_back(tree.insert(1,2));
    tree.insert(2,3);
    tree.insert(3,4);  
    tree.insert(4,5); 
    tree.insert(0,2);
    tree.insert(3,5);

    
    auto bridge = tree.find_bridge(0, 5);
    REQUIRE((bridge->endpoints[0] == 2 && bridge->endpoints[1] == 3));
    tree.insert(0,5);
    tree.remove(edges.back());
    edges.pop_back();
    tree.expose(0,5)->print(0,false);
    tree.deexpose(0,5);
    bridge = tree.find_bridge(0,5);
    REQUIRE(bridge == nullptr);
    bridge = tree.find_bridge(1,5);
    REQUIRE((bridge->endpoints[0] == 0 && bridge->endpoints[1] == 1));

}