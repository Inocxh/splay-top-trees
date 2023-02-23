#include <vector>
#include <iostream>
#include "underlying_tree_new.h"

/*
C: Cluster data,
E: Edge data,
V: Vertex data
*/
template<class C, class E, class V> class Node;
template<class C, class E, class V> class LeafNode;
template<class C, class E, class V> class InternalNode;
// Class TNode;



template<class C, class E, class V> class Edge;
template<class C, class E, class V> class Vertex;
template<class C, class E, class V> class Tree;

template<class C, class E, class V> 
class Node {
    InternalNode<C,E,V>* parent;
    int num_internal;

    protected: 
    virtual void merge() = 0;
    virtual void split() = 0;
    bool is_path() { return true; };

    Node() {
        this->num_internal = 1;
        this->parent = nullptr;
        std::cout << "hello from node const" << std::endl;
    };
    
};

// C *must* inherit from Node<C,E,V>
template<class C, class E, class V>
class LeafNode : C {
    Edge<C, E, V>* edge;

    public:
    void rotate_up() {
        this->merge();
        this->is_path();
    };
    LeafNode() : C() {
    };
};

// C *must* inherit from Node<C,E,V>
template<class C, class E, class V>
class InternalNode : C {

};


class UserT {
    int weight;
};
class MyNode : public Node<MyNode, UserT, void> {
    UserT* user_data;
    protected:
    void merge() {
        std::cout <<"hello from merge" << std::endl;
        return;
    };
    void split() {
        return;
    };
    public:
    MyNode() : Node<MyNode,UserT,void>() {
        
    };
};
int main() {
    LeafNode<MyNode,UserT,void>* t = new LeafNode<MyNode,UserT,void>();
    t->rotate_up();
    return 0;
}