#include "connected.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Connected Test", "[connected]")  {
    Connected t = Connected(10);

    t.link(1,2);
    t.link(3,4);
    REQUIRE(!t.connected(2,3));
    REQUIRE(t.connected(1,2));
    REQUIRE(t.connected(3,4));
    t.link(2,3);
    REQUIRE(t.connected(2,3));
    REQUIRE(t.connected(1,2));
    REQUIRE(t.connected(3,4));
    
    t.cut(2,3);
    REQUIRE(!t.connected(2,3));
    REQUIRE(t.connected(1,2));
    REQUIRE(t.connected(3,4));
    
    t.link(5,6);
    t.link(4,5);
    t.link(5,7);
    REQUIRE(t.connected(3,5));
    REQUIRE(t.connected(3,7));
    t.cut(4,5);
    REQUIRE(!t.connected(3,5));
    REQUIRE(!t.connected(3,7));
    REQUIRE(t.connected(5,7));
}

TEST_CASE("Vertex connected", "[connected]")  {
    Connected t = Connected(10);

    t.link(1,2);
    t.link(2,3);
    REQUIRE(t.connected(1,2));
    REQUIRE(t.connected(2,3));
    REQUIRE(t.connected(1,3));
    REQUIRE(!t.connected(0,1));
    REQUIRE(!t.connected(0,2));
    REQUIRE(!t.connected(0,3));
    REQUIRE(!t.connected(0,4));
}

TEST_CASE("Single edge link and cut", "[connected]")  {
    Connected t = Connected(10);


    REQUIRE(!t.connected(0,1));
    t.link(0,1);
    REQUIRE(t.connected(0,1));
    t.cut(0,1);
    REQUIRE(!t.connected(0,1));
    t.link(0,1);
    REQUIRE(t.connected(0,1));

    
    
    
}