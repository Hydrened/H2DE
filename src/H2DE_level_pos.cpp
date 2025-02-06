#include "H2DE/H2DE_utils.h"

// LEVEL POS OPERATIONS
H2DE_LevelPos H2DE_LevelPos::operator+(const H2DE_LevelPos& other) const {
    H2DE_LevelPos res = *this;
    res.x += other.x;
    res.y += other.y;
    return res;
}

H2DE_LevelPos H2DE_LevelPos::operator-(const H2DE_LevelPos& other) const {
    H2DE_LevelPos res = *this;
    res.x -= other.x;
    res.y -= other.y;
    return res;
}

H2DE_LevelPos H2DE_LevelPos::operator*(const float& multiplier) const {
    H2DE_LevelPos res = *this;
    res.x *= multiplier;
    res.y *= multiplier;
    return res;
}

H2DE_LevelPos H2DE_LevelPos::operator/(const float& divider) const {
    H2DE_LevelPos res = *this;
    res.x /= divider;
    res.y /= divider;
    return res;
}

// VELOCITY OPERATIONS
H2DE_LevelPos H2DE_LevelPos::operator+(const H2DE_LevelVelocity& velocity) const {
    H2DE_LevelPos res = *this;
    res.x += velocity.x;
    res.y += velocity.y;
    return res;
}

H2DE_LevelPos H2DE_LevelPos::operator-(const H2DE_LevelVelocity& velocity) const {
    H2DE_LevelPos res = *this;
    res.x -= velocity.x;
    res.y -= velocity.y;
    return res;
}

// COMPARISONS
bool H2DE_LevelPos::operator==(const H2DE_LevelPos& other) const {
    return (x == other.x && y == other.y);
}

// METHODS
H2DE_LevelRect H2DE_LevelPos::makeHitbox(const H2DE_LevelSize& size) const {
    return { x, y, size.w, size.h };
}
