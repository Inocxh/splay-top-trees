#include <catch2/catch_test_macros.hpp>
#include <cassert>

#include <min_spanning_forest.h>


TEST_CASE("MinSpanForest") {
    int size = 10;
    MinSpanForest msf = MinSpanForest(size);

    msf.insert(0,1,1);
    msf.insert(0,1,0);
    msf.insert(1,2,2);
    msf.insert(0,2,-1);

    msf.top_tree.print_tree();
}


