#define TEST 1

#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include "top_tree.h" //Also 'Includes underlying_tree.h'
#include "user_data.h"
//#include "underlying_tree.h"

TEST_CASE( "test", "[tree constructor]") {
    int size = 10;
    Tree tree = Tree(size);
    REQUIRE(tree.num_vertices == size);
    REQUIRE(tree.get_vertices()->size() == size);
}

/*
Path test tests top tree derived from:
    Node     Sib     Uncle
O--------O--------O--------O

"Standard" top tree    "Flipped" top tree
         O                    O          
        / \                  / \           
       O   U                U   O                        
      / \                      / \         
     N   S                    S   N          

*/
TEST_CASE( "Rotation on path", "[rotations]") {
    int size = 4;
    Tree tree = Tree(size);
    Edge* node = tree.add_edge(0, 1);
    Edge* sibling = tree.add_edge(1, 2);
    Edge* uncle = tree.add_edge(3, 2);
    
    LeafNode<EmptyData>* n = new LeafNode<EmptyData>(node, 1, false);
    LeafNode<EmptyData>* s = new LeafNode<EmptyData>(sibling, 2, false);
    LeafNode<EmptyData>* u = new LeafNode<EmptyData>(uncle, 1, true);
    
    InternalNode<EmptyData>* parent = new InternalNode<EmptyData>(1, false);
    parent->set_children(n, s);
    n->set_parent(parent);
    s->set_parent(parent);

    InternalNode<EmptyData>* grandparent = new InternalNode<EmptyData>(0, false);
    grandparent->set_children(parent, u);
    parent->set_parent(grandparent);
    u->set_parent(grandparent);

    
    SECTION("Standard") {
        n->rotate_up();
        
        REQUIRE(n->get_parent() == grandparent);
        REQUIRE(s->get_parent() == parent);
        REQUIRE(u->get_parent() == parent);
        REQUIRE(parent->get_parent() == grandparent);
        
        REQUIRE(grandparent->children[0] == n);
        REQUIRE(grandparent->children[1] == parent);
        REQUIRE(parent->children[0] == s);
        REQUIRE(parent->children[1] == u);
        
        REQUIRE(n->get_endpoint(0) == tree.get_vertex(0));
        REQUIRE(n->get_endpoint(1) == tree.get_vertex(1));
        REQUIRE(s->get_endpoint(0) == tree.get_vertex(1));
        REQUIRE(s->get_endpoint(1) == tree.get_vertex(2));

        REQUIRE((u->is_flipped()));
        REQUIRE(u->get_endpoint(0) == tree.get_vertex(3));
        REQUIRE(u->get_endpoint(1) == tree.get_vertex(2));
    }

    SECTION("Flipped") {
        grandparent->flip();
        n->rotate_up();

        REQUIRE(n->get_parent() == grandparent);
        REQUIRE(s->get_parent() == parent);
        REQUIRE(u->get_parent() == parent);
        REQUIRE(parent->get_parent() == grandparent);
        
        REQUIRE(grandparent->children[0] == parent);
        REQUIRE(grandparent->children[1] == n);
        REQUIRE(parent->children[0] == u);
        REQUIRE(parent->children[1] == s);
        
        REQUIRE(n->is_flipped());
        REQUIRE(n->get_endpoint(0) == tree.get_vertex(0));
        REQUIRE(n->get_endpoint(1) == tree.get_vertex(1));
        
        REQUIRE(s->is_flipped());
        REQUIRE(s->get_endpoint(0) == tree.get_vertex(1));
        REQUIRE(s->get_endpoint(1) == tree.get_vertex(2));

        REQUIRE(u->get_endpoint(0) == tree.get_vertex(3));
        REQUIRE(u->get_endpoint(1) == tree.get_vertex(2));   
    }
}
/*
Star top-tree represents:
           O
           | Node
           |
           O
   Uncle  / \  Sibling
         /   \
        O     O
'Standard' same side      'Flipped' same side
        O                          O            
       / \                        / \            
      O   U                      U   O           
     / \                            / \              
    N   S                          S   N        
        
*/
TEST_CASE( "Rotation on star same side", "[rotations]") {
    int size = 4;
    Tree tree = Tree(size);
    Edge* node = tree.add_edge(0, 1);
    Edge* sibling = tree.add_edge(1, 2);
    Edge* uncle = tree.add_edge(1, 3);
    
    LeafNode<EmptyData>* n = new LeafNode<EmptyData>(node, 1, false);
    LeafNode<EmptyData>* s = new LeafNode<EmptyData>(sibling, 1, false);
    LeafNode<EmptyData>* u = new LeafNode<EmptyData>(uncle, 1, false);
    
    InternalNode<EmptyData>* parent = new InternalNode<EmptyData>(1, false);
    parent->set_children(n, s);
    n->set_parent(parent);
    s->set_parent(parent);

    InternalNode<EmptyData>* grandparent = new InternalNode<EmptyData>(0, false);
    grandparent->set_children(parent, u);
    parent->set_parent(grandparent);
    u->set_parent(grandparent);
    
    SECTION("Standard") {
        n->rotate_up();
        
        REQUIRE(n->get_parent() == grandparent);
        REQUIRE(parent->get_parent() == grandparent);
        REQUIRE(s->get_parent() == parent);
        REQUIRE(u->get_parent() == parent);
        
        
        REQUIRE(grandparent->children[0] == n);
        REQUIRE(grandparent->children[1] == parent);
        REQUIRE(parent->children[0] == s);
        REQUIRE(parent->children[1] == u);
        
        REQUIRE(n->get_endpoint(0) == tree.get_vertex(0));
        REQUIRE(n->get_endpoint(1) == tree.get_vertex(1));

        REQUIRE(s->is_flipped());
        REQUIRE(s->get_endpoint(0) == tree.get_vertex(1));
        REQUIRE(s->get_endpoint(1) == tree.get_vertex(2));

        REQUIRE(u->get_endpoint(0) == tree.get_vertex(1));
        REQUIRE(u->get_endpoint(1) == tree.get_vertex(3));
    }

    SECTION("Flipped") {
        grandparent->flip();
        n->rotate_up();

        REQUIRE(n->get_parent() == grandparent);
        REQUIRE(parent->get_parent() == grandparent);
        REQUIRE(s->get_parent() == parent);
        REQUIRE(u->get_parent() == parent);
        
        REQUIRE(grandparent->children[0] == parent);
        REQUIRE(grandparent->children[1] == n);
        REQUIRE(parent->children[0] == u);
        REQUIRE(parent->children[1] == s);
        
        REQUIRE(n->is_flipped());
        REQUIRE(n->get_endpoint(0) == tree.get_vertex(0));
        REQUIRE(n->get_endpoint(1) == tree.get_vertex(1));

        REQUIRE(!s->is_flipped());
        REQUIRE(s->get_endpoint(0) == tree.get_vertex(1));
        REQUIRE(s->get_endpoint(1) == tree.get_vertex(2));

        REQUIRE(u->is_flipped());
        REQUIRE(u->get_endpoint(0) == tree.get_vertex(1));
        REQUIRE(u->get_endpoint(1) == tree.get_vertex(3));
    }
}
/*
We emulate that siblings endpoint is exposed, making it a path.
Star top-tree represents:
           O
           | Node
           |
           O
   Uncle  / \  Sibling
         /   \
        O     Ø
'Standard' same side      'Flipped' same side
        O                          O            
       / \                        / \            
      O   U                      U   O           
     / \                            / \              
    S   N                          N   S        
*/
TEST_CASE( "Rotation on star diff. side", "[rotations]") {
    int size = 4;
    Tree tree = Tree(size);
    Edge* node = tree.add_edge(0, 1);
    Edge* sibling = tree.add_edge(1, 2);
    Edge* uncle = tree.add_edge(1, 3);
    
    LeafNode<EmptyData>* n = new LeafNode<EmptyData>(node, 1, false);
    LeafNode<EmptyData>* s = new LeafNode<EmptyData>(sibling, 2, false); //Sib endpoint exposed
    LeafNode<EmptyData>* u = new LeafNode<EmptyData>(uncle, 1, true);
    
    InternalNode<EmptyData>* parent = new InternalNode<EmptyData>(1, false);
    parent->set_children(n, s);
    n->set_parent(parent);
    s->set_parent(parent);

    InternalNode<EmptyData>* grandparent = new InternalNode<EmptyData>(0, false);
    grandparent->set_children(u, parent);
    parent->set_parent(grandparent);
    u->set_parent(grandparent);
    
    SECTION("Sibling is path") {
        n->rotate_up();

        REQUIRE(n->get_parent() == grandparent);
        REQUIRE(parent->get_parent() == grandparent);
        REQUIRE(s->get_parent() == parent);
        REQUIRE(u->get_parent() == parent);
        
        REQUIRE(grandparent->is_flipped());
        REQUIRE(grandparent->children[0] == parent);
        REQUIRE(grandparent->children[1] == n);

        REQUIRE(parent->is_flipped());
        REQUIRE(parent->children[0] == u);
        REQUIRE(parent->children[1] == s);
        
        REQUIRE(n->is_flipped());
        REQUIRE(n->get_endpoint(0) == tree.get_vertex(0));
        REQUIRE(n->get_endpoint(1) == tree.get_vertex(1));

        REQUIRE(!s->is_flipped());
        REQUIRE(s->get_endpoint(0) == tree.get_vertex(1));
        REQUIRE(s->get_endpoint(1) == tree.get_vertex(2));

        REQUIRE(u->is_flipped());
        REQUIRE(u->get_endpoint(0) == tree.get_vertex(1));
        REQUIRE(u->get_endpoint(1) == tree.get_vertex(3));
    }
}


