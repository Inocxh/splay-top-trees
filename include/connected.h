#include "top_tree.h"
#include <variant>

using None = std::monostate;

class ConnectedCluster;

class Connected {
    inline static unsigned int next_id = 0;
    TopTree<ConnectedCluster,None,None> *top_tree;
    
    public:
    void link(int, int);
    void cut(int, int);
    bool connected(int, int);

    Connected(int size);
};

class ConnectedCluster : public Node<ConnectedCluster, None, None> {
    public:
    size_t id;

    void create(None* , None* left, None* right);
    void merge(ConnectedCluster* left, ConnectedCluster* right);
    void split(ConnectedCluster* left, ConnectedCluster* right);
};
