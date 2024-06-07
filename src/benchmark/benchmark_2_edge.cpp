#include <two_edge_connected.h>

#include <stdio.h>
#include <fstream>
#include <bits/stdc++.h>
#include <string>
#include <cassert>
#include <chrono>
#include <map> 

struct TwoEdgeQuery {
    char op;
    int u;
    int v;
};

int main(int argc, char *argv[]) {
    int warmups = std::atol(argv[1]);
    int iterations = std::atol(argv[2]);
    std::ifstream data_file(argv[3]);


    if (!data_file) {
        std::cerr << "Datafile not found!";
        exit(1);
    }

    std::string problem;
    int n;
    int m;
    data_file >> problem >> n >> m;
    std::vector<TwoEdgeQuery> queries;
    for (int i = 0; i < m; i++) {
        char query;
        int u;
        int v = 0;
        int weight;
        data_file >> query;
        if (query == 'd') {
            data_file >> u;
        } else {
            data_file >> u >> v;

        }
        queries.push_back(TwoEdgeQuery{query, u, v});
    }

    std::vector<double> warmup_times;
    //Warmup
    for (int i = 0; i < warmups; i++) {
        auto start = std::chrono::high_resolution_clock::now();
        int total_cons = 0;

        TwoEdgeConnectivity top_tree = TwoEdgeConnectivity(n);
        std::vector<std::shared_ptr<EdgeData>> edges; 
        
        for (TwoEdgeQuery q : queries) {
            if (q.op == 'i') {
                edges.push_back(top_tree.insert(q.u,q.v));
                top_tree.insert(q.u, q.v);
            } else if (q.op == 'd') {
                std::swap(edges[q.u],edges.back());
                auto edge = edges.back();
                edges.pop_back();
                top_tree.remove(edge);
            } else if (q.op == 'q') { 
                if (top_tree.two_edge_connected(q.u,q.v))
                    total_cons += 1;          
            }
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::nano> time_taken = end - start;
        warmup_times.push_back(time_taken.count());
    }
    std::vector<double> times;

    for (int i = 0; i < iterations; i++) {
    // Run measurement
        auto start = std::chrono::high_resolution_clock::now();
        int total_cons = 0;
        
        TwoEdgeConnectivity top_tree = TwoEdgeConnectivity(n);
        std::vector<std::shared_ptr<EdgeData>> edges; 
        
        for (TwoEdgeQuery q : queries) {
            if (q.op == 'i') {
                edges.push_back(top_tree.insert(q.u,q.v));
                top_tree.insert(q.u, q.v);
            } else if (q.op == 'd') {
                std::swap(edges[q.u],edges.back());
                auto edge = edges.back();
                edges.pop_back();
                top_tree.remove(edge);
            } else if (q.op == 'q') { 
                if (top_tree.two_edge_connected(q.u,q.v))
                    total_cons += 1;          
            }
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::nano> time_taken = end - start;
        times.push_back(time_taken.count());
    }
    std::vector<double> median = times;
	std::sort(median.begin(), median.end());

	std::cout << "{ \"num_vertices\":" << n << ",\"num_edges\":" << queries.size() << ",\"name\":\"splay top tree 2edge con\",\"median\":" << median[median.size() / 2] << ",\"warmup_times\":[";
	std::cout << std::accumulate(std::next(warmup_times.begin()), warmup_times.end(), std::to_string(warmup_times[0]), [](std::string a, double b) {
		return a + ',' + std::to_string(b);
	});
	std::cout << "], \"times\":[";
	std::cout << std::accumulate(std::next(times.begin()), times.end(), std::to_string(times[0]), [](std::string a, double b) {
		return a + ',' + std::to_string(b);
	});
	std::cout << "]}\n";
    return 0;
}

