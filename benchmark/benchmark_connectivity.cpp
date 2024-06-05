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

    int warmups = std::atol(argv[1]);
    int iterations = std::atol(argv[2]);
    std::ifstream data_file(argv[3]);



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


    // Warmup
    for (int i = 0; i < warmups; i++) {
        int total_cons = 0;
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

    std::vector<double> times;

    // Run measurement
    for (int i = 0; i < iterations; i++) {
        auto start = std::chrono::high_resolution_clock::now();
        int total_cons = 0;
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
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::nano> time_taken = end - start;
        times.push_back(time_taken.count());
    }
    std::sort(times.begin(), times.end());
    std::cout << "{ \"num_vertices\":" << n << ",\"num_edges\":" << m << ",\"name\":\"" << argv[3] << "\",\"time_ns\":" << times[times.size() / 2] << "}\n";
    return 0;
}

