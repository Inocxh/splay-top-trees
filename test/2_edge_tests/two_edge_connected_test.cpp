#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <deque>
#include <random>
#include "two_edge_connected.h"

bool has_endpoints(TreeEdgeData* bridge, int u, int v) {
    return (bridge->endpoints[0] == u && bridge->endpoints[1] == v) || (bridge->endpoints[0] == v && bridge->endpoints[1] == u);
}

TEST_CASE("2-edge: small", "[2-edge]") {
    TwoEdgeConnectivity tree = TwoEdgeConnectivity(50);
    std::vector<std::shared_ptr<EdgeData>> edges;
    
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
    TwoEdgeConnectivity tree = TwoEdgeConnectivity(40);
    std::vector<std::shared_ptr<EdgeData>> edges = std::vector<std::shared_ptr<EdgeData>>(40);
    TreeEdgeData* bridge;
    
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
    TwoEdgeConnectivity tree = TwoEdgeConnectivity(40);
    std::vector<std::shared_ptr<EdgeData>> edges = std::vector<std::shared_ptr<EdgeData>>(40);
    TreeEdgeData* bridge;
    
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
    TwoEdgeConnectivity tree = TwoEdgeConnectivity(20);
    std::vector<std::shared_ptr<EdgeData>> edges = std::vector<std::shared_ptr<EdgeData>>(40);
    TreeEdgeData* bridge;
    
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

    tree.expose(13,3)->print(0,false);
    tree.deexpose(13,3);

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

    tree.expose(13,3)->print(0,false);
    tree.deexpose(13,3);


    for (int i = 0; i < 26; i++) {
        tree.remove(edges[i]);
    }
}

TEST_CASE("2-edge: many parallel edges", "[2-edge]") {
    TwoEdgeConnectivity tree = TwoEdgeConnectivity(3);
    std::vector<std::shared_ptr<EdgeData>> edges;
    for (int i = 0; i < 1000; i++) {
        auto e = tree.insert(0,1);
        edges.push_back(tree.insert(0,1));
        edges.push_back(tree.insert(1,2));
        tree.remove(edges[0]);
        edges[0] = edges.back();
        edges.pop_back();
    }
    while (edges.size()>0) {
        tree.remove(edges.back());
        edges.pop_back();
    }
}

TEST_CASE("2-edge: specific", "[2-edge]") {
    TwoEdgeConnectivity tree = TwoEdgeConnectivity(6);
    std::vector<std::shared_ptr<EdgeData>> edges;
    edges.push_back(tree.insert(3,2));
    tree.insert(2,4);
    tree.insert(0,3);
    tree.insert(1,2);
   
    edges.push_back(tree.insert(0,1));
    edges.push_back(tree.insert(0,1));

    tree.remove(edges[0]);
    tree.remove(edges[2]);
}


TEST_CASE("2-edge: delete mini", "[2-edge]") {
    TwoEdgeConnectivity tree = TwoEdgeConnectivity(40);
    std::vector<std::shared_ptr<EdgeData>> edges = std::vector<std::shared_ptr<EdgeData>>(40);
    EdgeData* bridge;

    edges[0] = tree.insert(0,1);
    edges[1] = tree.insert(1,2);
    edges[2] = tree.insert(2,3);
    edges[3] = tree.insert(3,0);

    for (int i = 0; i < 4; i++) {
        tree.remove(edges[i]);
    }
}

TEST_CASE("2-edge: completely connected subgraphs", "[2-edge]") {
    int N = 100 / 2;
    TwoEdgeConnectivity tree = TwoEdgeConnectivity(N*2);
    std::vector<EdgeData*> edges;
    //First create two instances of K_50 in vertices 0..=49 and 50..=99
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < i; j++) {
            tree.insert(i,j);
        }
    }
    for (int i = N; i < 2*N; i++) {
        for (int j = N; j < i; j++ ) {
            tree.insert(i,j);
        }
    }
    //Assert that each section is completely two edge connected
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < i; j++) {
            REQUIRE(tree.two_edge_connected(i,j));
        }
    }
    for (int i = N; i < 2*N; i++) {
        for (int j = N; j < i; j++) {
            REQUIRE(tree.two_edge_connected(i,j));
        }
    }
    // No i, i+N pair is two_edge_connected.
    for (int i = 0; i < N; i++) {
        REQUIRE(!tree.two_edge_connected(i,i+N));
    }
    // Insert a single edge between - still not two_edge_connected
    auto e1 = tree.insert(0,N+N-1);
    for (int i = 0; i < N; i++) {
        REQUIRE(!tree.two_edge_connected(i,i+N));
    }
    // Insert another - now two_edge_connected
    auto e2 = tree.insert(N-1,2*N-1);
    for (int i = 0; i < N; i++) {
        REQUIRE(tree.two_edge_connected(i,i+N));
    }
    tree.remove(e1);
    // Remove the first - not connected again
    for (int i = N-1; i >= 0; i--) {
        REQUIRE(!tree.two_edge_connected(i,i+N));
    }
}

TEST_CASE("2-edge: completely connected subgraphs 2", "[2-edge]") {
    int N = 100 / 2;
    int K = 20;
    int I = 3;
    TwoEdgeConnectivity tree = TwoEdgeConnectivity(N*2);
    std::vector<std::shared_ptr<EdgeData>> edges;
    //Create two K_(N) and connect them with K edges
    // remove edges until the graph is now two_edge_connected and require that all but 1 edge have been removed when this happens
    // do this I times
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < i; j++) {
            tree.insert(i,j);
        }
    }
    for (int i = N; i < 2*N; i++) {
        for (int j = N; j < i; j++ ) {
            tree.insert(i,j);
        }
    }
    auto rng = std::default_random_engine {};
    for (int times = 0; times < I; times++) {
        for (int i = 0; i < K; i++) {
            edges.push_back(tree.insert(i,N+i));
        }
        std::shuffle(edges.begin(), edges.end(), rng);
        REQUIRE(tree.two_edge_connected(0,N+N-1));
        int i = 0;
        //Delete edges until the two components are not two_edge_connected anymore
        while (tree.two_edge_connected(0,N+N-1)) {
            auto e = edges[0];
            edges[0] = edges.back();
            edges.pop_back();

            tree.remove(e);
            i++;
        }
        //This will delete all but 1 edge! Remove it and repeat again
        REQUIRE(K-1 == i);
        tree.remove(edges[0]);
        edges.pop_back();
    }    
}


TEST_CASE("2-edge: cycle check", "[2-edge]") {
    int iterations = 20;
    int N = 200;
    TwoEdgeConnectivity tree = TwoEdgeConnectivity(N);
    
    std::deque<std::shared_ptr<EdgeData>> edges;

    for (int i = 0; i < N; i++) {
        edges.push_back(tree.insert(i, (i + 1) % N));
    }

    for (int i = 0; i < iterations; i++) {
        for (int j = 0; j < N; j++) {
            REQUIRE(tree.two_edge_connected(j, (j + 1) % N));
            tree.remove(edges.front());
            edges.pop_front();
            REQUIRE(!tree.two_edge_connected(j, (j + 1) % N));
            edges.push_back(tree.insert(j, (j + 1) % N));
        }
    }    
}