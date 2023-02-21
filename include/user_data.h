#include <iostream>

class EmptyData {
    public:
    static EmptyData* merge(EmptyData* left, EmptyData* right) {
        std::cout << "hello from merge" << std::endl;
        return new EmptyData();
    };
    static EmptyData* split(EmptyData* left, EmptyData* right) {
        return new EmptyData();
    };
};
class MyData {
    int num;
    public:

    MyData(int n) {
        this->num = n;
        std::cout << "I have num " << this->num << std::endl;
    }
    static MyData* merge(MyData* left, MyData* right) {
        return new MyData(left->num + right->num);
    };
    static MyData* split(MyData* left, MyData* right) {
        return new MyData(0);
    };
};

class WeightData {
    public:
    int weight;

    WeightData(int n) {
        this->weight = n;
    
    }
    static WeightData* merge(WeightData* left, WeightData* right) {
        return new WeightData(std::max(left->weight, right->weight));
    };
    static WeightData* split(WeightData* left, WeightData* right) {
        return new WeightData(0);
    };
};