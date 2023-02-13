#include <catch2/catch_test_macros.hpp>
#include "underlying_tree.h"

TEST_CASE( "Factorials are computed", "[a]" ) {
    REQUIRE( 1+ 1 ==2 );
}

TEST_CASE( "Create tree", "[create_tree]") {
    int size = 10;
    Tree tree = Tree(size);
    REQUIRE(tree.num_vertices == size);


}


