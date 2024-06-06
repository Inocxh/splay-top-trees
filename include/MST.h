#include <bits/stdc++.h> 
#include <vector>

class UnionFind {
    std::vector<int> parent, rank;
    int n;
    public:
    UnionFind() {};
    UnionFind(int n) {
        this->n = n;
        this->parent = std::vector<int>(n + 1);
        this->rank = std::vector<int>(n + 1);

        for (int i = 0; i < n + 1; i++) {
            this->parent[i] = i;
            this->rank[i] = 0;
        }
    };

    int find(int u) {
        int node = u;

        while (node != parent[node]) {
            node = parent[node];
        }
        return node;
    };

    void merge(int u, int v) {
        int x = this->find(u);
        int y = this->find(v);

        
        if (this->rank[x] <= this->rank[y])
            this->parent[x] = y;
        else
            this->parent[y] = x;

        if (this->rank[x] == this->rank[y])
            this->rank[y]++;
    };
};

typedef std::pair<int, std::pair<int, int>> t_Edge;

class MSTGraph {
    public:
    int n;
    std::vector<t_Edge> edges;

    MSTGraph(int n) {
        this->n = n;
    };

    void add_edge(int u, int v, int weight) {
        edges.push_back({weight, {u, v}});
    };

    int MST_weight() {
        int mst_weight = 0;
        UnionFind uf = UnionFind(this->n);

        std::sort(edges.begin(), edges.end());

        std::vector<t_Edge>::iterator it; 
        for (it = edges.begin(); it != edges.end(); it++) { 
            int u = it->second.first; 
            int v = it->second.second; 
    
            int set_u = uf.find(u); 
            int set_v = uf.find(v); 
    
            if (set_u != set_v) { 

                std::cout << u << " - " << v << ":" << it->first << std::endl; 
                mst_weight += it->first; 

                uf.merge(set_u, set_v); 
            } 
        } 
        return mst_weight;
    };
};


