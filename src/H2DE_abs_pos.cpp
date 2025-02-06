#include "H2DE/H2DE_utils.h"

// ABS SIZE 
H2DE_AbsPos H2DE_AbsPos::operator+(const H2DE_AbsSize& size) const {
    H2DE_AbsPos res = *this;
    res.x += size.w;
    res.y += size.h;
    return res;
}

// CONVERT
H2DE_AbsPos::operator SDL_Point() const {
    return { x, y };
}

// METHODS
H2DE_AbsRect H2DE_AbsPos::makeRect(H2DE_AbsSize size) const {
    return { x, y, size.w, size.h };
}
