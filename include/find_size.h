#include "cover_level.h"

class FindSizeCluster : public CoverLevelCluster {
    int* size = nullptr;//[]
    int* part_size[2] = {nullptr,nullptr};//[][] Could be binary trees.
    int* diag_size[2] = {nullptr,nullptr};//[][]

    void copy_row(int*, int*, int);
    void compute_part_size(int*, int*, int*);
    void compute_diag_size(int*, int*, int*);
    void sum_rows_from(int*, int*, int);
    void sum_diag_size(int*, int*, int);


    public:

    void create(EdgeData* edge_data, None* left, None* right);
    void merge(FindSizeCluster*, FindSizeCluster*);
    void split(FindSizeCluster*, FindSizeCluster*);

    FindSizeCluster();
    ~FindSizeCluster();
};