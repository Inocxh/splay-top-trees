#include "two_edge_cluster.h"
#include <catch2/catch_test_macros.hpp>

void cover(TopTree<TwoEdgeCluster,EdgeData,None> T, int v, int w, int i) {
    TwoEdgeCluster *root = T.expose(v,w);
    root->cover(i);
    T.deexpose(v,w);
}

TEST_CASE("Simple find size test", "[find size test]")  {
    TopTree<TwoEdgeCluster,EdgeData,None> T = TopTree<TwoEdgeCluster,EdgeData,None>(10);
    TwoEdgeCluster::set_l_max(3);
    EdgeData* ed[10];
    for (int i = 0; i < 10; i++) {
        ed[i] = new EdgeData();
    }
    
    T.link(1,2, *ed[0]);
    T.link(2,3, *ed[1]);
    T.link(3,4, *ed[2]);
    T.link(4,5, *ed[3]);
    T.link(6,3,EdgeData());
    T.link(6,7,EdgeData());
    T.link(2,8,EdgeData());
    T.link(9,8,EdgeData());

    cover(T,1,5,0);
    TwoEdgeCluster *root = T.expose(1, 5);
    root->print(0,false);
    REQUIRE(root->get_size(0) == 5);

    //t.link(3,2, *ed[1]);
}
TEST_CASE("Small find size", "[find size test]")  {
    TopTree<TwoEdgeCluster,EdgeData,None> T = TopTree<TwoEdgeCluster,EdgeData,None>(8);
    TwoEdgeCluster::set_l_max(4);
    EdgeData* ed[10];
    for (int i = 0; i < 10; i++) {
        ed[i] = new EdgeData();
    }
    
    T.link(1,2, *ed[0]);
    T.link(2,3, *ed[1]);

    cover(T,1,3,0);
    TwoEdgeCluster *root = T.expose(1, 3);
    root->print(0,false);
    REQUIRE(root->get_size(0) == 3);

    //t.link(3,2, *ed[1]);
}


TEST_CASE("Find size test", "[find size test]")  {
    TopTree<TwoEdgeCluster,EdgeData,None> T = TopTree<TwoEdgeCluster,EdgeData,None>(12);
    TwoEdgeCluster::set_l_max(3);
    TwoEdgeCluster *root;

    T.link(0,1, EdgeData(0, 1));
    T.link(1,2, EdgeData(1, 2));
    T.link(2,3, EdgeData(2,3 ));
    T.link(2,4, EdgeData(2,4));
    T.link(4,5, EdgeData(4,5));
    T.link(5,6, EdgeData(5,6));
    T.link(5,7, EdgeData(5,7));
    T.link(4,8, EdgeData(4,8));
    T.link(8,9, EdgeData(8,9));

    cover(T,1,7,0);
    root = T.expose(1, 7);
    REQUIRE(root->get_size(0) == 5);
    T.deexpose(1, 7);
    
    T.cut(5,4);
    T.link(4,6, EdgeData(4,6));
    
    root = T.expose(1, 7);
    REQUIRE(root->get_size(0) == 6);
    T.deexpose(1, 7);
    
    cover(T,3,9,0);
    cover(T,3,8,1);

    root = T.expose(0, 3);
    root->print(0,false);
    REQUIRE(root->get_size(0) == 7);
    REQUIRE(root->get_size(1) == 6);
    T.deexpose(0, 3);

}