// Case 1: b0, b1, b2 = point, point, point
// Case 2: b0, b1, b2 = point, path,  point
// Case 3: b1, b2, b3 = path,  path,  point
// Case 4: b1, b2, b3 = path,  point, point
// Case 5: b1, b2, b3 = path,  path,  path

/*
Underlying tree:
O -1- O -2- O -3- O -4- O
Top tree case 1:
 b3
|  \
b2  4
| \
b1 3
| \
1  2
Top tree case 2:


*/
TEST_CASE("semi_splay_step on path (Case 1 and 3)", "[semi_splay_step]") {
    int size = 5;
    Tree tree = Tree(size);
    Edge* node = tree.add_edge(0, 1);
    Edge* sibling = tree.add_edge(1, 2);
    Edge* uncle = tree.add_edge(2, 3);
    Edge* granduncle = tree.add_edge(3, 4);

    //Top tree setup
    LeafNode<EmptyData>* top_node = new LeafNode<EmptyData>(node, 1, false);
    LeafNode<EmptyData>* top_sibling = new LeafNode<EmptyData>(sibling, 2, false);
    LeafNode<EmptyData>* top_uncle = new LeafNode<EmptyData>(uncle, 2, false);
    LeafNode<EmptyData>* top_granduncle = new LeafNode<EmptyData>(granduncle, 1, false);
    
    InternalNode<EmptyData>* top_parent = new InternalNode<EmptyData>(1, false);
    top_parent->set_children(top_node, top_sibling);
    top_sibling->set_parent(top_parent);
    top_node->set_parent(top_parent);

    InternalNode<EmptyData>* top_grandparent = new InternalNode<EmptyData>(1, false);
    top_grandparent->set_children(top_parent, top_uncle);
    top_parent->set_parent(top_grandparent);
    top_uncle->set_parent(top_grandparent);
    
    InternalNode<EmptyData>* top_ggparent = new InternalNode<EmptyData>(0, false);
    top_ggparent->set_children(top_grandparent, top_granduncle);
    top_grandparent->set_parent(top_ggparent);
    top_granduncle->set_parent(top_ggparent);


    
    InternalNode<EmptyData>* res = (InternalNode<EmptyData>*) (top_node->semi_splay_step());;
    
    REQUIRE(res == top_grandparent);

    REQUIRE(top_sibling->get_parent() == top_parent);
    REQUIRE(top_uncle->get_parent() == top_parent);
    REQUIRE((
        top_parent->children[0] == top_sibling &&
        top_parent->children[1] == top_uncle && 
        !top_parent->is_flipped()
    ));

    REQUIRE(top_node->get_parent() == top_grandparent);
    REQUIRE(top_parent->get_parent() == top_grandparent);
    REQUIRE((
        top_grandparent->children[0] == top_node &&
        top_grandparent->children[1] == top_parent &&
        !top_grandparent->is_flipped()
    ));

    REQUIRE(top_grandparent->get_parent() == top_ggparent);
    REQUIRE(top_granduncle->get_parent() == top_ggparent);
    REQUIRE((
        top_ggparent->children[0] == top_grandparent &&
        top_ggparent->children[1] == top_granduncle &&
        !top_ggparent->is_flipped()
    ));
}



