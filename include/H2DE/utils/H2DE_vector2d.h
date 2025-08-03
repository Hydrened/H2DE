#pragma once

#include <H2DE/utils/H2DE_utils.h>
template<typename H2DE_Rect_T>
struct H2DE_Rect;

/**
 * @brief A generic 2D vector class template.
 * 
 * Provides basic 2D vector operations (addition, subtraction, scalar multiplication/division),
 * comparison operators, rotation around a pivot point, and utility functions.
 * 
 * @tparam H2DE_Vector2D_T The type of the vector components (e.g., int, float).
 */
template<typename H2DE_Vector2D_T>
struct H2DE_Vector2D {
    H2DE_Vector2D_T x = 0.0f;   /** The x component of the vector. */
    H2DE_Vector2D_T y = 0.0f;   /** The y component of the vector. */

    /**
     * @brief Default constructor initializes vector to (0,0).
     */
    constexpr H2DE_Vector2D() noexcept = default;
    /**
     * @brief Construct a vector with given x and y values.
     * @param x The x component.
     * @param y The y component.
     */
    constexpr H2DE_Vector2D(H2DE_Vector2D_T x, H2DE_Vector2D_T y) noexcept : x(x), y(y) {}

    /**
     * @brief Vector addition.
     * @param other The other vector to add.
     * @return The result of vector addition.
     */
    constexpr H2DE_Vector2D<H2DE_Vector2D_T> operator+(const H2DE_Vector2D<H2DE_Vector2D_T>& other) const noexcept {
        return H2DE_Vector2D<H2DE_Vector2D_T>{ x + other.x, y + other.y };
    }
    /**
     * @brief Vector subtraction.
     * @param other The other vector to subtract.
     * @return The result of vector subtraction.
     */
    constexpr H2DE_Vector2D<H2DE_Vector2D_T> operator-(const H2DE_Vector2D<H2DE_Vector2D_T>& other) const noexcept {
        return H2DE_Vector2D<H2DE_Vector2D_T>{ x - other.x, y - other.y };
    }
    /**
     * @brief Scalar multiplication.
     * @param multiplier The scalar multiplier.
     * @return The scaled vector.
     */
    constexpr H2DE_Vector2D<H2DE_Vector2D_T> operator*(float multiplier) const noexcept {
        return H2DE_Vector2D{ static_cast<H2DE_Vector2D_T>(x * multiplier), static_cast<H2DE_Vector2D_T>(y * multiplier) };
    }
    /**
     * @brief Scalar division.
     * @param divider The scalar divisor.
     * @return The scaled vector.
     */
    constexpr H2DE_Vector2D<H2DE_Vector2D_T> operator/(float divider) const {
        return H2DE_Vector2D{ static_cast<H2DE_Vector2D_T>(x / divider), static_cast<H2DE_Vector2D_T>(y / divider) };
    }

    /**
     * @brief Compound addition assignment.
     * @param other The other vector to add.
     * @return Reference to this vector after addition.
     */
    H2DE_Vector2D<H2DE_Vector2D_T>& operator+=(const H2DE_Vector2D<H2DE_Vector2D_T>& other) noexcept;
    /**
     * @brief Compound subtraction assignment.
     * @param other The other vector to subtract.
     * @return Reference to this vector after subtraction.
     */
    H2DE_Vector2D<H2DE_Vector2D_T>& operator-=(const H2DE_Vector2D<H2DE_Vector2D_T>& other) noexcept;
    /**
     * @brief Compound scalar multiplication assignment.
     * @param multiplier The scalar multiplier.
     * @return Reference to this vector after scaling.
     */
    H2DE_Vector2D<H2DE_Vector2D_T>& operator*=(float multiplier) noexcept;
    /**
     * @brief Compound scalar division assignment.
     * @param divider The scalar divisor.
     * @return Reference to this vector after scaling.
     */
    H2DE_Vector2D<H2DE_Vector2D_T>& operator/=(float divider);

