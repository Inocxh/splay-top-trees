#include <min_spanning_forest.h>
#include <MST.h>

#include <stdio.h>
#include <fstream>
#include <cassert>
#include <chrono>
#include <string>

struct MSF_Query {
    int u;
    int v;
    int weight;
};

int main(int argc, char *argv[]) {

    int warmups = std::atol(argv[1]);
    int iterations = std::atol(argv[2]);
    std::ifstream data_file(argv[3]);

    if (!data_file) {
        std::cerr << "Datafile not found!";
        exit(1);
    }

    std::string name;
    //std::getline(data_file, name);

    std::string problem;
    int n;
    int m;
    data_file >> problem >> n >> m;

    std::vector<MSF_Query> queries;

    for (int i = 0; i < m; i++) {
        char query;
        int u;
        int v;
        int weight;
        data_file >> query >> u >> v >> weight;
        assert(query == 'e');
        queries.push_back(MSF_Query{u,v,weight});
    }

    std::vector<double> warmup_times;
    // Warmup
    for (int i = 0; i < warmups; i++) {
        //Run measurement
        auto start = std::chrono::high_resolution_clock::now();
        MinSpanForest tree(n);
        for (MSF_Query q : queries) {
            tree.insert(q.u,q.v,q.weight);
        }    
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::micro> time_taken = end - start;
        warmup_times.push_back(time_taken.count());
    }

    std::vector<double> times;
    // Run measurement
    for (int i = 0; i < iterations; i++) {
        auto start = std::chrono::high_resolution_clock::now();
        MinSpanForest tree(n);
        for (MSF_Query q : queries) {
            tree.insert(q.u,q.v,q.weight);
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::nano> time_taken = end - start;
        times.push_back(time_taken.count());
    }

    std::vector<double> median = times;
	std::sort(median.begin(), median.end());
	std::cout << "{ \"num_vertices\":" << n << ",\"num_edges\":" << queries.size() << ",\"name\":\"Splay top tree\",\"median\":" << median[median.size() / 2] << ",\"warmup_times\":[";
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