TEST_CASE("Case 3", "[semi_splay_step]") {
    int size = 6;
    Tree tree = Tree(size);
    Edge* gguncle = tree.add_edge(0, 1);
    Edge* node = tree.add_edge(1, 2);
    Edge* sibling = tree.add_edge(2, 3);
    Edge* uncle = tree.add_edge(3, 4);
    Edge* granduncle = tree.add_edge(4, 5);

    //Top tree setup
    LeafNode<EmptyData>* top_node = new LeafNode<EmptyData>(node, 2, false);
    LeafNode<EmptyData>* top_sibling = new LeafNode<EmptyData>(sibling, 2, false); 
    LeafNode<EmptyData>* top_uncle = new LeafNode<EmptyData>(uncle, 2, false);
    LeafNode<EmptyData>* top_granduncle = new LeafNode<EmptyData>(granduncle, 1, false);
    LeafNode<EmptyData>* top_gguncle = new LeafNode<EmptyData>(gguncle, 1, false);
    
    InternalNode<EmptyData>* top_parent = new InternalNode<EmptyData>(2, false);
    top_parent->set_children(top_node, top_sibling);
    top_sibling->set_parent(top_parent);
    top_node->set_parent(top_parent);

    InternalNode<EmptyData>* top_grandparent = new InternalNode<EmptyData>(1, false);
    top_grandparent->set_children(top_parent, top_uncle);
    top_parent->set_parent(top_grandparent);
    top_uncle->set_parent(top_grandparent);
    
    InternalNode<EmptyData>* top_ggparent = new InternalNode<EmptyData>(0, false);
    top_ggparent->set_children(top_grandparent, top_granduncle);
    top_grandparent->set_parent(top_ggparent);
    top_granduncle->set_parent(top_ggparent);

    InternalNode<EmptyData>* top_gggparent = new InternalNode<EmptyData>(0, false);
    top_gggparent->set_children(top_ggparent, top_gguncle);
    top_ggparent->set_parent(top_gggparent);
    top_gguncle->set_parent(top_gggparent);


    InternalNode<EmptyData>* res = (InternalNode<EmptyData>*) (top_node->semi_splay_step());

    REQUIRE(res == top_grandparent);

    REQUIRE(top_sibling->get_parent() == top_parent);
    REQUIRE(top_uncle->get_parent() == top_parent);
    REQUIRE((
        top_parent->children[0] == top_sibling &&
        top_parent->children[1] == top_uncle &&
        !top_parent->is_flipped()
    ));

    REQUIRE(top_node->get_parent() == top_grandparent);
    REQUIRE(top_parent->get_parent() == top_grandparent);
    REQUIRE((
        top_grandparent->children[0] == top_node &&
        top_grandparent->children[1] == top_parent &&
        !top_grandparent->is_flipped()
    ));


    REQUIRE(top_grandparent->get_parent() == top_ggparent);
    REQUIRE(top_granduncle->get_parent() == top_ggparent);
    REQUIRE((
        top_ggparent->children[0] == top_grandparent &&
        top_ggparent->children[1] == top_granduncle &&
        !top_ggparent->is_flipped()
    ));

    REQUIRE(top_ggparent->get_parent() == top_gggparent);
    REQUIRE(top_gguncle->get_parent() == top_gggparent);
    REQUIRE((
        top_gggparent->children[0] == top_ggparent &&
        top_gggparent->children[1] == top_gguncle &&
        !top_gggparent->is_flipped()
    ));
}












