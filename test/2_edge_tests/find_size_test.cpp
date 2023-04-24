#include "two_edge_cluster.h"
#include <catch2/catch_test_macros.hpp>


//TODO: Change treeedge() arguments

void cover(TopTree<TwoEdgeCluster,TreeEdge,VertexLabel> T, int v, int w, int i) {
    TwoEdgeCluster *root = T.expose(v,w);
    root->cover(i);
    root->print(0, false);
    std::cout << std::endl;
    T.deexpose(v,w);
}
void uncover(TopTree<TwoEdgeCluster,TreeEdge,VertexLabel> T, int v, int w, int i) {
    TwoEdgeCluster *root = T.expose(v,w);
    root->uncover(i);
    root->print(0, false);
    std::cout << std::endl;
    T.deexpose(v,w);
}

TEST_CASE("Simple find size test", "[find size test]")  {
    TopTree<TwoEdgeCluster,TreeEdge,VertexLabel> T = TopTree<TwoEdgeCluster,TreeEdge,VertexLabel>(10);
    TwoEdgeCluster::set_l_max(3);

    T.link(1,2,TreeEdge());
    T.link(2,3,TreeEdge());
    T.link(3,4,TreeEdge());
    T.link(4,5,TreeEdge());
    T.link(6,3,TreeEdge());
    T.link(6,7,TreeEdge());
    T.link(2,8,TreeEdge());
    T.link(9,8,TreeEdge())->print(0, false);
    std::cout << "\n";
    cover(T,1,5,0);
    TwoEdgeCluster *root = T.expose(1, 5);
    root->print(0,false);
    REQUIRE(root->get_size(0) == 5);

}
TEST_CASE("Small find size", "[find size test]")  {
    TopTree<TwoEdgeCluster,TreeEdge,VertexLabel> T = TopTree<TwoEdgeCluster,TreeEdge,VertexLabel>(8);
    TwoEdgeCluster::set_l_max(4);
    TreeEdge* ed[10];
    for (int i = 0; i < 10; i++) {
        ed[i] = new TreeEdge();
    }
    
    T.link(1,2, *ed[0]);
    T.link(2,3, *ed[1]);

    cover(T,1,3,0);
    TwoEdgeCluster *root = T.expose(1, 3);
    root->print(0,false);
    REQUIRE(root->get_size(0) == 3);

}


TEST_CASE("Find size test", "[find size test]")  {
    TopTree<TwoEdgeCluster,TreeEdge,VertexLabel> T = TopTree<TwoEdgeCluster,TreeEdge,VertexLabel>(12);
    TwoEdgeCluster::set_l_max(3);
    TwoEdgeCluster *root;

    T.link(0,1, TreeEdge(0,1));
    T.link(1,2, TreeEdge(1,2));
    T.link(2,3, TreeEdge(2,3));
    T.link(2,4, TreeEdge(2,4));
    T.link(4,5, TreeEdge(4,5));
    T.link(5,6, TreeEdge(5,6));
    T.link(5,7, TreeEdge(5,7));
    T.link(4,8, TreeEdge(4,8));
    T.link(8,9, TreeEdge(8,9));

    cover(T,1,7,0);
    root = T.expose(1, 7);
    REQUIRE(root->get_size(0) == 5);
    T.deexpose(1, 7);
    
    T.cut(5,4);
    T.link(4,6, TreeEdge(4,6));
    
    root = T.expose(1, 7);
    REQUIRE(root->get_size(0) == 6);
    T.deexpose(1, 7);
    
    cover(T,3,9,0);

    root = T.expose(0, 3);
    root->print(0,false);
    REQUIRE(root->get_size(0) == 7);
    T.deexpose(0, 3);

}


