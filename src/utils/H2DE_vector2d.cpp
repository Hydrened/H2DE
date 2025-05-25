#include "H2DE/H2DE_utils.h"

template struct H2DE_Vector2D<int>;
template struct H2DE_Vector2D<float>;

// OPERATIONS
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

// GETTER
template<typename H2DE_Vector2D_T>
H2DE_Vector2D<H2DE_Vector2D_T> H2DE_Vector2D<H2DE_Vector2D_T>::rotate(const H2DE_Vector2D<H2DE_Vector2D_T>& pivot, float angle) {
    constexpr float DEG_TO_RAD = M_PI / 180.0f;
    float rad = angle * DEG_TO_RAD;

    float cosA = std::cos(rad);
    float sinA = std::sin(rad);

    float dx = static_cast<float>(x - pivot.x);
    float dy = static_cast<float>(y - pivot.y);

    return {
        static_cast<H2DE_Vector2D_T>(cosA * dx - sinA * dy + pivot.x),
        static_cast<H2DE_Vector2D_T>(sinA * dx + cosA * dy + pivot.y)
    };
}
