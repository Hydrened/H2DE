#include "H2DE/H2DE_utils.h"

// LEVEL SIZE OPERATIONS
H2DE_LevelSize H2DE_LevelSize::operator+(const H2DE_LevelSize& other) const {
    H2DE_LevelSize res = *this;
    res.w += other.w;
    res.h += other.h;
    return res;
}

H2DE_LevelSize H2DE_LevelSize::operator-(const H2DE_LevelSize& other) const {
    H2DE_LevelSize res = *this;
    res.w -= other.w;
    res.h -= other.h;
    return res;
}

H2DE_LevelSize H2DE_LevelSize::operator*(const float& multiplier) const {
    H2DE_LevelSize res = *this;
    res.w *= multiplier;
    res.h *= multiplier;
    return res;
}

H2DE_LevelSize H2DE_LevelSize::operator/(const float& divider) const {
    H2DE_LevelSize res = *this;
    res.w /= divider;
    res.h /= divider;
    return res;
}

H2DE_LevelSize& H2DE_LevelSize::operator+=(const H2DE_LevelSize& other) {
    w += other.w;
    h += other.h;
    return *this;
}

H2DE_LevelSize& H2DE_LevelSize::operator-=(const H2DE_LevelSize& other) {
    w -= other.w;
    h -= other.h;
    return *this;
}

H2DE_LevelSize& H2DE_LevelSize::operator*=(const float& multiplier) {
    w *= multiplier;
    h *= multiplier;
    return *this;
}

H2DE_LevelSize& H2DE_LevelSize::operator/=(const float& divider) {
    w /= divider;
    h /= divider;
    return *this;
}

// COMPARISONS
bool H2DE_LevelSize::operator==(const H2DE_LevelSize& other) const {
    return (w == other.w && h == other.h);
}

bool H2DE_LevelSize::operator>=(const H2DE_LevelSize& other) const {
    return (w >= other.w && h >= other.h);
}

bool H2DE_LevelSize::operator<=(const H2DE_LevelSize& other) const {
    return (w <= other.w && h <= other.h);
}

bool H2DE_LevelSize::operator>(const H2DE_LevelSize& other) const {
    return (w > other.w && h > other.h);
}

bool H2DE_LevelSize::operator<(const H2DE_LevelSize& other) const {
    return (w < other.w && h < other.h);
}

// METHODS
H2DE_LevelRect H2DE_LevelSize::makeHitbox(const H2DE_LevelPos& pos) const {
    return { pos.x, pos.y, w, h };
}

H2DE_LevelPos H2DE_LevelSize::toPos() const {
    return { w, h };
}