TEST_CASE("FS: Massive", "[find size test]")  {
    TopTree<TwoEdgeCluster,TreeEdge,VertexLabel> T = TopTree<TwoEdgeCluster,TreeEdge,VertexLabel>(20);
    TwoEdgeCluster::set_l_max(4);
    TwoEdgeCluster *root;

    T.link(0,1, TreeEdge(0,1));
    T.link(1,2, TreeEdge(1,2));
    T.link(2,3, TreeEdge(2,3));
    T.link(1,4, TreeEdge(1,4)); 

    cover(T,2,3,1);

    T.link(5,6, TreeEdge(0,1));
    T.link(6,7, TreeEdge(1,2));
    T.link(7,8, TreeEdge(2,3));
    T.link(6,9, TreeEdge(1,4)); 
    T.link(9,10, TreeEdge(2,3));
    T.link(11,9, TreeEdge(1,4)); 
    cover(T,5,11,0);
    cover(T,10,8,1);

    root = T.expose(5,8);
    REQUIRE(root->get_size(0) == 7);
    REQUIRE(root->get_size(1) == 6);
    REQUIRE(root->get_size(2) == 4);
    T.deexpose(5, 8);

    T.link(4,10, TreeEdge(2,3));
    T.link(8,12, TreeEdge(1,4)); 
    cover(T,10,6,2);
    cover(T,10,1,3);

    root = T.expose(5,8);
    REQUIRE(root->get_size(0) == 9);
    REQUIRE(root->get_size(1) == 8);
    REQUIRE(root->get_size(2) == 8);
    T.deexpose(5, 8);   
}



TEST_CASE("FS: Uncover", "[find size test]")  {
    TopTree<TwoEdgeCluster,TreeEdge,VertexLabel> T = TopTree<TwoEdgeCluster,TreeEdge,VertexLabel>(20);
    TwoEdgeCluster::set_l_max(4);
    TwoEdgeCluster *root;

    T.link(0,1, TreeEdge(0,1));
    T.link(1,2, TreeEdge(1,2));
    T.link(2,3, TreeEdge(2,3));
    T.link(1,4, TreeEdge(1,4)); 

    cover(T,0,3,0);
    cover(T,1,2,2);
    
    T.link(4,5, TreeEdge(1,4)); 

    cover(T,5,2,1);

    root = T.expose(0,4);
    REQUIRE(root->get_size(0) == 6);
    REQUIRE(root->get_size(1) == 5);
    REQUIRE(root->get_size(2) == 4);
    T.deexpose(0,4);
    

    uncover(T, 4, 3, 0);
    root = T.expose(0,4);
    REQUIRE(root->get_size(0) == 5);
    REQUIRE(root->get_size(1) == 5);
    REQUIRE(root->get_size(2) == 4);
    T.deexpose(0,4);

    uncover(T, 4, 0, 1);
    root = T.expose(1,5);
    REQUIRE(root->get_size(0) == 4);
    REQUIRE(root->get_size(1) == 4);
    REQUIRE(root->get_size(2) == 4);
    REQUIRE(root->get_size(3) == 3);
    T.deexpose(1,5);


    cover(T, 0, 3, 1);
    uncover(T, 0, 2, 1);
    cover(T, 0, 5, 3);
    uncover(T, 4, 2, 2);

    root = T.expose(4,1);
    REQUIRE(root->get_size(0) == 4);
    REQUIRE(root->get_size(3) == 4);
    T.deexpose(4,1);

}

