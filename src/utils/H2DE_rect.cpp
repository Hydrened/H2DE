#include "H2DE/H2DE_utils.h"

template struct H2DE_Rect<int>;
template struct H2DE_Rect<float>;

// OPERATIONS
template<typename H2DE_Rect_T>
H2DE_Rect<H2DE_Rect_T> H2DE_Rect<H2DE_Rect_T>::operator+(const H2DE_Rect<H2DE_Rect_T>& other) const {
    return H2DE_Rect{
        x + other.x,
        y + other.y,
        w + other.w,
        h + other.h
    };
}

template<typename H2DE_Rect_T>
H2DE_Rect<H2DE_Rect_T> H2DE_Rect<H2DE_Rect_T>::operator-(const H2DE_Rect<H2DE_Rect_T>& other) const {
    return H2DE_Rect{
        x - other.x,
        y - other.y,
        w - other.w,
        h - other.h
    };
}

template<typename H2DE_Rect_T>
H2DE_Rect<H2DE_Rect_T> H2DE_Rect<H2DE_Rect_T>::operator*(float multiplier) const {
    return H2DE_Rect{
        static_cast<H2DE_Rect_T>(x * multiplier),
        static_cast<H2DE_Rect_T>(y * multiplier),
        static_cast<H2DE_Rect_T>(w * multiplier),
        static_cast<H2DE_Rect_T>(h * multiplier)
    };
}

template<typename H2DE_Rect_T>
H2DE_Rect<H2DE_Rect_T> H2DE_Rect<H2DE_Rect_T>::operator/(float divider) const {
    return H2DE_Rect{
        static_cast<H2DE_Rect_T>(x / divider),
        static_cast<H2DE_Rect_T>(y / divider),
        static_cast<H2DE_Rect_T>(w / divider),
        static_cast<H2DE_Rect_T>(h / divider)
    };
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
H2DE_Rect<H2DE_Rect_T>& H2DE_Rect<H2DE_Rect_T>::operator*=(float multiplier) {
    x *= multiplier;
    y *= multiplier;
    w *= multiplier;
    h *= multiplier;
    return *this;
}

template<typename H2DE_Rect_T>
H2DE_Rect<H2DE_Rect_T>& H2DE_Rect<H2DE_Rect_T>::operator/=(float divider) {
    x /= divider;
    y /= divider;
    w /= divider;
    h /= divider;
    return *this;
}

template<typename H2DE_Rect_T>
H2DE_Rect<H2DE_Rect_T>::operator SDL_Rect() const {
    return SDL_Rect{
        static_cast<int>(x),
        static_cast<int>(y),
        static_cast<int>(w),
        static_cast<int>(h)
    };
}

// COMPARISONS
template<typename H2DE_Rect_T>
const bool H2DE_Rect<H2DE_Rect_T>::operator==(const H2DE_Rect& other) const {
    return x == other.x && y == other.y && w == other.w && h == other.h;
}

template<typename H2DE_Rect_T>
const bool H2DE_Rect<H2DE_Rect_T>::operator!=(const H2DE_Rect& other) const {
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
H2DE_Rect<H2DE_Rect_T> H2DE_Rect<H2DE_Rect_T>::multiplyPos(float multiplier) {
    x *= multiplier;
    y *= multiplier;
    return *this;
}

template<typename H2DE_Rect_T>
H2DE_Rect<H2DE_Rect_T> H2DE_Rect<H2DE_Rect_T>::multiplySize(float multiplier) {
    w *= multiplier;
    h *= multiplier;
    return *this;
}

template<typename H2DE_Rect_T>
H2DE_Rect<H2DE_Rect_T> H2DE_Rect<H2DE_Rect_T>::dividePos(float divider) {
    x /= divider;
    y /= divider;
    return *this;
}

template<typename H2DE_Rect_T>
H2DE_Rect<H2DE_Rect_T> H2DE_Rect<H2DE_Rect_T>::divideSize(float divider) {
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

template<typename H2DE_Rect_T>
const bool H2DE_Rect<H2DE_Rect_T>::collides(const H2DE_Rect<H2DE_Rect_T>& rect) const {
    return (
        rect.x + rect.w > x &&
        rect.x < x + w &&
        rect.y + rect.h > y &&
        rect.y < y + h
    );
}

template<typename H2DE_Rect_T>
const bool H2DE_Rect<H2DE_Rect_T>::collides(const H2DE_Vector2D<H2DE_Rect_T>& pos) const {
    return (
        pos.x >= x &&
        pos.x <= x + w &&
        pos.y >= y &&
        pos.y <= y + h
    );
}

template<typename H2DE_Rect_T>
const std::optional<H2DE_Face> H2DE_Rect<H2DE_Rect_T>::getCollidedFace(const H2DE_Rect<H2DE_Rect_T>& rect) const {
    if (collides(rect)) {
        float overlapLeft = rect.x + rect.w - x;
        float overlapRight = x + w - rect.x;
        float overlapTop = rect.y + rect.h - y;
        float overlapBottom = y + h - rect.y;

        float minOverlap = std::min({overlapLeft, overlapRight, overlapTop, overlapBottom});

        if (minOverlap == overlapLeft) {
            return H2DE_FACE_LEFT;
            
        } else if (minOverlap == overlapRight) {
            return H2DE_FACE_RIGHT;

        } else if (minOverlap == overlapTop) {
            return H2DE_FACE_TOP;

        } else {
            return H2DE_FACE_BOTTOM;
        }

    } else return std::nullopt;
}
