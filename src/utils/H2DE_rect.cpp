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
bool H2DE_Rect<H2DE_Rect_T>::collides(const H2DE_Vector2D<H2DE_Rect_T>& translate, float radius) const {
    float closestX = std::max(x, std::min(translate.x, x + w));
    float closestY = std::max(y, std::min(translate.y, y + h));

    float dx = translate.x - closestX;
    float dy = translate.y - closestY;

    return ((dx * dx + dy * dy) <= (radius * radius));
}

template<typename H2DE_Rect_T>
const std::optional<H2DE_Face> H2DE_Rect<H2DE_Rect_T>::getCollidedFace(const H2DE_Rect<H2DE_Rect_T>& rect) const {
    H2DE_Rect_T dx = rect.x - x;
    H2DE_Rect_T px = (w + rect.w) - std::abs(dx);

    if (px <= 0) {
        return std::nullopt;
    }

    H2DE_Rect_T dy = rect.y - y;
    H2DE_Rect_T py = (h + rect.h) - std::abs(dy);

    if (py <= 0) {
        return std::nullopt;
    }

    if (px < py) {
        return ((dx < 0) ? H2DE_FACE_LEFT : H2DE_FACE_RIGHT);
    } else {
        return ((dy < 0) ? H2DE_FACE_TOP : H2DE_FACE_BOTTOM);
    }
}
