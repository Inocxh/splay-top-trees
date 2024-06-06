#include <catch2/catch_test_macros.hpp>
#include "underlying_tree.h"
#include "top_tree.h"

TEST_CASE("Create tree", "[tree constructor]")
{
    int size = 10;
    Tree<> tree = Tree<>(size);
    REQUIRE(tree.get_size() == size);
    
}

bool vertex_has_edge_incident(Vertex<>* vertex, Edge<>* edge)
{
    Edge<>*current = vertex->get_first_edge();
    while (current != edge)
    {
        if (current == NULL)
        {
            return false;
        }
        int is_right_vertex = current->is_right_vertex(vertex);
        current = current->get_next(is_right_vertex);
    }
    return current == edge;
}

TEST_CASE("Adding edges", "[add_edge]")
{
    int size = 10;
    Tree<> tree = Tree<>(size);

    SECTION("Add 1 edge")
    {
        Edge<>*e = tree.add_edge(1, 2, None());
        REQUIRE(e == tree.get_vertex(1)->get_first_edge());
        REQUIRE(e == tree.get_vertex(2)->get_first_edge());
    }
    SECTION("Add multiple edges")
    {
        Edge<>* e1 = tree.add_edge(1, 2, None());
        REQUIRE(e1 == tree.get_vertex(1)->get_first_edge());
        REQUIRE(e1 == tree.get_vertex(2)->get_first_edge());

        Edge<>* e2 = tree.add_edge(1, 3, None());
        REQUIRE(e2 == tree.get_vertex(1)->get_first_edge());
        REQUIRE(vertex_has_edge_incident(tree.get_vertex(1), e1));
        REQUIRE(e2 == tree.get_vertex(3)->get_first_edge());

        Edge<>* e3 = tree.add_edge(1, 4, None());
        REQUIRE(e3 == tree.get_vertex(1)->get_first_edge());
        REQUIRE(vertex_has_edge_incident(tree.get_vertex(1), e1));
        REQUIRE(vertex_has_edge_incident(tree.get_vertex(1), e2));
        REQUIRE(e3 == tree.get_vertex(4)->get_first_edge());
    }
}

TEST_CASE("Delete edges", "[del_edge]")
{
    int size = 10;
    Tree<> tree = Tree<>(size);
    Edge<>* e1 = tree.add_edge(1, 2,None());
    Edge<>* e2 = tree.add_edge(1, 3,None());
    // Check both exist
    REQUIRE(vertex_has_edge_incident(tree.get_vertex(1), e1));
    REQUIRE(vertex_has_edge_incident(tree.get_vertex(2), e1));
    REQUIRE(vertex_has_edge_incident(tree.get_vertex(1), e2));
    REQUIRE(vertex_has_edge_incident(tree.get_vertex(3), e2));
    tree.del_edge(e1);
    // Check only e2 exist
    REQUIRE_FALSE(vertex_has_edge_incident(tree.get_vertex(1), e1));
    REQUIRE_FALSE(vertex_has_edge_incident(tree.get_vertex(2), e1));
    REQUIRE(vertex_has_edge_incident(tree.get_vertex(1), e2));
    REQUIRE(vertex_has_edge_incident(tree.get_vertex(3), e2));
    tree.del_edge(e2);
    // Check neither e1 or e2 exist
    REQUIRE_FALSE(vertex_has_edge_incident(tree.get_vertex(1), e1));
    REQUIRE_FALSE(vertex_has_edge_incident(tree.get_vertex(1), e2));
}

TEST_CASE("Vertex is right", "[is_right_vertex]")
{
    int size = 10;
    Tree<> tree = Tree<>(size);

    SECTION("Simple")
    {
        Edge<>* e1 = tree.add_edge(1, 2,None());
        REQUIRE_FALSE(e1->is_right_vertex(tree.get_vertex(1)));
        REQUIRE(e1->is_right_vertex(tree.get_vertex(2)));
    }
    SECTION("Simple reversed")
    {
        Edge<>* e1 = tree.add_edge(2, 1,None());
        REQUIRE(e1->is_right_vertex(tree.get_vertex(1)));
        REQUIRE_FALSE(e1->is_right_vertex(tree.get_vertex(2)));
    }
    SECTION("Traverse edges around vertex with is_right_vertex")
    {
        Edge<>* e1 = tree.add_edge(1, 2,None());
        Edge<>* e2 = tree.add_edge(3, 1,None());
        Edge<>* e3 = tree.add_edge(1, 4,None());

        Vertex<>* v1 = tree.get_vertex(1);
        Edge<>* current = v1->get_first_edge();

        // Iterate linked list of vertex 1
        int is_right_vertex = current->is_right_vertex(v1);
        REQUIRE(is_right_vertex == 0);
        current = current->get_next(is_right_vertex);
        REQUIRE(current == e2);
        is_right_vertex = current->is_right_vertex(v1);
        REQUIRE(is_right_vertex == 1);
        current = current->get_next(is_right_vertex);
        REQUIRE(current == e1);
        is_right_vertex = current->is_right_vertex(v1);
        REQUIRE(is_right_vertex == 0);
    }
}

