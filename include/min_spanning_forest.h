#include "edge.h"

#include <vector>
#include <cmath>

#include "top_tree.h"
#include <climits>

struct MinSpanForestCluster : Node<MinSpanForestCluster, int, None> {

    int max_weight;
    Edge<MinSpanForestCluster, int, None>* max_edge;

    void create(int* weight, None* left, None* right) {
        this->max_weight = this->is_path() ? *weight : INT_MIN;
        this->max_edge = this->is_path() ? this->get_edge() : nullptr;
    };    
    
    void merge(MinSpanForestCluster* left, MinSpanForestCluster* right) {
        this->max_weight = INT_MIN;
        this->max_edge = nullptr;
        if (left->is_path()) {
            if (left->max_weight > this->max_weight) {
                this->max_weight = left->max_weight;
                this->max_edge = left->max_edge;
            }
        }
        if (right->is_path()) {
            if (right->max_weight > this->max_weight) {
                this->max_weight = right->max_weight;
                this->max_edge = right->max_edge;
            }
        }
    };

    void print_data() {
        std::cout << "num_b: "  << this->get_num_boundary_vertices() << " ";
    }
};


class MinSpanForest {

    public:
    int weight = 0;
    TopTree<MinSpanForestCluster, int, None> top_tree;

    void insert(int u, int v, int weight) {
        if (!this->top_tree.connected(u, v)) {
            this->top_tree.link(u, v, weight);
            this->weight += weight;
            return;
        }
            
        MinSpanForestCluster* root = this->top_tree.expose(u, v);
        if (root->max_weight < weight) {
            this->top_tree.deexpose(u, v);
            return;
        }
        
        Edge<MinSpanForestCluster,int,None>* max_edge = root->max_edge;
        int max_weight = root->max_weight;
        this->top_tree.deexpose(u, v);
        this->top_tree.cut_ptr(max_edge);
        this->weight -= max_weight;
        
        this->top_tree.link(u, v, weight);
        this->weight += weight;
    };

    
    MinSpanForest(int num_vertices) {
        this->top_tree = TopTree<MinSpanForestCluster, int, None>(num_vertices);
    };
    ~MinSpanForest() {};
};


