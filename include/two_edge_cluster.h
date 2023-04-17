#include "top_tree.h"
#include <variant>
#include <vector>

using CoverLevel = int;
using None = std::monostate;

using namespace std;

struct EdgeData {
    int cover_level = -1;
    int endpoints[2];
    EdgeData(int v1, int v2) {
        endpoints[0] = v1;
        endpoints[1] = v2;
    }; 
    EdgeData(){};
};

class TwoEdgeCluster : public Node<TwoEdgeCluster, EdgeData, None> {
    //Cover level
    inline static int l_max; //floor(log (tree_size))
    int cover_level;
    int cover_plus;
    int cover_minus;
    int global_cover;

    EdgeData* min_path_edge;
    EdgeData* min_global_edge;

    void merge_cover(TwoEdgeCluster*, TwoEdgeCluster*);
    void create_cover(EdgeData*, None*, None*);
    void split_cover(TwoEdgeCluster*, TwoEdgeCluster*);
    void split_leaf_cover(EdgeData*, None*, None*);

    //Find size
    vector<int> size;//[]
    vector<vector<int>> part_size[2];//[][] Could be binary trees.
    vector<vector<int>> diag_size[2];//[][]

    void compute_part_size(vector<vector<int>>&, vector<vector<int>>&, vector<vector<int>>&);
    void compute_diag_size(vector<vector<int>>&, vector<vector<int>>&, vector<vector<int>>&);
    void sum_rows_from(vector<int>&, vector<vector<int>>&, int);
    void sum_diag_size(vector<int>&, vector<vector<int>>&, int);

    void merge_find_size(TwoEdgeCluster*, TwoEdgeCluster*);
    void create_find_size(EdgeData*, None*, None*);
    void split_find_size(TwoEdgeCluster*, TwoEdgeCluster*);
    public:

    TwoEdgeCluster();
    ~TwoEdgeCluster();

    static void set_l_max(int);
    static int get_l_max();
    int get_cover_level();

    void cover(int);
    void uncover(int);

    int get_size(int);

    void create(EdgeData*, None*, None*);
    void merge(TwoEdgeCluster*, TwoEdgeCluster*);
    void split(TwoEdgeCluster*, TwoEdgeCluster*);
    void split_leaf(EdgeData*, None*, None*);
    void swap_data();

    void print_data() {
        std::cout << "size: [";
        for (int i = 0; i < l_max; i++) {
            std::cout << this->size[i] << ", ";
        }
        std::cout << "] ";
        for (int i = 0; i < 2; i++) {
            std::cout << "diag " << i << ": [";
            for (int j = 0; j < l_max + 1; j++) {
                std::cout << "[";
                for (int k = 0; k < l_max; k++) {
                    std::cout << this->diag_size[i][j][k] << ", ";
                }
                std::cout << "]";
            }
            std::cout << "];  ";
            
        }
        std::cout << " c: " << this->cover_level << " c-: " << this->cover_minus << " c+: " << cover_plus << "   ";
    }
};