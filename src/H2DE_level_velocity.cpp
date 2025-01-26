#include "H2DE_utils.h"

// LEVEL VELOCITY OPERATIONS
H2DE_LevelVelocity H2DE_LevelVelocity::operator+(const H2DE_LevelVelocity& other) const {
    H2DE_LevelVelocity res = *this;
    res.x += other.x;
    res.y += other.y;
    return res;
}

H2DE_LevelVelocity H2DE_LevelVelocity::operator-(const H2DE_LevelVelocity& other) const {
    H2DE_LevelVelocity res = *this;
    res.x -= other.x;
    res.y -= other.y;
    return res;
}

H2DE_LevelVelocity H2DE_LevelVelocity::operator*(const float& multiplier) const {
    H2DE_LevelVelocity res = *this;
    res.x *= multiplier;
    res.y *= multiplier;
    return res;
}

H2DE_LevelVelocity H2DE_LevelVelocity::operator/(const float& divider) const {
    H2DE_LevelVelocity res = *this;
    res.x /= divider;
    res.y /= divider;
    return res;
}

// LEVEL POS OPERATIONS
H2DE_LevelPos H2DE_LevelVelocity::operator+(const H2DE_LevelPos& pos) const {
    H2DE_LevelPos res = pos;
    res.x += x;
    res.y += y;
    return res;
}

// COMPARISONS
bool H2DE_LevelVelocity::operator==(const H2DE_LevelVelocity& other) const {
    return (x == other.x && y == other.y);
}

bool H2DE_LevelVelocity::operator>=(const H2DE_LevelVelocity& other) const {
    return (x >= other.x && y >= other.y);
}

bool H2DE_LevelVelocity::operator<=(const H2DE_LevelVelocity& other) const {
    return (x <= other.x && y <= other.y);
}

bool H2DE_LevelVelocity::operator>(const H2DE_LevelVelocity& other) const {
    return (x > other.x && y > other.y);
}

bool H2DE_LevelVelocity::operator<(const H2DE_LevelVelocity& other) const {
    return (x < other.x && y < other.y);
}

// METHODS
bool H2DE_LevelVelocity::isNull() const {
    return (*this == H2DE_LevelVelocity{ 0.0f, 0.0f });
}
