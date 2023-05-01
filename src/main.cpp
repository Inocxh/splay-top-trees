#include <vector>
#include <iostream>
#include <bitset>

void clear_from(long int* bitvec, int pos) {
    unsigned long int mask = ~0;
    *bitvec &= ~(mask << pos);
}

int main() {
    long int incident = 13;
    clear_from(&incident, 2);
    std::cout << incident << std::endl;

    std::vector<long int> source = std::vector<long int>(7);
    int lmax_idx = 5 + 1;
    source[0] = 0;
    source[1] = 0; 
    source[2] = 15;
    source[3] = 5;
    source[4] = 0;
    source[5] = 0;
    source[6] = 0;

    // Start from 1, i.e. row 0, as row -1 is 0s. 
    for (int i = 1; i < lmax_idx + 1; i++) {
        long int tmp = source[i];
        clear_from(&tmp, i);
        incident |= tmp;
        //std::cout << std::bitset<6>(tmp) << std::endl;
    }
    std::cout << std::bitset<10>(incident);
    return 0;
}


