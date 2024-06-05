#include <min_spanning_forest.h>
#include <top_tree.h>
#include "add_weight_cluster.hpp"

#include <stdio.h>
#include <fstream>
#include <cassert>
#include <chrono>


struct Connectivity_Query {
    char op;
    int u;
    int v;
    
};

int main(int argc, char *argv[]) {
    int repeats = std::atol(argv[1]);

    //Parse text-file as data
    std::ifstream data_file(argv[2]);

    std::string name;

    //std::getline(data_file, name);


    std::string problem;
    int n;
    int m;
    data_file >> problem >> n >> m;
    std::vector<Connectivity_Query> queries;

    for (int i = 0; i < m; i++) {
        char query;
        int u;
        int v;
        int weight;
        data_file >> query >> u >> v;
        queries.push_back(Connectivity_Query{query, u, v});
        
        
    }

    //Start clock
    auto start = std::chrono::high_resolution_clock::now();
    int total_cons = 0;
    for (int i = 0; i < repeats; i++) {

        //Run measurement
        TopTree<AddWeightCluster, int, None> top_tree = TopTree<AddWeightCluster, int, None>(n);
        
        for (Connectivity_Query q : queries) {
            if (q.op == 'i') {
                top_tree.link(q.u, q.v, 0);
            } else if (q.op == 'd') {
                top_tree.cut(q.u, q.v);
            } else if (q.op == 'p') { 
                if (top_tree.connected(q.u, q.v))   {
                    total_cons += 1;
                }
                
            }
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> time_taken = end - start;

    //Return elapsed time
    std::cout << total_cons / repeats << std::endl;

    std::cout << time_taken.count() / repeats << "ms \n";
    return 0;
}

