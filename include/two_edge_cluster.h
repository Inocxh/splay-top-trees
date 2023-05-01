#ifndef TWO_EDGE_CLUSTER
#define TWO_EDGE_CLUSTER

#include "edge.h"
#include <cassert>
#include <tuple>
#include <iostream>
#include <variant>
#include <vector>
#include <bitset>

#include "top_tree.h"

struct VertexLabel;
struct TwoEdgeConnectivty;
struct TwoEdgeCluster;

struct VertexLabel {
    std::vector<std::vector<NewEdge*>> labels;
    TwoEdgeCluster* leaf_node = nullptr; 

    void print() {
        for (int i = 0; i < labels.size(); i++) {
            if (i != 1) continue;
            for (int j = 0; j < labels[i].size(); j++) {
                std::cout << "(" << labels[i][j]->endpoints[0] << "," << labels[i][j]->endpoints[1] << ") ";
            }
        }
    }
    VertexLabel() {
    };

    VertexLabel(int lmax) {
        this->labels = std::vector<std::vector<NewEdge*>>(lmax);
        for (int i = 0; i < this->labels.size(); i++) {
            this->labels[i] = std::vector<NewEdge*>();
        }
    };

    ~VertexLabel() {
        /*for (int i = 0; i < labels.size(); i++) {
            for (int j = 0; j < labels[i].size(); j++) {
                delete labels[i][j];
            }
        }*/
    };

};

class TwoEdgeCluster : public Node<TwoEdgeCluster,NewEdge,VertexLabel> {
    friend class TwoEdgeConnectivity;

    //Cover level
    inline static int l_max; //floor(log (tree_size))
    int cover_level;
    int cover_plus;
    int cover_minus;
    int global_cover;

    NewEdge* min_path_edge;
    NewEdge* min_global_edge;

    void merge_cover(TwoEdgeCluster*, TwoEdgeCluster*);
    void create_cover(NewEdge*, VertexLabel*, VertexLabel*);
    void split_cover(TwoEdgeCluster*, TwoEdgeCluster*);
    void destroy_cover(NewEdge*, VertexLabel*, VertexLabel*);

    //Find Size
    std::vector<int> size;
    std::vector<std::vector<int>> part_size[2]; // could be binary tree

    void compute_part_size(std::vector<std::vector<int>>&, std::vector<std::vector<int>>&, std::vector<std::vector<int>>&, int);
    void sum_row_range(std::vector<int>&, std::vector<std::vector<int>>&, int, int);
    void delete_row_range(std::vector<std::vector<int>>&, int, int);
    void sum_diagonal(std::vector<int>&, std::vector<std::vector<int>>&);

    void merge_find_size(TwoEdgeCluster*, TwoEdgeCluster*);
    void create_find_size(NewEdge*, VertexLabel*, VertexLabel*);
    void split_find_size(TwoEdgeCluster*, TwoEdgeCluster*);
    
    
    
    // Find First Label
    VertexLabel* vertex[2] = {nullptr,nullptr};
    int boundary_vertices_id[2] = {-1,-1};
    long int incident;
    std::vector<long int> part_incident[2];

    //VertexLabel* find_vertex_label(TwoEdgeCluster*, int v, int w, int i);


    void merge_find_first_label(TwoEdgeCluster*, TwoEdgeCluster*);
    void create_find_first_label(NewEdge*, VertexLabel*, VertexLabel*);
    void split_find_first_label(TwoEdgeCluster*, TwoEdgeCluster*);



    public:
    std::tuple<TwoEdgeCluster*,VertexLabel*> find_first_label(int, int , int);
    TwoEdgeCluster();
    ~TwoEdgeCluster() {
    };

    static void set_l_max(int);
    static int get_l_max();
    int get_cover_level();

    void cover(int);
    void uncover(int);

    void assign_vertex(int, VertexLabel*);

    int get_size(int);
    long int get_incident();

    void create(NewEdge*, VertexLabel*, VertexLabel*);
    void merge(TwoEdgeCluster*, TwoEdgeCluster*);
    void split(TwoEdgeCluster*, TwoEdgeCluster*);
    void destroy(NewEdge*, VertexLabel*, VertexLabel*);
    void swap_data();


    void print_data() {
    std::cout << "inci: " << std::bitset<4>(this->incident) << " "; 
    std::cout << "bounds: (" << this->boundary_vertices_id[0] << "," << this->boundary_vertices_id[1] << ") ";
    std::cout << " c: " << this->cover_level; //<< " c-: " << this->cover_minus << " c+: " << this->cover_plus << " "; 
    //std::cout << "s: " << this->size[1] << " ";
    if (this->vertex[0]) {
        std::cout << "vl: ";
        this->vertex[0]->print();
    }
    if (this->vertex[1]) {
        std::cout << "vr: ";
        this->vertex[1]->print();
    }
    if (!this->get_child(0)) {
        auto this_leaf = (LeafNode<TwoEdgeCluster, NewEdge, VertexLabel>*) this;
        std::cout << "cl: " << this_leaf->edge->get_data()->level;
    }
    };
};




#endif