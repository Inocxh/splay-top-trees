#include "find_size.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Simple cover level test 123", "[cover_level123]")  {
    TopTree<FindSizeCluster,EdgeData,None> t = TopTree<FindSizeCluster,EdgeData,None>(8);
    CoverLevelCluster::set_l_max(3);
    EdgeData* ed[10];
    for (int i = 0; i < 10; i++) {
        ed[i] = new EdgeData();
    }
    
    t.link(1,2, *ed[0]);
    //t.link(3,2, *ed[1]);
}