TEST_CASE("semi_splay_step on pronged tree (Case 2 and 4)", "[semi_splay_step]") {
    int size = 5;
    Tree tree = Tree(6);
    Edge* node = tree.add_edge(3, 1);
    Edge* sibling = tree.add_edge(0, 1);
    Edge* uncle = tree.add_edge(2, 1);
    Edge* granduncle = tree.add_edge(3, 4);
    
    SECTION("Case 4", "[semi_splay_step]") {
        LeafNode<EmptyData>* top_node = new LeafNode<EmptyData>(node, 1, true);
        LeafNode<EmptyData>* top_sibling = new LeafNode<EmptyData>(sibling, 2, false);
        LeafNode<EmptyData>* top_uncle = new LeafNode<EmptyData>(uncle, 2, false);
        LeafNode<EmptyData>* top_granduncle = new LeafNode<EmptyData>(granduncle, 1, false);
        
        InternalNode<EmptyData>* top_parent = new InternalNode<EmptyData>(1, false);
        top_parent->set_children(top_sibling, top_node);
        top_sibling->set_parent(top_parent);
        top_node->set_parent(top_parent);

        InternalNode<EmptyData>* top_grandparent = new InternalNode<EmptyData>(1, false);
        top_grandparent->set_children(top_uncle, top_parent);
        top_parent->set_parent(top_grandparent);
        top_uncle->set_parent(top_grandparent);
        
        InternalNode<EmptyData>* top_ggparent = new InternalNode<EmptyData>(0, false);
        top_ggparent->set_children(top_grandparent, top_granduncle);
        top_grandparent->set_parent(top_ggparent);
        top_granduncle->set_parent(top_ggparent);

        InternalNode<EmptyData>* res = (InternalNode<EmptyData>*) (top_node->semi_splay_step());

        REQUIRE(res == top_grandparent);

        REQUIRE(top_node->is_flipped());
        REQUIRE(top_sibling->get_parent() == top_parent);
        REQUIRE(top_uncle->get_parent() == top_parent);
        REQUIRE((
            top_parent->children[0] == top_uncle &&
            top_parent->children[1] == top_sibling &&
            !top_parent->is_flipped()
        ));

        REQUIRE(top_node->get_parent() == top_grandparent);
        REQUIRE(top_parent->get_parent() == top_grandparent);
        REQUIRE((
            top_grandparent->children[0] == top_parent &&
            top_grandparent->children[1] == top_node &&
            !top_grandparent->is_flipped()
        ));

        REQUIRE(top_grandparent->get_parent() == top_ggparent);
        REQUIRE(top_granduncle->get_parent() == top_ggparent);
        REQUIRE((
            top_ggparent->children[0] == top_grandparent &&
            top_ggparent->children[1] == top_granduncle &&
            !top_ggparent->is_flipped()
        ));
    }
}

TEST_CASE("semi_splay_step on double pronged tree (Case 5)", "[semi_splay_step]") {
    
}