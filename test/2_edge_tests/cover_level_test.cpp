#include "two_edge_cluster.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Simple cover level test", "[cover_level]")  {
    TopTree<TwoEdgeCluster,NewEdge,VertexLabel> t = TopTree<TwoEdgeCluster,NewEdge,VertexLabel>(5);
    NewEdge ed = NewEdge();
    TwoEdgeCluster::set_l_max(3);
    t.link(1,2, ed);
}

