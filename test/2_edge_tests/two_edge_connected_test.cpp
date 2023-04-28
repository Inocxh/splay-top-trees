#include <catch2/catch_test_macros.hpp>
#include <cassert>
#include "two_edge_connected.h"



bool has_endpoints(NewEdge* bridge, int u, int v) {
    return (bridge->endpoints[0] == u && bridge->endpoints[1] == v) || (bridge->endpoints[0] == v && bridge->endpoints[1] == u);
}

TEST_CASE("2-edge: small", "[2-edge]") {
    TwoEdgeConnectivity tree = TwoEdgeConnectivity(50);
    vector<NewEdge*> edges;
    
    edges.push_back(tree.insert(0,1));
    edges.push_back(tree.insert(1,2));
    tree.insert(2,3);
    tree.insert(3,4);  
    tree.insert(4,5); 
    tree.insert(0,2);
    tree.insert(3,5);

    
    auto bridge = tree.find_bridge(0, 5);
    REQUIRE(has_endpoints(bridge, 2, 3));
    tree.insert(0,5);
    tree.remove(edges.back());
    edges.pop_back();
    tree.expose(0,5)->print(0,false);
    tree.deexpose(0,5);
    bridge = tree.find_bridge(0,5);
    REQUIRE(bridge == nullptr);
    bridge = tree.find_bridge(1,5);
    REQUIRE(has_endpoints(bridge, 0, 1));
}
TEST_CASE("2-edge: Medium", "[2-edge]") {
    TwoEdgeConnectivity tree = TwoEdgeConnectivity(50);
    vector<NewEdge*> edges = vector<NewEdge*>(40);
    NewEdge* bridge;
    
    edges[0] = tree.insert(0,1);
    edges[1] = tree.insert(1,2);
    edges[2] = tree.insert(2,3);
    edges[3] = tree.insert(3,4);  
    edges[4] = tree.insert(4,5); 
    

    edges[5] = tree.insert(2,6);
    edges[6] = tree.insert(6,7);  
    edges[7] = tree.insert(7,8);

    edges[8] = tree.insert(7,9);
    edges[9] = tree.insert(9,10);  
    edges[10] = tree.insert(10,11);

    // Non tree edges
    edges[11] = tree.insert(0,5);
    edges[12] = tree.insert(11,9);
    edges[13] = tree.insert(10,6);
    edges[14] = tree.insert(7,8);

    bridge = tree.find_bridge(0,5);
    REQUIRE(bridge == nullptr);
    bridge = tree.find_bridge(6,11);
    REQUIRE(bridge == nullptr);
    bridge = tree.find_bridge(8,5);
    REQUIRE(has_endpoints(bridge, 2, 6));
    bridge = tree.find_bridge(11,0);
    REQUIRE(has_endpoints(bridge, 2, 6));

    tree.remove(edges[5]);
    tree.remove(edges[7]);
    tree.remove(edges[11]);
    edges[14] = tree.insert(6,1);
    edges[15] = tree.insert(6,0);
    edges[16] = tree.insert(6,8);
    edges[17] = tree.insert(2,5);
    
    bridge = tree.find_bridge(11,5);
    REQUIRE(has_endpoints(bridge, 1, 2));
    bridge = tree.find_bridge(8,5);
    REQUIRE(has_endpoints(bridge, 1, 2));
    bridge = tree.find_bridge(8,11);
    REQUIRE(bridge == nullptr);



}
TEST_CASE("2-edge: large", "[2-edge]") {
    TwoEdgeConnectivity tree = TwoEdgeConnectivity(50);
    vector<NewEdge*> edges = vector<NewEdge*>(40);
    NewEdge* bridge;
    
    edges[0] = tree.insert(0,1);
    edges[1] = tree.insert(1,2);
    edges[2] = tree.insert(2,3);
    edges[3] = tree.insert(3,4);  
    edges[4] = tree.insert(4,5); 
    

    edges[5] = tree.insert(2,6);
    edges[6] = tree.insert(6,7);  
    edges[7] = tree.insert(7,8);

    edges[8] = tree.insert(7,9);
    edges[9] = tree.insert(9,10);  
    edges[10] = tree.insert(10,11);

    // Non tree edges
    edges[11] = tree.insert(0,5);
    edges[12] = tree.insert(11,9);
    edges[13] = tree.insert(10,6);
    edges[14] = tree.insert(7,8);

    bridge = tree.find_bridge(0,5);
    REQUIRE(bridge == nullptr);
    bridge = tree.find_bridge(6,11);
    REQUIRE(bridge == nullptr);
    bridge = tree.find_bridge(8,5);
    REQUIRE(has_endpoints(bridge, 2, 6));
    bridge = tree.find_bridge(11,0);
    REQUIRE(has_endpoints(bridge, 2, 6));

    tree.remove(edges[5]);
    tree.remove(edges[7]);
    tree.remove(edges[11]);
    edges[14] = tree.insert(3,12);
    edges[15] = tree.insert(12,13);
    edges[16] = tree.insert(5,14);
    edges[17] = tree.insert(5,14);
    edges[18] = tree.insert(12,14);
    edges[19] = tree.insert(13,0);
    edges[20] = tree.insert(6,0);
    edges[21] = tree.insert(6,4);
    edges[22] = tree.insert(1,12);
    tree.remove(edges[19]);
    edges[23] = tree.insert(8,6);

    bridge = tree.find_bridge(11,8);
    REQUIRE(bridge == nullptr);
    bridge = tree.find_bridge(8,14);
    REQUIRE(bridge == nullptr);
    bridge = tree.find_bridge(11,14);
    REQUIRE(bridge == nullptr);
    bridge = tree.find_bridge(11,13);
    REQUIRE(has_endpoints(bridge, 12, 13));



    tree.expose(4,6)->print(0,false);
    tree.deexpose(4,6);
    tree.remove(edges[21]);
    edges[24] = tree.insert(13,4);
    edges[25] = tree.insert(0,7);

    bridge = tree.find_bridge(11,6);
    REQUIRE(bridge == nullptr);
    bridge = tree.find_bridge(8,14);
    REQUIRE(has_endpoints(bridge, 0, 1));
    bridge = tree.find_bridge(11,13);
    REQUIRE(has_endpoints(bridge, 0, 1));
    bridge = tree.find_bridge(13,14);
    REQUIRE(bridge == nullptr);

}
TEST_CASE("2-edge: delete all", "[2-edge]") {
    TwoEdgeConnectivity tree = TwoEdgeConnectivity(50);
    vector<NewEdge*> edges = vector<NewEdge*>(40);
    NewEdge* bridge;
    
    edges[0] = tree.insert(0,1);
    edges[1] = tree.insert(1,2);
    edges[2] = tree.insert(2,3);
    edges[3] = tree.insert(3,4);  
    edges[4] = tree.insert(4,5); 
    edges[5] = tree.insert(2,6);
    edges[6] = tree.insert(6,7);  
    edges[7] = tree.insert(7,8);
    edges[8] = tree.insert(7,9);
    edges[9] = tree.insert(9,10);  
    edges[10] = tree.insert(10,11);
    edges[11] = tree.insert(0,5);
    edges[12] = tree.insert(11,9);
    edges[13] = tree.insert(10,6);
    edges[14] = tree.insert(7,8);
    edges[14] = tree.insert(3,12);
    edges[15] = tree.insert(12,13);
    edges[16] = tree.insert(5,14);
    edges[17] = tree.insert(5,14);
    edges[18] = tree.insert(12,14);
    edges[19] = tree.insert(13,0);
    edges[20] = tree.insert(6,0);
    edges[21] = tree.insert(6,4);
    edges[22] = tree.insert(1,12);
    edges[23] = tree.insert(8,6);
    edges[24] = tree.insert(13,4);

    edges[25] = tree.insert(0,7);

    for (int i = 0; i < 26; i++) {
        tree.remove(edges[i]);
    }
}

