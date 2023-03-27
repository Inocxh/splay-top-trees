#include "top_tree.h"
#include <variant>

using CoverLevel = int;
using None = std::monostate;

struct EdgeData {
    int cover_level;
    int endpoints[2];
};

class CoverLevelCluster : public Node<CoverLevelCluster, EdgeData, None> {
    static int l_max; //floor(log (tree_size))
    int cover;
    int cover_plus;
    int cover_minus;
    int global_cover;

    EdgeData* min_path_edge;
    EdgeData* min_global_edge;

    public:
    static void set_l_max(int l);
    void create(EdgeData* edge_data, None* left, None* right);
    void merge(CoverLevelCluster* left, CoverLevelCluster* right);
};