    /**
     * @brief Equality operator.
     * @param other The other vector to compare.
     * @return True if both x and y components are equal.
     */
    constexpr bool operator==(const H2DE_Vector2D<H2DE_Vector2D_T>& other) const noexcept {
        return (x == other.x && y == other.y);
    }
    /**
     * @brief Inequality operator.
     * @param other The other vector to compare.
     * @return True if any component differs.
     */
    constexpr bool operator!=(const H2DE_Vector2D<H2DE_Vector2D_T>& other) const noexcept {
        return !(*this == other);
    }
    /**
     * @brief Greater-than operator based on sum of absolute components.
     * @param other The other vector to compare.
     * @return True if this vector's component sum is greater.
     */
    constexpr bool operator>(const H2DE_Vector2D<H2DE_Vector2D_T>& other) const noexcept {
        return (H2DE::abs<H2DE_Vector2D_T>(x) + H2DE::abs<H2DE_Vector2D_T>(y) > H2DE::abs<H2DE_Vector2D_T>(other.x) + H2DE::abs<H2DE_Vector2D_T>(other.y));
    }
    /**
     * @brief Greater-than-or-equal operator based on sum of absolute components.
     * @param other The other vector to compare.
     * @return True if this vector's component sum is greater or equal.
     */
    constexpr bool operator>=(const H2DE_Vector2D<H2DE_Vector2D_T>& other) const noexcept {
        return (H2DE::abs<H2DE_Vector2D_T>(x) + H2DE::abs<H2DE_Vector2D_T>(y) >= H2DE::abs<H2DE_Vector2D_T>(other.x) + H2DE::abs<H2DE_Vector2D_T>(other.y));
    }
    /**
     * @brief Less-than operator based on sum of absolute components.
     * @param other The other vector to compare.
     * @return True if this vector's component sum is less.
     */
    constexpr bool operator<(const H2DE_Vector2D<H2DE_Vector2D_T>& other) const noexcept {
        return !(*this >= other);
    }
    /**
     * @brief Less-than-or-equal operator based on sum of absolute components.
     * @param other The other vector to compare.
     * @return True if this vector's component sum is less or equal.
     */
    constexpr bool operator<=(const H2DE_Vector2D<H2DE_Vector2D_T>& other) const noexcept {
        return !(*this > other);
    }

    friend inline std::ostream& operator<<(std::ostream& os, const H2DE_Vector2D<H2DE_Vector2D_T>& vec) {
        os << "[x: " << vec.x << ", y: " << vec.y << "]";
        return os;
    }

    /**
     * @brief Convert vector to SDL_Point (casting components to int).
     * @return SDL_Point with x and y components.
     */
    inline operator SDL_Point() const noexcept {
        return { static_cast<int>(x), static_cast<int>(y) };
    }

    /**
     * @brief Returns a new vector with the X component increased by a given value.
     * @param value Value to add to the X component.
     */
    constexpr H2DE_Vector2D<H2DE_Vector2D_T> addX(H2DE_Vector2D_T value) const noexcept {
        return H2DE_Vector2D<H2DE_Vector2D_T>{ x + value, y };
    }
    /**
     * @brief Returns a new vector with the Y component increased by a given value.
     * @param value Value to add to the Y component.
     */
    constexpr H2DE_Vector2D<H2DE_Vector2D_T> addY(H2DE_Vector2D_T value) const noexcept {
        return H2DE_Vector2D<H2DE_Vector2D_T>{ x, y + value };
    }

    /**
     * @brief Returns a new vector with the X component decreased by a given value.
     * @param value Value to subtract from the X component.
     */
    constexpr H2DE_Vector2D<H2DE_Vector2D_T> subtractX(H2DE_Vector2D_T value) const noexcept {
        return H2DE_Vector2D<H2DE_Vector2D_T>{ x - value, y };
    }
    /**
     * @brief Returns a new vector with the Y component decreased by a given value.
     * @param value Value to subtract from the Y component.
     */
    constexpr H2DE_Vector2D<H2DE_Vector2D_T> subtractY(H2DE_Vector2D_T value) const noexcept {
        return H2DE_Vector2D<H2DE_Vector2D_T>{ x, y - value };
    }

    /**
     * @brief Returns a new vector with the X component multiplied by a given multiplier.
     * @param multiplier Multiplier to multiply the X component by.
     */
    constexpr H2DE_Vector2D<H2DE_Vector2D_T> multiplyX(float multiplier) const noexcept {
        return H2DE_Vector2D<H2DE_Vector2D_T>{ static_cast<H2DE_Vector2D_T>(x * multiplier), y };
    }
    /**
     * @brief Returns a new vector with the Y component multiplied by a given multiplier.
     * @param multiplier Multiplier to multiply the Y component by.
     */
    constexpr H2DE_Vector2D<H2DE_Vector2D_T> multiplyY(float multiplier) const noexcept {
        return H2DE_Vector2D<H2DE_Vector2D_T>{ x, static_cast<H2DE_Vector2D_T>(y * multiplier) };
    }

    /**
     * @brief Returns a new vector with the X component divided by a given divider.
     * @param divider Divider to divide the X component by.
     */
    constexpr H2DE_Vector2D<H2DE_Vector2D_T> divideX(float divider) const {
        return H2DE_Vector2D<H2DE_Vector2D_T>{ static_cast<H2DE_Vector2D_T>(x / divider), y };
    }
    /**
     * @brief Returns a new vector with the Y component divided by a given divider.
     * @param divider Divider to divide the Y component by.
     */
    constexpr H2DE_Vector2D<H2DE_Vector2D_T> divideY(float divider) const {
        return H2DE_Vector2D<H2DE_Vector2D_T>{ x, static_cast<H2DE_Vector2D_T>(y / divider) };
    }