TEST_CASE("2-edge: delete mini", "[2-edge]") {
    TwoEdgeConnectivity tree = TwoEdgeConnectivity(50);
    vector<NewEdge*> edges = vector<NewEdge*>(40);
    NewEdge* bridge;
    
    /*edges[0] = tree.insert(0,1);
    edges[1] = tree.insert(1,2);
    edges[2] = tree.insert(2,3);
    edges[3] = tree.insert(3,4);  
    edges[4] = tree.insert(4,5); 
    edges[5] = tree.insert(3,12);
    edges[6] = tree.insert(12,13);  
    edges[7] = tree.insert(13,4);*/

    edges[0] = tree.insert(0,1);
    edges[1] = tree.insert(1,2);
    edges[2] = tree.insert(2,3);
    edges[3] = tree.insert(3,0);

    for (int i = 0; i < 4; i++) {
        bool ost = edges[3]->edge_type == TreeEdge;
        tree.remove(edges[i]);
    }
}

TEST_CASE("2-edge: delete micro", "[2-edge]") {
    NewEdge* lol = NewEdge::new_non_tree_edge(0,0,0);
    NewEdge* lol2 = NewEdge::new_tree_edge(1,1,1,0);
    NewEdge::swap(lol,lol2);
    
    REQUIRE(lol->edge_type == TreeEdge);
    REQUIRE(lol->endpoints[0] == 1);

    
}