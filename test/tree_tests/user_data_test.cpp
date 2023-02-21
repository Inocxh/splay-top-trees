#include <catch2/catch_test_macros.hpp>
#define TEST 1

#include "top_tree.h"
#include "underlying_tree.h"
#include "user_data.h"



TEST_CASE("Simple test", "[user data]")
{
    int size = 10;
    int vertex_id = 1;
    TopTree<EmptyData> top_tree = TopTree<EmptyData>(size);
    top_tree.expose(vertex_id);
    REQUIRE((top_tree.get_tree()->get_vertex(vertex_id)->is_exposed()));
    
}