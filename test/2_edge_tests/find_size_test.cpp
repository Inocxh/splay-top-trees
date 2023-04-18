#include "two_edge_cluster.h"
#include <catch2/catch_test_macros.hpp>

void cover(TopTree<TwoEdgeCluster,EdgeData,None> T, int v, int w, int i) {
    TwoEdgeCluster *root = T.expose(v,w);
    root->cover(i);
    root->print(0, false);
    std::cout << std::endl;
    T.deexpose(v,w);
}
void uncover(TopTree<TwoEdgeCluster,EdgeData,None> T, int v, int w, int i) {
    TwoEdgeCluster *root = T.expose(v,w);
    root->uncover(i);
    root->print(0, false);
    std::cout << std::endl;
    T.deexpose(v,w);
}

TEST_CASE("Simple find size test", "[find size test]")  {
    TopTree<TwoEdgeCluster,EdgeData,None> T = TopTree<TwoEdgeCluster,EdgeData,None>(10);
    TwoEdgeCluster::set_l_max(3);

    T.link(1,2,EdgeData());
    T.link(2,3,EdgeData());
    T.link(3,4,EdgeData());
    T.link(4,5,EdgeData());
    T.link(6,3,EdgeData());
    T.link(6,7,EdgeData());
    T.link(2,8,EdgeData());
    T.link(9,8,EdgeData())->print(0, false);
    std::cout << "\n";
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

    T.link(0,1, EdgeData(0,1));
    T.link(1,2, EdgeData(1,2));
    T.link(2,3, EdgeData(2,3));
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
    //cover(T,3,8,1);

    root = T.expose(0, 3);
    root->print(0,false);
    REQUIRE(root->get_size(0) == 7);
    T.deexpose(0, 3);

}


TEST_CASE("FS: Massive", "[find size test]")  {
    TopTree<TwoEdgeCluster,EdgeData,None> T = TopTree<TwoEdgeCluster,EdgeData,None>(20);
    TwoEdgeCluster::set_l_max(4);
    TwoEdgeCluster *root;

    T.link(0,1, EdgeData(0,1));
    T.link(1,2, EdgeData(1,2));
    T.link(2,3, EdgeData(2,3));
    T.link(1,4, EdgeData(1,4)); 

    cover(T,2,3,1);

    T.link(5,6, EdgeData(0,1));
    T.link(6,7, EdgeData(1,2));
    T.link(7,8, EdgeData(2,3));
    T.link(6,9, EdgeData(1,4)); 
    T.link(9,10, EdgeData(2,3));
    T.link(11,9, EdgeData(1,4)); 
    cover(T,5,11,0);
    cover(T,10,8,1);

    root = T.expose(5,8);
    REQUIRE(root->get_size(0) == 7);
    REQUIRE(root->get_size(1) == 6);
    REQUIRE(root->get_size(2) == 4);
    T.deexpose(5, 8);

    T.link(4,10, EdgeData(2,3));
    T.link(8,12, EdgeData(1,4)); 
    cover(T,10,6,2);
    cover(T,10,1,3);

    root = T.expose(5,8);
    REQUIRE(root->get_size(0) == 9);
    REQUIRE(root->get_size(1) == 8);
    REQUIRE(root->get_size(2) == 8);
    T.deexpose(5, 8);   
}



TEST_CASE("FS: Uncover", "[find size test]")  {
    TopTree<TwoEdgeCluster,EdgeData,None> T = TopTree<TwoEdgeCluster,EdgeData,None>(20);
    TwoEdgeCluster::set_l_max(4);
    TwoEdgeCluster *root;

    T.link(0,1, EdgeData(0,1));
    T.link(1,2, EdgeData(1,2));
    T.link(2,3, EdgeData(2,3));
    T.link(1,4, EdgeData(1,4)); 

    cover(T,0,3,0);
    cover(T,1,2,2);
    
    T.link(4,5, EdgeData(1,4)); 

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
    TopTree<TwoEdgeCluster,EdgeData,None> T = TopTree<TwoEdgeCluster,EdgeData,None>(20);
    TwoEdgeCluster::set_l_max(4);
    TwoEdgeCluster *root;

    T.link(0,1, EdgeData(0,1));
    T.link(1,2, EdgeData(1,2));
    T.link(2,3, EdgeData(2,3));
    T.link(3,4, EdgeData(2,3));
    T.link(4,5, EdgeData(2,3));
    T.link(5,6, EdgeData(2,3));
    T.link(6,7, EdgeData(2,3));
    
    cover(T, 2, 5, 1);
    
    T.link(3,8, EdgeData(2,3));
    T.link(8,9, EdgeData(2,3));
    T.link(8,10, EdgeData(2,3));
    
    cover(T, 9, 1, 2);
    
    T.link(5,11, EdgeData(2,3));
    T.link(11,12, EdgeData(1,4));
    T.link(12,13, EdgeData(2,3));
    T.link(13,14, EdgeData(1,4));
    T.link(11,15, EdgeData(1,4));

    cover(T,14,4,0);
    cover(T,15,7,1);
    cover(T,5,11,2);
    
    T.link(4,16, EdgeData(1,4));    

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

    root = T.expose(16,6);
    REQUIRE(root->get_size(0) == 11);
    REQUIRE(root->get_size(1) == 7); // TODO; it finds one less here
    REQUIRE(root->get_size(2) == 6); // TODO; also here
    REQUIRE(root->get_size(3) == 6); // TODO; and here.
    T.deexpose(16,6);




}


























TEST_CASE("FS: minified massive", "[find size test]")  {
    TopTree<TwoEdgeCluster,EdgeData,None> T = TopTree<TwoEdgeCluster,EdgeData,None>(8);
    TwoEdgeCluster::set_l_max(2);
    TwoEdgeCluster *root;

    T.link(0,1, EdgeData(0,1));
    T.link(1,2, EdgeData(1,2));
    T.link(2,3, EdgeData(2,3));
    T.link(1,4, EdgeData(1,4)); 

    cover(T,2,3,1);
    cover(T,0,1,0);

    T.link(2,5, EdgeData(2,3));
    T.link(5,6, EdgeData(1,4)); 
    T.link(5,7, EdgeData(2,3));

    cover(T,2,7,0);
    cover(T,2,5,1);

    root = T.expose(4,2);
    REQUIRE(root->get_size(0) == 7);
    REQUIRE(root->get_size(1) == 5);
    T.deexpose(4, 2); 
}

TEST_CASE("FS: miniminified massive", "[find size test]")  {
    TopTree<TwoEdgeCluster,EdgeData,None> T = TopTree<TwoEdgeCluster,EdgeData,None>(8);
    TwoEdgeCluster::set_l_max(2);
    TwoEdgeCluster *root;

    T.link(0,1, EdgeData(0,1));
    T.link(1,2, EdgeData(1,2));
    T.link(2,3, EdgeData(2,3));
    T.link(1,4, EdgeData(1,4)); 

   
    cover(T,0,3,0);
    cover(T,1,2,1);

    // PURGE
    T.expose(1,4);
    T.deexpose(1,4); 
    T.expose(2,3);
    T.deexpose(2,3);
    T.expose(0,1);
    T.deexpose(0,1);
    T.expose(2,4);
    T.deexpose(2,4);   

    // PURGE DONE


    root = T.expose(1,4);
    root->print(0, false);
    REQUIRE(root->get_size(0) == 5);
    REQUIRE(root->get_size(1) == 3);
    T.deexpose(4, 1); 
}

