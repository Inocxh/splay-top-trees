#include <iostream>
#include <tuple>

class EmptyData {
    public:
    static EmptyData* merge(EmptyData* left, EmptyData* right) {
        std::cout << "hello from merge" << std::endl;
        return new EmptyData();
    };
    static std::tuple<EmptyData*, EmptyData*> split(EmptyData* data) {
        return std::tuple(new EmptyData(), new EmptyData());
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
    static std::tuple<MyData*, MyData*> split(MyData* data) {
        return std::tuple(new MyData(0), new MyData(0));
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
    static std::tuple<WeightData*, WeightData*> split(WeightData* data) {
        return std::tuple(new WeightData(0), new WeightData(0));
    };
};