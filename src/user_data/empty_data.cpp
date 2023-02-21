#include "user_data.h"

class EmptyData {
    static EmptyData* merge(EmptyData* left, EmptyData* right) {
        return new EmptyData();
    };
    static EmptyData* split(EmptyData* left, EmptyData* right) {
        return new EmptyData();
    };
};