#pragma once

#include <H2DE/utils/H2DE_utils.h>

namespace H2DE {
    /**
     * @brief Compute absolute value (constexpr).
     * 
     * @tparam T Numeric type (int, float, double, etc.)
     * @param value Value to get the absolute of.
     * @return Absolute value of `value`.
     */
    template<typename T>
    constexpr T abs(T value) noexcept {
        return (value < 0 ? -value : value);
    }

    /**
     * @brief Compute integer power (constexpr).
     * 
     * @tparam T Numeric type.
     * @param value Base value.
     * @param power Exponent (non-negative integer).
     * @return value raised to the power.
     */
    template<typename T>
    constexpr T pow(T value, unsigned int power) noexcept {
        return (power == 0) ? static_cast<T>(1) : value * H2DE::pow(value, power - 1);
    }

    /**
     * @brief Round to nearest integer (constexpr).
     * 
     * @tparam T Floating numeric type.
     * @param value Value to round.
     * @return Rounded integer.
     */
    template<typename T>
    constexpr int round(T value) noexcept {
        return (value >= 0) ? static_cast<int>(value + 0.5f) : static_cast<int>(value - 0.5f);
    }
    /**
     * @brief Floor (round down) (constexpr).
     * 
     * @tparam T Floating numeric type.
     * @param value Value to floor.
     * @return Largest integer less or equal to value.
     */
    template<typename T>
    constexpr int floor(T value) noexcept {
        return static_cast<int>(value) - (value < static_cast<T>(static_cast<int>(value)) ? 1 : 0);
    }
    /**
     * @brief Ceil (round up) (constexpr).
     * 
     * @tparam T Floating numeric type.
     * @param value Value to ceil.
     * @return Smallest integer greater or equal to value.
     */
    template<typename T>
    constexpr int ceil(T value) noexcept {
        return static_cast<int>(value) + (value > static_cast<T>(static_cast<int>(value)) ? 1 : 0);
    }

    /**
     * @brief Returns the smallest of two values (constexpr).
     * 
     * @tparam T Comparable type.
     * @param a First value.
     * @param b Second value.
     * @return The lesser of a and b.
     */
    template<typename T>
    constexpr T min(T a, T b) noexcept {
        return ((a < b) ? a : b);
    }
    /**
     * @brief Returns the smallest of multiple values (constexpr).
     * 
     * @tparam T Comparable type.
     * @tparam Args More values of the same type.
     * @param first First value.
     * @param rest Remaining values.
     * @return The smallest value.
     */
    template <typename T, typename... Args>
    constexpr T min(T first, Args... rest) noexcept {
        return H2DE::min(first, H2DE::min(rest...));
    }

    /**
     * @brief Returns the largest of two values (constexpr).
     * 
     * @tparam T Comparable type.
     * @param a First value.
     * @param b Second value.
     * @return The greater of a and b.
     */
    template<typename T>
    constexpr T max(T a, T b) noexcept {
        return ((a > b) ? a : b);
    }
    /**
     * @brief Returns the largest of multiple values (constexpr).
     * 
     * @tparam T Comparable type.
     * @tparam Args More values of the same type.
     * @param first First value.
     * @param rest Remaining values.
     * @return The largest value.
     */
    template <typename T, typename... Args>
    constexpr T max(T first, Args... rest) noexcept {
        return H2DE::max(first, H2DE::max(rest...));
    }
    
    /**
     * @brief Clamps a value between a minimum and a maximum (constexpr).
     * 
     * @tparam T Comparable type.
     * @param value Value to clamp.
     * @param min Minimum bound.
     * @param max Maximum bound.
     * @return min if value < min, max if value > max, otherwise value.
     */
    template<typename T>
    constexpr T clamp(T value, T min, T max) noexcept {
        return H2DE::min(H2DE::max(value, min), max);
    }

    /**
     * @brief Linear interpolation with easing.
     * 
     * Calculates a blend between min and max based on blend (0.0 to 1.0),
     * applying an easing function.
     * 
     * @param min Start value.
     * @param max End value.
     * @param blend Interpolation factor from 0.0 (min) to 1.0 (max).
     * @param easing Easing function to apply.
     * @return Interpolated value.
     */
    float lerp(float min, float max, float blend, H2DE_Easing easing = H2DE_EASING_LINEAR);
};
