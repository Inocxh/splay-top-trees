#include "cover_level.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Simple cover level test", "[cover_level]")  {
    TopTree<CoverLevelCluster,EdgeData,None> t = TopTree<CoverLevelCluster,EdgeData,None>(5);
    EdgeData* ed = new EdgeData();
    CoverLevelCluster::set_l_max(3);
    t.link(1,2, *ed);
}

