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

TEST_CASE("Constructor test", "[user data]")
{
    int size = 10;
    int vertex_id = 1;
    TopTree<EmptyData> top_tree = TopTree<EmptyData>(size);
    NodeBase* e1 = top_tree.link(0, 1, new EmptyData());
    NodeBase* e2 = top_tree.link(1, 2, new EmptyData());
}
TEST_CASE("MyData test", "[user data]")
{
    int size = 10;
    int vertex_id = 1;
    TopTree<MyData> top_tree = TopTree<MyData>(size);
    NodeBase* e1 = top_tree.link(0, 1, new MyData(5));
    NodeBase* e2 = top_tree.link(1, 2, new MyData(10));
}
TEST_CASE("Max-edge-weight test (small)", "[user data]") {
    int size = 10;
    int vertex_id = 1;
    TopTree<WeightData> top_tree = TopTree<WeightData>(size);
    NodeBase* e1 = top_tree.link(0, 1, new WeightData(5));
    NodeBase* e2 = top_tree.link(1, 2, new WeightData(10));
    top_tree.expose(0);
    NodeBase* root = top_tree.expose(1);
    int lol = 1;
    WeightData* data = (dynamic_cast<TContainer<WeightData>*>(root))->user_data;
    REQUIRE(data->weight == 10);
}