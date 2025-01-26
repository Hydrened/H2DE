#include "H2DE_utils.h"

// CONVERT
H2DE_AbsPos::operator SDL_Point() const {
    return { x, y };
}

// METHODS
H2DE_AbsRect H2DE_AbsPos::makeRect(H2DE_AbsSize size) const {
    return { x, y, size.w, size.h };
}
