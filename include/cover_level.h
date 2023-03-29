#include "top_tree.h"
#include <variant>

using CoverLevel = int;
using None = std::monostate;

struct EdgeData {
    int cover_level;
    int endpoints[2];
};

class CoverLevelCluster : public Node<CoverLevelCluster, EdgeData, None> {
    inline static int l_max; //floor(log (tree_size))
    int cover;
    int cover_plus;
    int cover_minus;
    int global_cover;

    EdgeData* min_path_edge;
    EdgeData* min_global_edge;

    protected:
    int get_cover_level();

    public:
    static void set_l_max(int l);
    static int get_l_max();
    virtual void create(EdgeData* edge_data, None* left, None* right);
    virtual void merge(CoverLevelCluster* left, CoverLevelCluster* right);
    virtual void split(CoverLevelCluster* left, CoverLevelCluster* right);
};
