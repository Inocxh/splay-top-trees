#include "two_edge_cluster.h"
#include <catch2/catch_test_macros.hpp>

using TwoEdgeTree = TopTree<TwoEdgeCluster,TreeEdgeData,None>;

TEST_CASE("Simple cover level test", "[cover_level]")  {
    TwoEdgeTree t = TwoEdgeTree(5);
    TwoEdgeCluster::set_l_max(3);
    t.link(1,2, TreeEdgeData(1,2));
}

