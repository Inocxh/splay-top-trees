#ifndef TWO_EDGE_CLUSTER
#define TWO_EDGE_CLUSTER

#include "edge.h"
#include <cassert>
#include <tuple>
#include <iostream>
#include <variant>
#include <vector>
#include <memory>
#include <bitset>

#include "top_tree.h"

struct VertexLabel;
struct TwoEdgeConnectivty;
struct TwoEdgeCluster;

struct VertexLabel {
    std::vector<std::vector<std::shared_ptr<EdgeData>>> labels;
    TwoEdgeCluster* leaf_node = nullptr; 

    void print() {
        for (int i = 0; i < labels.size(); i++) {
            //if (i == 0) continue;
            for (int j = 0; j < labels[i].size(); j++) {
                std::cout << "(" << labels[i][j]->endpoints[0] << "," << labels[i][j]->endpoints[1] << "; " <<  labels[i][j]->level << ")";
            }
        }
    }
    VertexLabel() {
    };

    VertexLabel(int lmax) {
        this->labels = std::vector<std::vector<std::shared_ptr<EdgeData>>>(lmax);
        for (int i = 0; i < this->labels.size(); i++) {
            this->labels[i] = std::vector<std::shared_ptr<EdgeData>>();
        }
    };


};

class TwoEdgeCluster : public Node<TwoEdgeCluster,TreeEdgeData,None> {
    friend class TwoEdgeConnectivity;

    int last_uncover = -1;

    //Cover level
    inline static int l_max; //floor(log (tree_size))
    int cover_level = -1;
    int cover_plus = -1;
    int cover_minus = -1;
    int global_cover = -1;

    TreeEdgeData* min_path_edge = nullptr;
    TreeEdgeData* min_global_edge = nullptr;

    void cover_level_cover(int);
    void cover_level_uncover(int);
    

    void merge_cover(TwoEdgeCluster*, TwoEdgeCluster*);
    void create_cover(TreeEdgeData*, None*, None*);
    void split_cover(TwoEdgeCluster*, TwoEdgeCluster*);
    void destroy_cover(TreeEdgeData*, None*, None*);

    //Find Size
    std::vector<int> size;
    std::vector<std::vector<int>> part_size[2]; // could be binary tree

    void find_size_cover(int);
    void find_size_uncover(int);

    void compute_part_size(std::vector<std::vector<int>>&, std::vector<std::vector<int>>&, std::vector<std::vector<int>>&, int);
    void sum_row_range(std::vector<int>&, std::vector<std::vector<int>>&, int, int);
    void delete_row_range(std::vector<std::vector<int>>&, int, int);
    void sum_diagonal(std::vector<int>&, std::vector<std::vector<int>>&);

    void merge_find_size(TwoEdgeCluster*, TwoEdgeCluster*);
    void create_find_size(TreeEdgeData*, None*, None*);
    void split_find_size(TwoEdgeCluster*, TwoEdgeCluster*);
    void destroy_find_size(TreeEdgeData*, None*, None*);
    
    
    
    // Find First Label
    VertexLabel* vertex[2] = {nullptr,nullptr};
    int boundary_vertices_id[2] = {-1,-1};
    long int incident;
    std::vector<long int> part_incident[2];


    void find_first_label_cover(int);
    void find_first_label_uncover(int);

    void merge_find_first_label(TwoEdgeCluster*, TwoEdgeCluster*);
    void create_find_first_label(TreeEdgeData*, None*, None*);
    void split_find_first_label(TwoEdgeCluster*, TwoEdgeCluster*);
    void destroy_find_first_label(TreeEdgeData*,  None*, None*);



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

    void create(TreeEdgeData*, None*, None*);
    void merge(TwoEdgeCluster*, TwoEdgeCluster*);
    void split(TwoEdgeCluster*, TwoEdgeCluster*);
    void destroy(TreeEdgeData*, None*, None*);
    void swap_data();


    void print_data() {
        std::cout << "inci: " << std::bitset<4>(this->incident) << " "; 
        std::cout << "bounds: (" << this->boundary_vertices_id[0] << "," << this->boundary_vertices_id[1] << ") ";
        std::cout << " c:" << this->cover_level << " c-:" << this->cover_minus << " c+:" << this->cover_plus << " "; 
        //std::cout << " f: " << this->is_flipped() << " ";
        std::cout << " [";
        for (int i = 0; i <this->size.size(); i++) {
            std::cout << this->size[i] << ",";
        }
        std::cout << "] ";
        // for (int i = 0 ; i < 2; i++) {
        //     std::cout << "p" << i << ": ["; 
        //     for (int j = 0; j < this->part_size[i].size(); j++) {
        //         std::cout << "[";
        //         for(int k = 0; k < this->part_size[i][j].size(); k++) {
        //             std::cout << this->part_size[i][j][k] << ",";
        //         }
        //         std::cout << "],";
        //     }
        //     std::cout << "];";
        // }

        std::cout << "s: " << this->size[1] << " ";
        if (this->vertex[0]) {
            std::cout << "vl: ";
            this->vertex[0]->print();
        }
        if (this->vertex[1]) {
            std::cout << "vr: ";
            this->vertex[1]->print();
        }
    };
};




#endif