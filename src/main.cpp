#include <vector>
#include <iostream>
#include <bitset>
#include <vector>
using std::vector;

void sum_row_range(vector<int>& target_row, vector<vector<int>>& source, int start, int end) { 
    for (int i = start; i < end; i++) {
        for (int j = 0; j < 6; j++) {
            target_row[j] += source[i][j];
        }
    }
}

int main() {
    vector<int> target = vector<int>(5);
    vector<vector<int>> rows;
    rows.push_back(vector<int>(5,1));
    rows.push_back(vector<int>(5,2));
    sum_row_range(target,rows,0,2);
    for (auto i : target) {
        std::cout << i << " ";
    }


    return 0;
}
