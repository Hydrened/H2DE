#include "H2DE/H2DE_utils.h"

template struct H2DE_Rect<int>;
template struct H2DE_Rect<float>;

// OPERATIONS
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

template<typename H2DE_Rect_T>
void H2DE_Rect<H2DE_Rect_T>::snap(const H2DE_Rect<H2DE_Rect_T>& rect, H2DE_Face face) {
    switch (face) {
        case H2DE_FACE_TOP:
            y = rect.y + rect.h;
            break;

        case H2DE_FACE_RIGHT:
            x = rect.x - w;
            break;

        case H2DE_FACE_BOTTOM:
            y = rect.y - h;
            break;

        case H2DE_FACE_LEFT:
            x = rect.x + rect.w;
            break;

        default: return;
    }
}

// GETTER
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
