#include "H2DE/H2DE_utils.h"

// LEVEL POS OPERATIONS
H2DE_LevelRect H2DE_LevelRect::operator+(const H2DE_LevelPos& pos) const {
    H2DE_LevelRect res = *this;
    res.x += pos.x;
    res.y += pos.y;
    return res;
}

H2DE_LevelRect H2DE_LevelRect::operator-(const H2DE_LevelPos& pos) const {
    H2DE_LevelRect res = *this;
    res.x -= pos.x;
    res.y -= pos.y;
    return res;
}

H2DE_LevelRect& H2DE_LevelRect::operator+=(const H2DE_LevelPos& pos) {
    x += pos.x;
    y += pos.y;
    return *this;
}

H2DE_LevelRect& H2DE_LevelRect::operator-=(const H2DE_LevelPos& pos) {
    x -= pos.x;
    y -= pos.y;
    return *this;
}

// LEVEL SIZE OPERATIONS
H2DE_LevelRect H2DE_LevelRect::operator+(const H2DE_LevelSize& size) const {
    H2DE_LevelRect res = *this;
    res.w += size.w;
    res.h += size.h;
    return res;
}

H2DE_LevelRect H2DE_LevelRect::operator-(const H2DE_LevelSize& size) const {
    H2DE_LevelRect res = *this;
    res.w -= size.w;
    res.h -= size.h;
    return res;
}

H2DE_LevelRect& H2DE_LevelRect::operator+=(const H2DE_LevelSize& size) {
    w += size.w;
    h += size.h;
    return *this;
}

H2DE_LevelRect& H2DE_LevelRect::operator-=(const H2DE_LevelSize& size) {
    w -= size.w;
    h -= size.h;
    return *this;
}

// GETTER
H2DE_LevelPos H2DE_LevelRect::getPos() const {
    return { x, y };
}

H2DE_LevelSize H2DE_LevelRect::getSize() const {
    return { w, h };
}

H2DE_LevelPos H2DE_LevelRect::getCenter() const {
    return getPos() + (getSize() / 2.0f).toPos();
}

// METHODS
H2DE_Face H2DE_LevelRect::collides(const H2DE_LevelRect& other) const {
    bool collides = (
        other.x + other.w >= x &&
        other.x <= x + w &&
        other.y + other.h >= y &&
        other.y <= y + h
    );

    if (collides) {
        float overlapLeft = other.x + other.w - x;
        float overlapRight = x + w - other.x;
        float overlapTop = other.y + other.h - y;
        float overlapBottom = y + h - other.y;

        float minOverlap = std::min({overlapLeft, overlapRight, overlapTop, overlapBottom});

        if (minOverlap == overlapLeft) return H2DE_LEFT_FACE;
        else if (minOverlap == overlapRight) return H2DE_RIGHT_FACE;
        else if (minOverlap == overlapTop) return H2DE_TOP_FACE;
        else return H2DE_BOTTOM_FACE;
    } else return H2DE_NO_FACE;
}
