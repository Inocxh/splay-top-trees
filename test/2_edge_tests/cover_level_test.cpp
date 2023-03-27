#include "cover_level.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Simple cover level test", "[cover_level]")  {
    TopTree<CoverLevelCluster,CoverLevel,None> t = TopTree<CoverLevelCluster,CoverLevel,None>(5);
    t.link(1,2,9);
}

