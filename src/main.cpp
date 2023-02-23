#include <iostream>
#include "underlying_tree_new.h"

using std::cout;
using std::endl; 

template<int k>
class VertexStuff {
    char l[k] = {};
};
class EdgeStuff {

};
class NodeStuff {char l[0];};


int main() {
    Tree t = Tree(10);
    cout << sizeof(Tree<>) << endl;
    cout << sizeof(Vertex<NodeStuff,NodeStuff,NodeStuff>) << endl;
    cout << sizeof(Edge<>) << endl <<endl;

    cout << sizeof(Tree<int,int,int>) << endl;
    cout << sizeof(Vertex<int,int,int>) << endl;
    cout << sizeof(Edge<int,int,int>) << endl << endl;

    cout << sizeof(Vertex<NodeStuff,EdgeStuff,VertexStuff<0>>) << endl;
    cout << sizeof(Vertex<NodeStuff,EdgeStuff,VertexStuff<1>>) << endl;
    cout << sizeof(Vertex<NodeStuff,EdgeStuff,VertexStuff<2>>) << endl;
    cout << sizeof(Vertex<NodeStuff,EdgeStuff,VertexStuff<3>>) << endl;
    cout << sizeof(Vertex<NodeStuff,EdgeStuff,VertexStuff<4>>) << endl;
    cout << sizeof(Vertex<NodeStuff,EdgeStuff,VertexStuff<5>>) << endl;
    cout << sizeof(Vertex<NodeStuff,EdgeStuff,VertexStuff<6>>) << endl;
    cout << sizeof(Vertex<NodeStuff,EdgeStuff,VertexStuff<7>>) << endl;
    cout << sizeof(Vertex<NodeStuff,EdgeStuff,VertexStuff<8>>) << endl;
    cout << sizeof(Vertex<NodeStuff,EdgeStuff,VertexStuff<9>>) << endl;
    cout << sizeof(Vertex<NodeStuff,EdgeStuff,VertexStuff<10>>) << endl;

    
    return 0;
}

