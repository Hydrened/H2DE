#include "H2DE/utils/H2DE_utils.h"

template struct H2DE_Rect<int>;
template struct H2DE_Rect<float>;

// OPERATIONS
template<typename H2DE_Rect_T>
H2DE_Rect<H2DE_Rect_T>& H2DE_Rect<H2DE_Rect_T>::operator+=(const H2DE_Rect<H2DE_Rect_T>& other) noexcept {
    x += other.x;
    y += other.y;
    w += other.w;
    h += other.h;
    return *this;
}

template<typename H2DE_Rect_T>
H2DE_Rect<H2DE_Rect_T>& H2DE_Rect<H2DE_Rect_T>::operator-=(const H2DE_Rect<H2DE_Rect_T>& other) noexcept {
    x -= other.x;
    y -= other.y;
    w -= other.w;
    h -= other.h;
    return *this;
}

template<typename H2DE_Rect_T>
H2DE_Rect<H2DE_Rect_T>& H2DE_Rect<H2DE_Rect_T>::operator*=(float multiplier) noexcept {
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
void H2DE_Rect<H2DE_Rect_T>::snap(const H2DE_Rect<H2DE_Rect_T>& rect, H2DE_Face face) noexcept {
    const H2DE_Vector2D<H2DE_Rect_T> halfScale = getScale() * 0.5f;
    const H2DE_Vector2D<H2DE_Rect_T> rectHalfScale = rect.getScale() * 0.5f;

    switch (face) {
        case H2DE_FACE_TOP:
            y = rect.y + rectHalfScale.y + halfScale.y;
            break;

        case H2DE_FACE_RIGHT:
            x = rect.x - rectHalfScale.x - halfScale.x;
            break;

        case H2DE_FACE_BOTTOM:
            y = rect.y - rectHalfScale.y - halfScale.y;
            break;

        case H2DE_FACE_LEFT:
            x = rect.x + rectHalfScale.x + halfScale.x;
            break;

        default: return;
    }
}

// GETTER
template<typename H2DE_Rect_T>
bool H2DE_Rect<H2DE_Rect_T>::collides(const H2DE_Vector2D<H2DE_Rect_T>& translate, float radius) const noexcept {
    H2DE_Rect_T halfW = w * 0.5f;
    H2DE_Rect_T halfH = h * 0.5f;

    H2DE_Rect_T left = x - halfW;
    H2DE_Rect_T right = x + halfW;
    H2DE_Rect_T top = y - halfH;
    H2DE_Rect_T bottom = y + halfH;

    H2DE_Rect_T closestX = H2DE::clamp(translate.x, left, right);
    H2DE_Rect_T closestY = H2DE::clamp(translate.y, top, bottom);

    H2DE_Rect_T dx = translate.x - closestX;
    H2DE_Rect_T dy = translate.y - closestY;

    return ((dx * dx + dy * dy) <= (radius * radius));
}

template<typename H2DE_Rect_T>
const std::optional<H2DE_Face> H2DE_Rect<H2DE_Rect_T>::getCollidedFace(const H2DE_Rect<H2DE_Rect_T>& rect) const noexcept {
    H2DE_Rect_T dx = rect.x - x;
    H2DE_Rect_T dy = rect.y - y;

    H2DE_Rect_T px = (w + rect.w) * 0.5f - H2DE::abs(dx);
    if (px <= 0) {
        return std::nullopt;
    }

    H2DE_Rect_T py = (h + rect.h) * 0.5f - H2DE::abs(dy);
    if (py <= 0) {
        return std::nullopt;
    }

    if (px < py) {
        return (dx < 0) ? H2DE_FACE_LEFT : H2DE_FACE_RIGHT;
    } else {
        return (dy < 0) ? H2DE_FACE_TOP : H2DE_FACE_BOTTOM;
    }
}

template<typename H2DE_Rect_T>
std::array<H2DE_Vector2D<H2DE_Rect_T>, 4> H2DE_Rect<H2DE_Rect_T>::getCorners() const noexcept {
    H2DE_Rect_T halfW = static_cast<H2DE_Rect_T>(w * 0.5f);
    H2DE_Rect_T halfH = static_cast<H2DE_Rect_T>(h * 0.5f);

    return {
        H2DE_Vector2D<H2DE_Rect_T>{ x - halfW, y - halfH },
        H2DE_Vector2D<H2DE_Rect_T>{ x + halfW, y - halfH },
        H2DE_Vector2D<H2DE_Rect_T>{ x + halfW, y + halfH },
        H2DE_Vector2D<H2DE_Rect_T>{ x - halfW, y + halfH }
    };
}
