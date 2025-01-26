#include "H2DE_utils.h"

std::ostream& operator<<(std::ostream& os, const H2DE_AbsPos& pos) {
    os << "x: " << pos.x << " y: " << pos.y;
    return os;
}

std::ostream& operator<<(std::ostream& os, const H2DE_AbsSize& size) {
    os << "w: " << size.w << " h: " << size.h;
    return os;
}

std::ostream& operator<<(std::ostream& os, const H2DE_AbsRect& rect) {
    os << "x: " << rect.x << " y: " << rect.y << " w: " << rect.w << " h: " << rect.h;
    return os;
}

std::ostream& operator<<(std::ostream& os, const H2DE_LevelPos& pos) {
    os << "x: " << pos.x << " y: " << pos.y;
    return os;
}

std::ostream& operator<<(std::ostream& os, const H2DE_LevelSize& size) {
    os << "w: " << size.w << " h: " << size.h;
    return os;
}

std::ostream& operator<<(std::ostream& os, const H2DE_LevelVelocity& velocity) {
    os << "x: " << velocity.x << " y: " << velocity.y;
    return os;
}

std::ostream& operator<<(std::ostream& os, const H2DE_LevelHitbox& hitbox) {
    os << "x: " << hitbox.x << " y: " << hitbox.y << " w: " << hitbox.w << " h: " << hitbox.h;
    return os;
}

std::ostream& operator<<(std::ostream& os, const H2DE_Scale& scale) {
    os << "x: " << scale.x << " y: " << scale.y;
    return os;
}

std::ostream& operator<<(std::ostream& os, const H2DE_ColorRGB& rgb) {
    os << "r: " << static_cast<int>(rgb.r) << " g: " << static_cast<int>(rgb.g) << " b: " << static_cast<int>(rgb.b) << " a: " << static_cast<int>(rgb.a);
    return os;
}

std::ostream& operator<<(std::ostream& os, const H2DE_ColorHSV& hsv) {
    os << "h: " << hsv.h << " s: " << hsv.s << " v: " << hsv.v << " a: " << hsv.a;
    return os;
}
