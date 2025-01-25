#include "H2DE_utils.h"

// LEVEL POS OPERATIONS
H2DE_LevelHitbox H2DE_LevelHitbox::operator+(const H2DE_LevelPos& pos) const {
    H2DE_LevelHitbox res = *this;
    res.x += pos.x;
    res.y += pos.y;
    return res;
}

H2DE_LevelHitbox H2DE_LevelHitbox::operator-(const H2DE_LevelPos& pos) const {
    H2DE_LevelHitbox res = *this;
    res.x -= pos.x;
    res.y -= pos.y;
    return res;
}

// LEVEL SIZE OPERATIONS
H2DE_LevelHitbox H2DE_LevelHitbox::operator+(const H2DE_LevelSize& size) const {
    H2DE_LevelHitbox res = *this;
    res.w += size.w;
    res.h += size.h;
    return res;
}

H2DE_LevelHitbox H2DE_LevelHitbox::operator-(const H2DE_LevelSize& size) const {
    H2DE_LevelHitbox res = *this;
    res.w -= size.w;
    res.h -= size.h;
    return res;
}

// GETTER
H2DE_LevelPos H2DE_LevelHitbox::getPos() const {
    return { x, y };
}

H2DE_LevelSize H2DE_LevelHitbox::getSize() const {
    return { w, h };
}

H2DE_LevelPos H2DE_LevelHitbox::getCenter() const {
    return getPos() + (getSize() / 2.0f).toPos();
}

// METHODS
H2DE_Face H2DE_LevelHitbox::collides(const H2DE_LevelHitbox& other) const {
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

        if (minOverlap == overlapLeft) return LEFT;
        else if (minOverlap == overlapRight) return RIGHT;
        else if (minOverlap == overlapTop) return TOP;
        else return BOTTOM;
    } else return NONE;
}
