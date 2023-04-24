#ifndef TWO_EDGE_CLUSTER
#define TWO_EDGE_CLUSTER
#include <variant>
#include <vector>
#include <bitset>
#include "edge.h"

#include "top_tree.h"

using namespace std;

struct VertexLabel;

class TwoEdgeCluster : public Node<TwoEdgeCluster, TreeEdge, VertexLabel> {
    //Cover level
    inline static int l_max; //floor(log (tree_size))
    int cover_level;
    int cover_plus;
    int cover_minus;
    int global_cover;

    int uncover_val = -1;
    int cover_val = -1;

    TreeEdge* min_path_edge;
    TreeEdge* min_global_edge;

    void merge_cover(TwoEdgeCluster*, TwoEdgeCluster*);
    void create_cover(TreeEdge*, VertexLabel*, VertexLabel*);
    void split_cover(TwoEdgeCluster*, TwoEdgeCluster*);
    void destroy_cover(TreeEdge*, VertexLabel*, VertexLabel*);

    //Find Size
    vector<int> size;
    vector<vector<int>> part_size[2]; // could be binary tree

    void compute_part_size(vector<vector<int>>&, vector<vector<int>>&, vector<vector<int>>&, int);
    void sum_row_range(vector<int>&, vector<vector<int>>&, int, int);
    void delete_row_range(vector<vector<int>>&, int, int);
    void sum_diagonal(vector<int>&, vector<vector<int>>&);

    void merge_find_size(TwoEdgeCluster*, TwoEdgeCluster*);
    void create_find_size(TreeEdge*, VertexLabel*, VertexLabel*);
    void split_find_size(TwoEdgeCluster*, TwoEdgeCluster*);
    
    
    
    // Find First Label
    VertexLabel* vertex[2] = {nullptr,nullptr};
    long int incident;
    vector<long int> part_incident[2];

    VertexLabel* find_vertex_label(TwoEdgeCluster*, int v, int w, int i);
    NonTreeEdge* find_first_label(TwoEdgeCluster*, int, int , int);


    void merge_find_first_label(TwoEdgeCluster*, TwoEdgeCluster*);
    void create_find_first_label(TreeEdge*, VertexLabel*, VertexLabel*);
    void split_find_first_label(TwoEdgeCluster*, TwoEdgeCluster*);



    public:
    TwoEdgeCluster();
    ~TwoEdgeCluster();

    static void set_l_max(int);
    static int get_l_max();
    int get_cover_level();

    void cover(int);
    void uncover(int);

    void assign_vertex(int, VertexLabel*);

    int get_size(int);
    long int get_incident();

    void create(TreeEdge*, VertexLabel*, VertexLabel*);
    void merge(TwoEdgeCluster*, TwoEdgeCluster*);
    void split(TwoEdgeCluster*, TwoEdgeCluster*);
    void destroy(TreeEdge*, VertexLabel*, VertexLabel*);
    void swap_data();


    void print_data() {
        cout << "incident: " << bitset<4>(this->incident) << " ";
    }
    void print_data2() {
        cout << "size: [";
        for (int i = 0; i < l_max; i++) {
            cout << this->size[i] << ", ";
        }
        cout << "] ";
        for (int i = 0; i < 2; i++) {
            cout << " p" << i << ": [";
            for (int j = 0; j < l_max + 2; j++) {
                cout << "[";
                for (int k = 0; k < l_max; k++) {
                    cout << this->part_size[i][j][k] << ",";
                }
                cout << "] ";
            }
            cout << "];";
        }
        cout << " c: " << this->cover_level << " c-: " << this->cover_minus << " c+: " << cover_plus << "   ";
    }
};


struct VertexLabel {
    vector<vector<NonTreeEdge>> labels;
    TwoEdgeCluster* leaf_node = nullptr; 

    VertexLabel() {
        int lmax = TwoEdgeCluster::get_l_max();
        this->labels = std::vector<vector<NonTreeEdge>>(lmax);
        for (int i = 0; i < this->labels.size(); i++) {
            this->labels[i] = std::vector<NonTreeEdge>();
        }
    };

};


#endif