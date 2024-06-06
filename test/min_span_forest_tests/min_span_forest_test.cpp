#include <catch2/catch_test_macros.hpp>
#include <cassert>

#include <min_spanning_forest.h>
#include <MST.h>
#include <random>

TEST_CASE("MinSpanForest") {
    int size = 10;
    MinSpanForest msf = MinSpanForest(size);

    msf.insert(0,1,1);
    msf.insert(0,1,0);
    msf.insert(1,2,2);
    msf.insert(0,2,-1);

    msf.top_tree.print_tree();
}



TEST_CASE("Kruskals 1") {
    int size = 100;
    int ops = 10000;
    int rounds = 20;

    MinSpanForest msf = MinSpanForest(size);
    MSTGraph mst_graph = MSTGraph(size);

    std::random_device dev;
    std::mt19937 rng(0);
    std::uniform_int_distribution<std::mt19937::result_type> rand(0,size - 1);
    

    for (int j = 0; j < rounds; j++) {
        for (int i = 0; i < ops; i++) {
            int u = rand(rng);
            int v = rand(rng);
            int w = rand(rng);
            if (u == v) {
                continue;
            }
            // std::cout << "Insert: " << u << "," << v << ":" << w << std::endl;
            msf.insert(u, v, w);
            mst_graph.add_edge(u, v, w);
        }

        msf.top_tree.print_tree();
        REQUIRE(mst_graph.MST_weight() == msf.weight);

    }


}



