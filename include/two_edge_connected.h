#include "connected.h"

class TwoEdgeConnectivity {
    Connected connected;
    //FindFirstLabel FFL;

    int size();
    void swap();
    void find_replacement();
    void recover();
    void recover_phase();

    public: 
    int two_size();
    void insert();
    void remove(); //delete is keyword.
    void two_edge_connected();
    void find_bridge(int);
    void find_bridge(int, int);
};

