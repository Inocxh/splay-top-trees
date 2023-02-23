#include <catch2/catch_test_macros.hpp>
#include "underlying_tree_new.h"

TEST_CASE("Create tree", "[tree constructor]")
{
    int size = 10;
    Tree<void,void,void> tree = Tree<void,void,void>(size);
    REQUIRE(tree.get_size() == size);
    REQUIRE(tree.get_vertices()->size() == size);
}

bool vertex_has_edge_incident(Vertex<void,void,void>* vertex, Edge<void,void,void>* edge)
{
    Edge<void,void,void>*current = vertex->get_first_edge();
    while (current != edge)
    {
        if (current == NULL)
        {
            return false;
        }
        int vertex_is_right = current->vertex_is_right(vertex);
        current = current->next[vertex_is_right];
    }
    return current == edge;
}

TEST_CASE("Adding edges", "[add_edge]")
{
    int size = 10;
    Tree<void,void,void> tree = Tree<void,void,void>(size);

    SECTION("Add 1 edge")
    {
        Edge<void,void,void>*e = tree.add_edge(1, 2);
        REQUIRE(e == tree.get_vertex(1)->get_first_edge());
        REQUIRE(e == tree.get_vertex(2)->get_first_edge());
    }
    SECTION("Add multiple edges")
    {
        Edge<void,void,void>* e1 = tree.add_edge(1, 2);
        REQUIRE(e1 == tree.get_vertex(1)->get_first_edge());
        REQUIRE(e1 == tree.get_vertex(2)->get_first_edge());

        Edge<void,void,void>* e2 = tree.add_edge(1, 3);
        REQUIRE(e2 == tree.get_vertex(1)->get_first_edge());
        REQUIRE(vertex_has_edge_incident(tree.get_vertex(1), e1));
        REQUIRE(e2 == tree.get_vertex(3)->get_first_edge());

        Edge<void,void,void>* e3 = tree.add_edge(1, 4);
        REQUIRE(e3 == tree.get_vertex(1)->get_first_edge());
        REQUIRE(vertex_has_edge_incident(tree.get_vertex(1), e1));
        REQUIRE(vertex_has_edge_incident(tree.get_vertex(1), e2));
        REQUIRE(e3 == tree.get_vertex(4)->get_first_edge());
    }
}

TEST_CASE("Delete edges", "[del_edge]")
{
    int size = 10;
    Tree<void,void,void> tree = Tree<void,void,void>(size);
    Edge<void,void,void>* e1 = tree.add_edge(1, 2);
    Edge<void,void,void>* e2 = tree.add_edge(1, 3);
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
    Tree<void,void,void> tree = Tree<void,void,void>(size);

    SECTION("Simple")
    {
        Edge<void,void,void>* e1 = tree.add_edge(1, 2);
        REQUIRE_FALSE(e1->vertex_is_right(tree.get_vertex(1)));
        REQUIRE(e1->vertex_is_right(tree.get_vertex(2)));
    }
    SECTION("Simple reversed")
    {
        Edge<void,void,void>* e1 = tree.add_edge(2, 1);
        REQUIRE(e1->vertex_is_right(tree.get_vertex(1)));
        REQUIRE_FALSE(e1->vertex_is_right(tree.get_vertex(2)));
    }
    SECTION("Traverse edges around vertex with vertex_is_right")
    {
        Edge<void,void,void>* e1 = tree.add_edge(1, 2);
        Edge<void,void,void>* e2 = tree.add_edge(3, 1);
        Edge<void,void,void>* e3 = tree.add_edge(1, 4);

        Vertex *v1 = tree.get_vertex(1);
        Edge<void,void,void>* current = v1->get_first_edge();

        // Iterate linked list of vertex 1
        int vertex_is_right = current->vertex_is_right(v1);
        REQUIRE(vertex_is_right == 0);
        current = current->next[vertex_is_right];
        REQUIRE(current == e2);
        vertex_is_right = current->vertex_is_right(v1);
        REQUIRE(vertex_is_right == 1);
        current = current->next[vertex_is_right];
        REQUIRE(current == e1);
        vertex_is_right = current->vertex_is_right(v1);
        REQUIRE(vertex_is_right == 0);
    }
}

// TODO : implement test for next and prev. Maybe also supports deleting middle of linked list edge.