TEST_CASE("FS: Uncover massive", "[find size test]")  {
    TopTree<TwoEdgeCluster,TreeEdge,VertexLabel> T = TopTree<TwoEdgeCluster,TreeEdge,VertexLabel>(20);
    TwoEdgeCluster::set_l_max(4);
    TwoEdgeCluster *root;

    T.link(0,1, TreeEdge(0,1));
    T.link(1,2, TreeEdge(1,2));
    T.link(2,3, TreeEdge(2,3));
    T.link(3,4, TreeEdge(2,3));
    T.link(4,5, TreeEdge(2,3));
    T.link(5,6, TreeEdge(2,3));
    T.link(6,7, TreeEdge(2,3));
    
    cover(T, 2, 5, 1);
    
    T.link(3,8, TreeEdge(2,3));
    T.link(8,9, TreeEdge(2,3));
    T.link(8,10, TreeEdge(2,3));
    
    cover(T, 9, 1, 2);
    
    T.link(5,11, TreeEdge(2,3));
    T.link(11,12, TreeEdge(1,4));
    T.link(12,13, TreeEdge(2,3));
    T.link(13,14, TreeEdge(1,4));
    T.link(11,15, TreeEdge(1,4));

    cover(T,14,4,0);
    cover(T,15,7,1);
    cover(T,5,11,2);
    
    T.link(4,16, TreeEdge(1,4));    

    cover(T,16,8,3);
        
    root = T.expose(1,4);
    REQUIRE(root->get_size(0) == 15);
    REQUIRE(root->get_size(1) == 12);
    REQUIRE(root->get_size(2) == 7);
    REQUIRE(root->get_size(3) == 6);
    T.deexpose(1,4);

    root = T.expose(16,6);
    REQUIRE(root->get_size(0) == 15);
    REQUIRE(root->get_size(1) == 12);
    REQUIRE(root->get_size(2) == 10);
    REQUIRE(root->get_size(3) == 6);
    T.deexpose(16,6);

    uncover(T,8,5,1);
    uncover(T,9,0,2);
    cover(T, 2, 10, 0);
    uncover(T,12,13,3);
    uncover(T,16,15,1);
    uncover(T,16,12,2);
    cover(T, 12, 6, 0);

    root = T.expose(1,4);
    REQUIRE(root->get_size(0) == 7);
    REQUIRE(root->get_size(1) == 6);
    REQUIRE(root->get_size(2) == 6);
    T.deexpose(1,4);


    /*T.expose(4,7);
    T.deexpose(4,7);
    T.expose(10,2);
    T.deexpose(10,2);
    T.expose(9,3);
    T.deexpose(9,3);
    T.expose(11,7);
    T.deexpose(11,7);
    T.expose(16,14);
    T.deexpose(16,14);
    T.expose(0,10);
    T.deexpose(0,10);
    T.expose(6,5);
    T.deexpose(6,5);
    T.expose(11,5);
    T.deexpose(11,5);*/

    root = T.expose(16,6);
    root->print(0,false);
    REQUIRE(root->get_size(0) == 11); 
    REQUIRE(root->get_size(1) == 7); 
    REQUIRE(root->get_size(2) == 6); 
    REQUIRE(root->get_size(3) == 6); 
    T.deexpose(16,6);
}

TEST_CASE("Small", "[find size test]") {

    TopTree<TwoEdgeCluster,TreeEdge,VertexLabel> T = TopTree<TwoEdgeCluster,TreeEdge,VertexLabel>(7);
    TwoEdgeCluster::set_l_max(4);
    TwoEdgeCluster *root;

    T.link(0,6, TreeEdge(0,1));
    T.link(0,1, TreeEdge(0,1));
    T.link(1,2, TreeEdge(1,2));
    T.link(2,3, TreeEdge(2,3));
    T.link(3,4, TreeEdge(2,3));
    T.link(4,5, TreeEdge(2,3));
    
    cover(T, 1, 5, 0);
    cover(T, 2, 5, 1);
    cover(T, 3, 5, 2);
    cover(T, 4, 5, 3);

    cover(T, 0, 6, 3);

    uncover(T, 0, 5, 0);
    uncover(T, 0, 5, 1);
    uncover(T, 0, 5, 2);

    T.expose(0,6);
    T.deexpose(0,6);
    T.expose(3,4);
    T.deexpose(3,4);
    T.expose(0,4)->print(0, false);
    T.deexpose(0,4);
}


TEST_CASE("FS: minified massive", "[find size test]")  {
    TopTree<TwoEdgeCluster,TreeEdge,VertexLabel> T = TopTree<TwoEdgeCluster,TreeEdge,VertexLabel>(8);
    TwoEdgeCluster::set_l_max(2);
    TwoEdgeCluster *root;

    T.link(0,1, TreeEdge(0,1));
    T.link(1,2, TreeEdge(1,2));
    T.link(2,3, TreeEdge(2,3));
    T.link(1,4, TreeEdge(1,4)); 

    cover(T,2,3,1);
    cover(T,0,1,0);

    T.link(2,5, TreeEdge(2,3));
    T.link(5,6, TreeEdge(1,4)); 
    T.link(5,7, TreeEdge(2,3));

    cover(T,2,7,0);
    cover(T,2,5,1);

    root = T.expose(4,2);
    REQUIRE(root->get_size(0) == 7);
    REQUIRE(root->get_size(1) == 5);
    T.deexpose(4, 2); 
}

