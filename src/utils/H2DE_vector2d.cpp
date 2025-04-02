#include "H2DE/H2DE_utils.h"

template struct H2DE_Vector2D<int>;
template struct H2DE_Vector2D<float>;

// OPERATIONS
template<typename H2DE_Vector2D_T>
H2DE_Vector2D<H2DE_Vector2D_T> H2DE_Vector2D<H2DE_Vector2D_T>::operator+(const H2DE_Vector2D& other) const {
    return H2DE_Vector2D{ x + other.x, y + other.y };
}

template<typename H2DE_Vector2D_T>
H2DE_Vector2D<H2DE_Vector2D_T> H2DE_Vector2D<H2DE_Vector2D_T>::operator-(const H2DE_Vector2D& other) const {
    return H2DE_Vector2D{ x - other.x, y - other.y };
}

template<typename H2DE_Vector2D_T>
H2DE_Vector2D<H2DE_Vector2D_T> H2DE_Vector2D<H2DE_Vector2D_T>::operator*(float multiplier) const {
    return H2DE_Vector2D{ static_cast<H2DE_Vector2D_T>(x * multiplier), static_cast<H2DE_Vector2D_T>(y * multiplier) };
}

template<typename H2DE_Vector2D_T>
H2DE_Vector2D<H2DE_Vector2D_T> H2DE_Vector2D<H2DE_Vector2D_T>::operator/(float divider) const {
    return H2DE_Vector2D{ static_cast<H2DE_Vector2D_T>(x / divider), static_cast<H2DE_Vector2D_T>(y / divider) };
}

template<typename H2DE_Vector2D_T>
H2DE_Vector2D<H2DE_Vector2D_T>& H2DE_Vector2D<H2DE_Vector2D_T>::operator+=(const H2DE_Vector2D<H2DE_Vector2D_T>& other) {
    x += other.x;
    y += other.y;
    return *this;
}

template<typename H2DE_Vector2D_T>
H2DE_Vector2D<H2DE_Vector2D_T>& H2DE_Vector2D<H2DE_Vector2D_T>::operator-=(const H2DE_Vector2D<H2DE_Vector2D_T>& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

template<typename H2DE_Vector2D_T>
H2DE_Vector2D<H2DE_Vector2D_T>& H2DE_Vector2D<H2DE_Vector2D_T>::operator*=(float multiplier) {
    x *= multiplier;
    y *= multiplier;
    return *this;
}

template<typename H2DE_Vector2D_T>
H2DE_Vector2D<H2DE_Vector2D_T>& H2DE_Vector2D<H2DE_Vector2D_T>::operator/=(float divider) {
    x /= divider;
    y /= divider;
    return *this;
}

template<typename H2DE_Vector2D_T>
H2DE_Vector2D<H2DE_Vector2D_T>::operator SDL_Point() const {
    return { static_cast<int>(x), static_cast<int>(y) };
}

// COMPARISONS
template<typename H2DE_Vector2D_T>
const bool H2DE_Vector2D<H2DE_Vector2D_T>::operator==(const H2DE_Vector2D<H2DE_Vector2D_T>& other) const {
    return x == other.x && y == other.y;
}

template<typename H2DE_Vector2D_T>
const bool H2DE_Vector2D<H2DE_Vector2D_T>::operator!=(const H2DE_Vector2D<H2DE_Vector2D_T>& other) const {
    return !(*this == other);
}

template<typename H2DE_Vector2D_T>
const bool H2DE_Vector2D<H2DE_Vector2D_T>::operator>(const H2DE_Vector2D<H2DE_Vector2D_T>& other) const {
    H2DE_Vector2D_T sum1 = std::abs(x) + std::abs(y);
    H2DE_Vector2D_T sum2 = std::abs(other.x) + std::abs(other.y);
    return sum1 > sum2;
}

template<typename H2DE_Vector2D_T>
const bool H2DE_Vector2D<H2DE_Vector2D_T>::operator>=(const H2DE_Vector2D<H2DE_Vector2D_T>& other) const {
    H2DE_Vector2D_T sum1 = std::abs(x) + std::abs(y);
    H2DE_Vector2D_T sum2 = std::abs(other.x) + std::abs(other.y);
    return sum1 >= sum2;
}

template<typename H2DE_Vector2D_T>
const bool H2DE_Vector2D<H2DE_Vector2D_T>::operator<(const H2DE_Vector2D<H2DE_Vector2D_T>& other) const {
    return !(*this >= other);
}

template<typename H2DE_Vector2D_T>
const bool H2DE_Vector2D<H2DE_Vector2D_T>::operator<=(const H2DE_Vector2D<H2DE_Vector2D_T>& other) const {
    return !(*this > other);
}

// METHODS
template<typename H2DE_Vector2D_T>
H2DE_Rect<H2DE_Vector2D_T> H2DE_Vector2D<H2DE_Vector2D_T>::makeRect(const H2DE_Vector2D<H2DE_Vector2D_T>& size) const {
    return H2DE_Rect<H2DE_Vector2D_T>{ x, y, size.x, size.y };
}

// GETTER
template<typename H2DE_Vector2D_T>
const bool H2DE_Vector2D<H2DE_Vector2D_T>::isNull() const {
    return x == 0 && y == 0;
}

template<typename H2DE_Vector2D_T>
H2DE_Vector2D<H2DE_Vector2D_T> H2DE_Vector2D<H2DE_Vector2D_T>::rotate(const H2DE_Vector2D<H2DE_Vector2D_T>& center, float angle) {
    constexpr float DEG_TO_RAD = M_PI / 180.0f;
    float rad = angle * DEG_TO_RAD;

    float cosA = std::cos(rad);
    float sinA = std::sin(rad);

    float dx = static_cast<float>(x - center.x);
    float dy = static_cast<float>(y - center.y);

    return {
        static_cast<H2DE_Vector2D_T>(cosA * dx - sinA * dy + center.x),
        static_cast<H2DE_Vector2D_T>(sinA * dx + cosA * dy + center.y)
    };
}

template<typename H2DE_Vector2D_T>
H2DE_Vector2D<H2DE_Vector2D_T> H2DE_Vector2D<H2DE_Vector2D_T>::getCenter() const {
    return *this / 2;
}
