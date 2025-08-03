#pragma once

#include <random>
#include <H2DE/utils/H2DE_utils.h>

namespace H2DE {
    /**
     * @brief Generate random integer in range [min, max].
     * 
     * @param min Minimum inclusive.
     * @param max Maximum inclusive.
     * @return Random integer between min and max.
     */
    int randomIntegerInRange(int min, int max);
    /**
     * @brief Generate random float in range [min, max].
     * 
     * @param min Minimum inclusive.
     * @param max Maximum inclusive.
     * @return Random float between min and max.
     */
    float randomFloatInRange(float min, float max);
    /**
     * @brief Generate a random float in range [0.0f, 1.0f].
     * 
     * @return A random float between 0.0f and 1.0f.
     */
    inline float randomFloat() {
        return H2DE::randomFloatInRange(0.0f, 1.0f);
    }
    /**
     * @brief Generate a random boolean.
     * 
     * @return true or false randomly.
     */
    bool randomBool();
    /**
     * @brief Get a random valid index from a vector.
     * 
     * @tparam T Type of elements in the vector.
     * @param vector The vector to get a random index from.
     * @return A random index between 0 and vector.size() - 1.
     */
    template<typename T>
    inline int randomIndexFromVector(const std::vector<T>& vector) {
        return H2DE::randomIntegerInRange(0, vector.size() - 1);
    }
    /**
     * @brief Get a random value from a vector.
     * 
     * @tparam T Type of elements in the vector.
     * @param vector The vector to get a random value from.
     * @return A randomly chosen element from the vector.
     * @warning The vector must not be empty.
     */
    template<typename T>
    inline T randomValueFromVector(const std::vector<T>& vector) {
        return vector.at(H2DE::randomIndexFromVector(vector));
    }
};
