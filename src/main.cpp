#include <iostream>
#include "underlying_tree.h"

int main() {
    Tree t = Tree(10);
    t.add_edge(5, 6);
    t.add_edge(6, 7);
    t.add_edge(7, 8);
    //t.del_edge(e);

    cout << t.get_vertex(6)->has_at_most_one_incident_edge();
    cout << t.get_vertex(5)->has_at_most_one_incident_edge();
    cout << t.get_vertex(4)->has_at_most_one_incident_edge();
    
    t.print_tree();
}