    /**
     * @brief Returns a new vector with the X component set to a given value.
     * @param value New value for the X component.
     */
    constexpr H2DE_Vector2D<H2DE_Vector2D_T> setX(H2DE_Vector2D_T value) const noexcept {
        return H2DE_Vector2D<H2DE_Vector2D_T>{ value, y };
    }
    /**
     * @brief Returns a new vector with the Y component set to a given value.
     * @param value New value for the Y component.
     */
    constexpr H2DE_Vector2D<H2DE_Vector2D_T> setY(H2DE_Vector2D_T value) const noexcept {
        return H2DE_Vector2D<H2DE_Vector2D_T>{ x, value };
    }

    /**
     * @brief Create a rectangle at this vector's position with given size.
     * @param scale The size vector (width and height).
     * @return H2DE_Rect representing the rectangle.
     */
    constexpr H2DE_Rect<H2DE_Vector2D_T> makeRect(const H2DE_Vector2D<H2DE_Vector2D_T>& scale) const noexcept {
        return H2DE_Rect<H2DE_Vector2D_T>{ x, y, scale.x, scale.y };
    }
    /**
     * @brief Create a rectangle at this vector's position with zero size.
     * @return H2DE_Rect with zero width and height.
     */
    constexpr H2DE_Rect<H2DE_Vector2D_T> makeNullRect() const noexcept {
        return H2DE_Rect<H2DE_Vector2D_T>{ x, y, static_cast<H2DE_Vector2D_T>(0.0f), static_cast<H2DE_Vector2D_T>(0.0f) };
    }

    /**
     * @brief Check if the vector is (0,0).
     * @return True if both components are zero.
     */
    constexpr bool isNull() const noexcept {
        return (x == 0 && y == 0);
    }
    /**
     * @brief Check if at least one component is zero.
     * @return True if either x or y is zero.
     */
    constexpr bool hasNullMember() const noexcept {
        return (x == 0 || y == 0);
    }
    /**
     * @brief Rotate this vector around a pivot point by a given angle.
     * @param pivot The pivot point to rotate around.
     * @param angle The rotation angle in radians.
     * @return The rotated vector.
     */
    H2DE_Vector2D<H2DE_Vector2D_T> rotate(const H2DE_Vector2D<H2DE_Vector2D_T>& pivot, float angle) const;
    /**
     * @brief Get the vector representing half of this vector (center point).
     * 
     * @return A new vector scaled by 0.5 (half the x and y).
     */
    constexpr H2DE_Vector2D<H2DE_Vector2D_T> getCenter() const noexcept {
        return (*this * 0.5f);
    }
    /**
     * @brief Calculate the squared distance between this vector and another.
     * 
     * Avoids the cost of a sqrt, useful for distance comparisons.
     * 
     * @param other The other vector to compute the distance to.
     * @return The squared distance as the sum of squared component differences.
     */
    constexpr H2DE_Vector2D_T getDistanceSquared(const H2DE_Vector2D<H2DE_Vector2D_T>& other) const noexcept {
        return H2DE::pow(other.x - x, 2) + H2DE::pow(other.y - y, 2);
    }
    /**
     * @brief Calculate the Euclidean distance between this vector and another.
     * 
     * Uses sqrt on the squared distance.
     * 
     * @param other The other vector to compute the distance to.
     * @return The Euclidean distance as a float.
     */
    inline H2DE_Vector2D_T getDistance(const H2DE_Vector2D<H2DE_Vector2D_T>& other) const {
        return std::sqrt(getDistanceSquared(other));
    }
};

/** @brief Position in pixels, represented as integer 2D vector. */
using H2DE_PixelPos = H2DE_Vector2D<int>;
/** @brief Size in pixels, represented as integer 2D vector. */
using H2DE_PixelSize = H2DE_Vector2D<int>;
/** @brief Pivot point in pixels, represented as integer 2D vector. */
using H2DE_PixelPivot = H2DE_Vector2D<int>;
/** @brief Translation vector, usually float for smooth movement. */
using H2DE_Translate = H2DE_Vector2D<float>;
/** @brief Scale vector, usually float for smooth scaling. */
using H2DE_Scale = H2DE_Vector2D<float>;
/** @brief Pivot point with floating-point precision. */
using H2DE_Pivot = H2DE_Vector2D<float>;
