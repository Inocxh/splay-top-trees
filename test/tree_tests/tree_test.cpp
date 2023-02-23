#include <catch2/catch_test_macros.hpp>
#include "underlying_tree_new.h"

TEST_CASE("Create tree", "[tree constructor]")
{
    int size = 10;
    Tree<> tree = Tree<>(size);
    //REQUIRE(tree.get_size() == size);
    REQUIRE(tree.get_vertices()->size() == size);
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
        int vertex_is_right = current->vertex_is_right(vertex);
        current = current->get_next(vertex_is_right);
    }
    return current == edge;
}

TEST_CASE("Adding edges", "[add_edge]")
{
    int size = 10;
    Tree<> tree = Tree<>(size);

    SECTION("Add 1 edge")
    {
        Edge<>*e = tree.add_edge(1, 2);
        REQUIRE(e == tree.get_vertex(1)->get_first_edge());
        REQUIRE(e == tree.get_vertex(2)->get_first_edge());
    }
    SECTION("Add multiple edges")
    {
        Edge<>* e1 = tree.add_edge(1, 2);
        REQUIRE(e1 == tree.get_vertex(1)->get_first_edge());
        REQUIRE(e1 == tree.get_vertex(2)->get_first_edge());

        Edge<>* e2 = tree.add_edge(1, 3);
        REQUIRE(e2 == tree.get_vertex(1)->get_first_edge());
        REQUIRE(vertex_has_edge_incident(tree.get_vertex(1), e1));
        REQUIRE(e2 == tree.get_vertex(3)->get_first_edge());

        Edge<>* e3 = tree.add_edge(1, 4);
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
    Edge<>* e1 = tree.add_edge(1, 2);
    Edge<>* e2 = tree.add_edge(1, 3);
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

TEST_CASE("Vertex is right", "[vertex_is_right]")
{
    int size = 10;
    Tree<> tree = Tree<>(size);

    SECTION("Simple")
    {
        Edge<>* e1 = tree.add_edge(1, 2);
        REQUIRE_FALSE(e1->vertex_is_right(tree.get_vertex(1)));
        REQUIRE(e1->vertex_is_right(tree.get_vertex(2)));
    }
    SECTION("Simple reversed")
    {
        Edge<>* e1 = tree.add_edge(2, 1);
        REQUIRE(e1->vertex_is_right(tree.get_vertex(1)));
        REQUIRE_FALSE(e1->vertex_is_right(tree.get_vertex(2)));
    }
    SECTION("Traverse edges around vertex with vertex_is_right")
    {
        Edge<>* e1 = tree.add_edge(1, 2);
        Edge<>* e2 = tree.add_edge(3, 1);
        Edge<>* e3 = tree.add_edge(1, 4);

        Vertex<>* v1 = tree.get_vertex(1);
        Edge<>* current = v1->get_first_edge();

        // Iterate linked list of vertex 1
        int vertex_is_right = current->vertex_is_right(v1);
        REQUIRE(vertex_is_right == 0);
        current = current->get_next(vertex_is_right);
        REQUIRE(current == e2);
        vertex_is_right = current->vertex_is_right(v1);
        REQUIRE(vertex_is_right == 1);
        current = current->get_next(vertex_is_right);
        REQUIRE(current == e1);
        vertex_is_right = current->vertex_is_right(v1);
        REQUIRE(vertex_is_right == 0);
    }
}

// TODO : implement test for next and prev. Maybe also supports deleting middle of linked list edge.