TEST_CASE("FS: cut", "[find size test]")  {
    TopTree<TwoEdgeCluster,TreeEdge,VertexLabel> T = TopTree<TwoEdgeCluster,TreeEdge,VertexLabel>(20);
    TwoEdgeCluster::set_l_max(4);
    TwoEdgeCluster *root;

    T.link(0,1, TreeEdge(0,1));
    T.link(1,2, TreeEdge(1,2));
    T.link(2,3, TreeEdge(2,3));
    T.link(3,4, TreeEdge(3,4)); 

    T.link(1,5, TreeEdge(1,2));
    T.link(5,6, TreeEdge(1,2));
    T.link(6,7, TreeEdge(1,2));
    
    T.link(5,8, TreeEdge(1,2));
    T.link(8,9, TreeEdge(1,2));
    T.link(9,10, TreeEdge(1,2));

    T.link(8,11, TreeEdge(1,2));
    T.link(11,12, TreeEdge(1,2));

    cover(T,0,10,0);
    cover(T,12,7,1);

    root = T.expose(0,2);
    REQUIRE(root->get_size(0) == 11);
    REQUIRE(root->get_size(1) == 3);
    T.deexpose(0, 2); 

    T.cut(5,8);

    root = T.expose(8,9);
    REQUIRE(root->get_size(0) == 5);
    REQUIRE(root->get_size(1) == 4);
    T.deexpose(8,9);

    root = T.expose(0,2);
    REQUIRE(root->get_size(0) == 6);
    REQUIRE(root->get_size(1) == 3);
    T.deexpose(0, 2); 


    cover(T,6,3,0);

    T.cut(5,1);
    T.link(7,4, TreeEdge(1,2));
    T.link(10,7, TreeEdge(1,2));

    cover(T,7,4,2);
    cover(T,7,3,1);
    cover(T,7,10,1);
    uncover(T,7,9,0);

    root = T.expose(6,7);
    REQUIRE(root->get_size(0) == 9);
    REQUIRE(root->get_size(1) == 6);
    T.deexpose(6, 7); 


    T.cut(6,7);
    T.cut(1,2);
    root = T.expose(4,7);
    REQUIRE(root->get_size(0) == 5);
    REQUIRE(root->get_size(1) == 4);
    T.deexpose(4, 7); 


    cover(T,12,2,0);

    T.link(6,9, TreeEdge(1,2));

    cover(T,9,5, 2);
    uncover(T,2,4,1);
    cover(T,3,4,2);

    root = T.expose(7,8);
    REQUIRE(root->get_size(0) == 10);
    REQUIRE(root->get_size(1) == 10);
    REQUIRE(root->get_size(2) == 8);
    T.deexpose(7,8); 
    

    T.cut(9,10);
    T.link(6,7, TreeEdge(1,2));
    T.link(1,6, TreeEdge(1,2));
    T.cut(8,9);
    T.cut(7,4);


    root = T.expose(8,11);
    REQUIRE(root->get_size(0) == 3);
    REQUIRE(root->get_size(1) == 3);
    REQUIRE(root->get_size(2) == 2);
    T.deexpose(11,8); 

    root = T.expose(1,6);
    REQUIRE(root->get_size(0) == 5);
    REQUIRE(root->get_size(1) == 4);
    REQUIRE(root->get_size(2) == 4);
    REQUIRE(root->get_size(3) == 2);
    T.deexpose(1,6); 

    root = T.expose(2,3);
    REQUIRE(root->get_size(0) == 3);
    REQUIRE(root->get_size(1) == 3);
    REQUIRE(root->get_size(2) == 3);
    REQUIRE(root->get_size(3) == 2);
    T.deexpose(2,3); 

}

