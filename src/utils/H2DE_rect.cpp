#include "H2DE/H2DE_utils.h"

template struct H2DE_Rect<int>;
template struct H2DE_Rect<float>;

// OPERATIONS
template<typename H2DE_Rect_T>
H2DE_Rect<H2DE_Rect_T> H2DE_Rect<H2DE_Rect_T>::operator+(const H2DE_Rect<H2DE_Rect_T>& other) const {
    return H2DE_Rect{ x + other.x, y + other.y, w + other.w, h + other.h };
}

template<typename H2DE_Rect_T>
H2DE_Rect<H2DE_Rect_T> H2DE_Rect<H2DE_Rect_T>::operator-(const H2DE_Rect<H2DE_Rect_T>& other) const {
    return H2DE_Rect{ x - other.x, y - other.y, w - other.w, h - other.h };
}

template<typename H2DE_Rect_T>
H2DE_Rect<H2DE_Rect_T> H2DE_Rect<H2DE_Rect_T>::operator*(const float& multiplier) const {
    return H2DE_Rect{ static_cast<H2DE_Rect_T>(x * multiplier), static_cast<H2DE_Rect_T>(y * multiplier), static_cast<H2DE_Rect_T>(w * multiplier), static_cast<H2DE_Rect_T>(h * multiplier) };
}

template<typename H2DE_Rect_T>
H2DE_Rect<H2DE_Rect_T> H2DE_Rect<H2DE_Rect_T>::operator/(const float& divider) const {
    return H2DE_Rect{ static_cast<H2DE_Rect_T>(x / divider), static_cast<H2DE_Rect_T>(y / divider), static_cast<H2DE_Rect_T>(w / divider), static_cast<H2DE_Rect_T>(h / divider) };
}

template<typename H2DE_Rect_T>
H2DE_Rect<H2DE_Rect_T>& H2DE_Rect<H2DE_Rect_T>::operator+=(const H2DE_Rect<H2DE_Rect_T>& other) {
    x += other.x;
    y += other.y;
    w += other.w;
    h += other.h;
    return *this;
}

template<typename H2DE_Rect_T>
H2DE_Rect<H2DE_Rect_T>& H2DE_Rect<H2DE_Rect_T>::operator-=(const H2DE_Rect<H2DE_Rect_T>& other) {
    x -= other.x;
    y -= other.y;
    w -= other.w;
    h -= other.h;
    return *this;
}

template<typename H2DE_Rect_T>
H2DE_Rect<H2DE_Rect_T>& H2DE_Rect<H2DE_Rect_T>::operator*=(const float& multiplier) {
    x *= multiplier;
    y *= multiplier;
    w *= multiplier;
    h *= multiplier;
    return *this;
}

template<typename H2DE_Rect_T>
H2DE_Rect<H2DE_Rect_T>& H2DE_Rect<H2DE_Rect_T>::operator/=(const float& divider) {
    x /= divider;
    y /= divider;
    w /= divider;
    h /= divider;
    return *this;
}

// COMPARISONS
template<typename H2DE_Rect_T>
bool H2DE_Rect<H2DE_Rect_T>::operator==(const H2DE_Rect& other) const {
    return x == other.x && y == other.y && w == other.w && h == other.h;
}

template<typename H2DE_Rect_T>
bool H2DE_Rect<H2DE_Rect_T>::operator!=(const H2DE_Rect& other) const {
    return !(*this == other);
}

// METHODS
template<typename H2DE_Rect_T>
H2DE_Rect<H2DE_Rect_T> H2DE_Rect<H2DE_Rect_T>::addPos(const H2DE_Vector2D<H2DE_Rect_T>& pos) {
    x += pos.x;
    y += pos.y;
    return *this;
}

template<typename H2DE_Rect_T>
H2DE_Rect<H2DE_Rect_T> H2DE_Rect<H2DE_Rect_T>::addSize(const H2DE_Vector2D<H2DE_Rect_T>& size) {
    w += size.x;
    h += size.y;
    return *this;
}

template<typename H2DE_Rect_T>
H2DE_Rect<H2DE_Rect_T> H2DE_Rect<H2DE_Rect_T>::substractPos(const H2DE_Vector2D<H2DE_Rect_T>& pos) {
    x -= pos.x;
    y -= pos.y;
    return *this;
}

template<typename H2DE_Rect_T>
H2DE_Rect<H2DE_Rect_T> H2DE_Rect<H2DE_Rect_T>::substractSize(const H2DE_Vector2D<H2DE_Rect_T>& size) {
    w -= size.x;
    h -= size.y;
    return *this;
}

template<typename H2DE_Rect_T>
H2DE_Rect<H2DE_Rect_T> H2DE_Rect<H2DE_Rect_T>::multiplyPos(const float& multiplier) {
    x *= multiplier;
    y *= multiplier;
    return *this;
}

template<typename H2DE_Rect_T>
H2DE_Rect<H2DE_Rect_T> H2DE_Rect<H2DE_Rect_T>::multiplySize(const float& multiplier) {
    w *= multiplier;
    h *= multiplier;
    return *this;
}

template<typename H2DE_Rect_T>
H2DE_Rect<H2DE_Rect_T> H2DE_Rect<H2DE_Rect_T>::dividePos(const float& divider) {
    x /= divider;
    y /= divider;
    return *this;
}

template<typename H2DE_Rect_T>
H2DE_Rect<H2DE_Rect_T> H2DE_Rect<H2DE_Rect_T>::divideSize(const float& divider) {
    w /= divider;
    h /= divider;
    return *this;
}

// GETTER
template<typename H2DE_Rect_T>
H2DE_Vector2D<H2DE_Rect_T> H2DE_Rect<H2DE_Rect_T>::getPos() const {
    return H2DE_Vector2D<H2DE_Rect_T>{ x, y };
}

template<typename H2DE_Rect_T>
H2DE_Vector2D<H2DE_Rect_T> H2DE_Rect<H2DE_Rect_T>::getSize() const {
    return H2DE_Vector2D<H2DE_Rect_T>{ w, h };
}

template<typename H2DE_Rect_T>
H2DE_Vector2D<H2DE_Rect_T> H2DE_Rect<H2DE_Rect_T>::getCenter() const {
    return getPos() + getSize().getCenter();
}
