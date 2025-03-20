#include "H2DE/H2DE_utils.h"

float H2DE_Lerp(float min, float max, float blend) {
    return min + blend * (max - min);
}

float H2DE_RandomFloatInRange(float min, float max) {
    static std::random_device rd;
    static std::default_random_engine e(rd());
    std::uniform_real_distribution<float> dist(min, max);
    return dist(e);
}

int H2DE_RandomIntegerInRange(int min, int max) {
    static std::random_device rd;
    static std::default_random_engine e(rd());
    std::uniform_int_distribution<int> dist(min, max);
    return dist(e);
}

bool H2DE_RandomBool() {
    static std::random_device rd;
    static std::default_random_engine e(rd());
    std::uniform_int_distribution<int> dist(0, 1);
    return dist(e) == 0;
}
